#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

        int scale = 2;

        texture->render(
            transformComponent.position.x * scale,
            transformComponent.position.y * scale,
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
            Entity tile = scene->createEntity(
                "TILE",
                x * tilemap.tileSize,
                y * tilemap.tileSize
            );

            tile.addComponent<TileComponent>(grassTexture, tmap[y * tilemap.width + x]);
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
        const auto pos = transform.position;
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
            pos.x,
            pos.y,
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

