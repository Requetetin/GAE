#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <cmath>
#include "print.h"
#include "Systems.h"
#include "Components.h"
#include "constants.h"

#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "Game/Graphics/TextureManager.h"

SpriteSetupSystem::SpriteSetupSystem(SDL_Renderer* renderer)
    : renderer(renderer) { }

SpriteSetupSystem::~SpriteSetupSystem() {
    auto view = scene->r.view<SpriteComponent>();

    for(auto entity : view) {
        const auto spriteComponent = view.get<SpriteComponent>(entity);
        TextureManager::UnloadTexture(spriteComponent.name, spriteComponent.shader.name);
    }
}

void SpriteSetupSystem::run() {
    auto view = scene->r.view<SpriteComponent>();

    for(auto entity : view) {
        const auto spriteComponent = view.get<SpriteComponent>(entity);
        TextureManager::LoadTexture(spriteComponent.name, renderer, spriteComponent.shader);
    }
}

void SpriteRenderSystem::run(SDL_Renderer* renderer) {
    auto view = scene->r.view<TransformComponent, SpriteComponent>();

    for(auto entity : view) {
        const auto spriteComponent = view.get<SpriteComponent>(entity);
        const auto transformComponent = view.get<TransformComponent>(entity);
  
        Texture* texture = TextureManager::GetTexture(spriteComponent.name, spriteComponent.shader.name);
  
        SDL_Rect clip = {
            spriteComponent.xIndex * spriteComponent.w,
            spriteComponent.yIndex * spriteComponent.h,
            spriteComponent.w,
            spriteComponent.h
        };

        int scale = 2;

        texture->render(
            transformComponent.x * scale,
            transformComponent.y * scale,
            16 * scale,
            16 * scale,
            &clip
        );
    }
}

void SpriteUpdateSystem::run(double dT) {
    auto view = scene->r.view<SpriteComponent>();

    Uint32 now = SDL_GetTicks();

    for(auto entity : view) {
        auto& spriteComponent = view.get<SpriteComponent>(entity);

        if (spriteComponent.animationFrames > 0) {
            float timeSinceLastUpdate = now - spriteComponent.lastUpdate;

            int framesToUpdate = static_cast<int>(
                timeSinceLastUpdate / 
                spriteComponent.animationDuration * spriteComponent.animationFrames
            );

            if (framesToUpdate > 0) {
                spriteComponent.xIndex += framesToUpdate;
                spriteComponent.xIndex %= spriteComponent.animationFrames;
                spriteComponent.lastUpdate = now;            
            }
        }
    }
}

TilemapSetupSystem::TilemapSetupSystem(SDL_Renderer* renderer)
    : renderer(renderer) {}

TilemapSetupSystem::~TilemapSetupSystem() {

}

void TilemapSetupSystem::run() {
    Texture* grassTexture = TextureManager::LoadTexture("Tilesets/Grass_Tileset.png", renderer);
    Texture* playerTexture = TextureManager::LoadTexture("Sprites/MainChar/SpriteSheet.png", renderer);
    auto& tilemap = scene->world->get<TilemapComponent>();
    tilemap.width = 64;
    tilemap.height = 36;
    tilemap.tileSize = 16;

    std::vector<int> tmap;
    std::string line;
    std::string word;
    std::ifstream Level1("assets/Tilesets/Levels/1/Screens/1/1-1.txt");
    while (std::getline (Level1, line)) {
        std::stringstream ss(line);
        while (ss >> word) {
            tmap.push_back(stoi(word));
        }
    }
    Level1.close();

    for (int y = 0; y < tilemap.height; y++) {
        for (int x = 0; x < tilemap.width; x++) {
            Entity* tile = scene->createEntity(
                "TILE",
                x * tilemap.tileSize,
                y * tilemap.tileSize
            );
            auto world = scene->world->get<PhysicsComponent>().b2d;

            tile->addComponent<TileComponent>(grassTexture, tmap[y * tilemap.width + x]);
            const auto index = tmap[y * tilemap.width + x];
            if (index != 0) {
                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position.Set((x + 0.5) * 16, (y + 0.5) * 16);

                b2Body* body = world->CreateBody(&bodyDef);

                b2PolygonShape box;
                box.SetAsBox(8, 8);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &box;
                fixtureDef.density = 0.0000001f;
                fixtureDef.friction = 0.0f;

                body->CreateFixture(&fixtureDef);

                body->GetUserData().pointer = reinterpret_cast<uintptr_t>(scene->player);

                tile->addComponent<RigidBodyComponent>(
                    bodyDef.type,
                    body,
                    x * 16,
                    y * 16,
                    8,
                    8,
                    SDL_Color{0, 255, 0}
                );
            }
        }
    }
}

void TilemapRenderSystem::run(SDL_Renderer* renderer) {
    auto& tilemap = scene->world->get<TilemapComponent>();
    auto view = scene->r.view<TileComponent, TransformComponent>();

    int size = 16;

    for (auto e : view) {
        const auto transform = view.get<TransformComponent>(e);
        const auto tile = view.get<TileComponent>(e);
        const auto x = transform.x;
        const auto y = transform.y;
        const auto index = tile.index;
        const int xIndex = index % 7;
        const int yIndex = index / 7;
        
        SDL_Rect clip = {
            xIndex * 16,
            yIndex * 16,
            16,
            16
        };

        tile.texture->render(
            x,
            y,
            16,
            16,
            &clip
        );
    }
}

BackgroundSetupSystem::BackgroundSetupSystem(SDL_Renderer* renderer)
    : renderer(renderer) { }

BackgroundSetupSystem::~BackgroundSetupSystem() {
    auto view = scene->r.view<BackgroundComponent>();

    for(auto entity : view) {
        const auto backgroundComponent = view.get<BackgroundComponent>(entity);
        TextureManager::UnloadTexture(backgroundComponent.name);
    }
}

void BackgroundSetupSystem::run() {
    auto view = scene->r.view<BackgroundComponent>();

    for(auto entity : view) {
        const auto backgroundComponent = view.get<BackgroundComponent>(entity);
        TextureManager::LoadTexture(backgroundComponent.name, renderer);
    }
}

void BackgroundRenderSystem::run(SDL_Renderer* renderer) {
    auto view = scene->r.view<TransformComponent, BackgroundComponent>();

    for(auto entity : view) {
        const auto backgroundComponent = view.get<BackgroundComponent>(entity);
        const auto transformComponent = view.get<TransformComponent>(entity);
  
        Texture* texture = TextureManager::GetTexture(backgroundComponent.name);
  
        SDL_Rect clip = {
            backgroundComponent.xIndex * backgroundComponent.width,
            1,
            backgroundComponent.width,
            backgroundComponent.height
        };

        texture->render(
            0, 0, 1024, 576, &clip
        );
    }
}

void BackgroundUpdateSystem::run(double dT) {
    auto view = scene->r.view<BackgroundComponent>();

    Uint32 now = SDL_GetTicks();

    for(auto entity : view) {
        auto& backgroundComponent = view.get<BackgroundComponent>(entity);

        if (backgroundComponent.animationFrames > 0) {
            float timeSinceLastUpdate = now - backgroundComponent.lastUpdate;

            int framesToUpdate = static_cast<int>(
                timeSinceLastUpdate / 
                backgroundComponent.animationDuration * backgroundComponent.animationFrames
            );

            if (framesToUpdate > 0) {
                backgroundComponent.xIndex += framesToUpdate;
                backgroundComponent.xIndex %= backgroundComponent.animationFrames;
                backgroundComponent.lastUpdate = now;            
            }
        }
    }
}

PhysicsSetupSystem::PhysicsSetupSystem(SDL_Renderer* renderer)
    : renderer(renderer) { }

PhysicsSetupSystem::~PhysicsSetupSystem() {

}

void PhysicsSetupSystem::run() {
    if (scene->world == nullptr) {
      scene->world = new Entity(scene->r.create(), scene);
    }
    b2Vec2 gravity(0.0f, 156.8f * PIXELS_PER_METER);  // 100 p/m    9.8 m/s * 10 p/m = 98 p/s;
    scene->world->addComponent<PhysicsComponent>(new b2World(gravity));
};

void PhysicsUpdateSystem::run(double dT) {
    const int velocityIterations = 6;
    const int positionIterations = 2;

    auto world = scene->world->get<PhysicsComponent>().b2d;
    world->Step(dT, velocityIterations, positionIterations);
};

void MovementUpdateSystem::run(double dT) {
    const auto view = scene->r.view<RigidBodyComponent, TransformComponent, NameComponent>();

    for (const auto e : view) {
        const auto rb = view.get<RigidBodyComponent>(e);
        auto& transform = view.get<TransformComponent>(e);
        auto& name = view.get<NameComponent>(e);
      
        if (name.tag == "PLAYER") {
            b2Vec2 position = rb.body->GetPosition(); // x, y meters

            // Convert the Box2D position (center of the body) to screen coordinates
            float centerX = position.x * SCALE / (PIXELS_PER_METER * 2) + 8;
            float centerY = position.y * SCALE / (PIXELS_PER_METER * 2) + 8;
            // 112, 304 -> 56, 152

            // Adjust for the entity's dimensions to get the top-left corner
            transform.x = (centerX - static_cast<float>(transform.w)/2.0f);
            transform.y = (centerY - static_cast<float>(transform.h)/2.0f);
        }
    }
};

void FixtureRenderSystem::run(SDL_Renderer* renderer) {
    auto world = scene->world->get<PhysicsComponent>().b2d;

    for (b2Body* body = world->GetBodyList(); body != nullptr ; body = body->GetNext()) {
      for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Shape* shape = fixture->GetShape();

        if (fixture->GetType() == b2Shape::e_polygon)
        {
          b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();

          // Ensure the rectangle has 4 vertices
          int vertexCount = polygonShape->m_count;
          if(vertexCount == 4)
          {
            // Considering vertices[1] and vertices[3] are opposite corners of the rectangle.
            b2Vec2 vertex1 = polygonShape->m_vertices[1];
            b2Vec2 vertex3 = polygonShape->m_vertices[3];

            // Calculating the width and height in meters
            float width = abs(vertex1.x - vertex3.x);
            float height = abs(vertex1.y - vertex3.y);

            // Getting the position of the body (center of mass) in meters
            b2Body* body = fixture->GetBody();
            b2Vec2 position = body->GetPosition();

            // Convert the values into pixel coordinates
            int PPM = PIXELS_PER_METER; // Pixels Per Meter scale
            float x_px = position.x / PPM;   // m  / (m / p)
            float y_px = position.y / PPM;
            float width_px = width / PPM;
            float height_px = height / PPM;

            // Create an SDL_Rect and initialize it
            SDL_Rect renderRect;
            renderRect.x = x_px - width_px/2.0f;  // Adjust for center of mass
            renderRect.y = y_px - height_px/2.0f; // Adjust for center of mass
            renderRect.w = width_px;
            renderRect.h = height_px;

            renderRect.x *= SCALE;
            renderRect.y *= SCALE;
            renderRect.w *= SCALE;
            renderRect.h *= SCALE;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &renderRect);
          }
        }
      }
    }
};

void MovementInputSystem::run(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
      // move
      if (event.key.keysym.sym == SDLK_a) {
        moveCharacter(-1);
      } else if (event.key.keysym.sym == SDLK_d) {
        moveCharacter(1);
      } else if (event.key.keysym.sym == SDLK_SPACE) {
        jumpCharacter();
      }
    } else if (event.type == SDL_KEYUP) {
      // stop movement
      if (event.key.keysym.sym == SDLK_a) {
        stopCharacter(-1);
      } else if (event.key.keysym.sym == SDLK_d) {
        stopCharacter(1);
      }
    }
}

void MovementInputSystem::moveCharacter(int direction) {
    const auto playerBody = scene->player->get<RigidBodyComponent>().body;
    playerBody->ApplyForceToCenter(b2Vec2(hForceMagnitude * direction, 0), true);

    auto& playerSprite = scene->player->get<SpriteComponent>();
    if (direction == -1) {
      playerSprite.yIndex = 3;
    } else {
      playerSprite.yIndex = 2;
    }
}

void MovementInputSystem::stopCharacter(int direction) {
    const auto playerBody = scene->player->get<RigidBodyComponent>().body;
    playerBody->SetLinearVelocity(b2Vec2(0, 0));

    auto& playerSprite = scene->player->get<SpriteComponent>();
    if (direction == -1) {
      playerSprite.yIndex = 1;
    } else {
      playerSprite.yIndex = 0;
    }
}

void MovementInputSystem::jumpCharacter() {
    const auto playerBody = scene->player->get<RigidBodyComponent>().body;
    playerBody->ApplyLinearImpulseToCenter(b2Vec2(0, -vForceMagnitude), true);
}

void CharacterSetupSystem::run() {
    scene->player = new Entity(scene->r.create(), scene);
    scene->player->addComponent<NameComponent>("PLAYER");

    auto transform = scene->player->addComponent<TransformComponent>(3*16 * SCALE, 9*16 * SCALE, 16 * SCALE, 16 * SCALE);
    scene->player->addComponent<SpriteComponent>(
        "Sprites/MainChar/SpriteSheet.png",
        16, 16,
        0, 0,
        5, 1000
    );

    auto world = scene->world->get<PhysicsComponent>().b2d;

    float x = 48 * PIXELS_PER_METER; 
    float y = 144 * PIXELS_PER_METER; 
    float hx = (16.0f * PIXELS_PER_METER) / 2.0f;
    float hy = (16.0f * PIXELS_PER_METER) / 2.0f;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x + hx, y + hy);

    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(hx, hy);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 0.0000001f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;

    body->CreateFixture(&fixtureDef);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(scene->player);

    scene->player->addComponent<RigidBodyComponent>(
        bodyDef.type,
        body,
        transform.x,
        transform.y,
        transform.w,
        transform.h,
        SDL_Color{0, 255, 0}
    );
}
