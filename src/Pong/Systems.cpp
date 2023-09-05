#include "print.h"
#include "Systems.h"
#include "Components.h"

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
            spriteComponent.xIndex * spriteComponent.size,
            spriteComponent.yIndex * spriteComponent.size,
            spriteComponent.size,
            spriteComponent.size
        };

        int scale = 5;

        texture->render(
            transformComponent.position.x * scale,
            transformComponent.position.y * scale,
            48 * scale,
            48 * scale,
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
    tilemap.width = 50;
    tilemap.height = 38;
    tilemap.tileSize = 16;

    std::vector<int> tmap;

    for (int y = 0; y < tilemap.height; y++) {
        for (int x = 0; x < tilemap.width; x++) {
            float factor = y * y - x / x;

            if (factor > 1) {
                tmap.push_back(0);
            } else {
                tmap.push_back(1);
            }
        }
    }

    for (int i = 0; i < tilemap.height * tilemap.width; i++) {
        tilemap.map.push_back((tmap[i] == 0) ? grassTexture : playerTexture);
    }
}

void TilemapRenderSystem::run(SDL_Renderer* renderer) {
    auto& tilemap = scene->world->get<TilemapComponent>();

    for (int y=0; y < tilemap.height; y++) {
        for (int x = 0; x < tilemap.width; x++) {
            Texture* texture = tilemap.map[y * tilemap.width + x];

            int size = tilemap.tileSize * 5;

            texture->render(
                x * size,
                y * size,
                size,
                size
            );
        }
    }
}

