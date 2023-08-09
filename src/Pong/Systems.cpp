#include "print.h"
#include "Systems.h"
#include "Components.h"

void RectRenderSystem::run(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

  const auto view = scene->r.view<TransformComponent, SizeComponent>();
  for (const entt::entity e : view) {
    const TransformComponent& t = view.get<TransformComponent>(e);
    const SizeComponent& c = view.get<SizeComponent>(e);
    const int x = t.position.x;
    const int y = t.position.y;
    const int w = c.w;
    const int h = c.h;

    SDL_Rect rect = { x, y, w, h };    
    SDL_RenderFillRect(renderer, &rect);
  }
}

MovementUpdateSystem::MovementUpdateSystem(int screen_width, int screen_height)
  : screen_width(screen_width), screen_height(screen_height) { }

void MovementUpdateSystem::run(double dT) {
  const auto view = scene->r.view<TransformComponent, SpeedComponent, NameComponent>();
  for (const entt::entity e : view) {
    TransformComponent& t = view.get<TransformComponent>(e);
    SpeedComponent& m = view.get<SpeedComponent>(e);
    NameComponent& n = view.get<NameComponent>(e);

    if (m.velocity.x == 0 && m.velocity.y == 0) {
      continue;
    }

    if (t.position.x <= 0)
    {
      // Gol izquierdo
      if (n.name == "ball") {
        print("RIGHT WINS");
        exit(0);
      }
    }
    if (t.position.x >= screen_width - 10)
    {
      // Gol derecho
      if (n.name == "ball") {
        print("LEFT WINS");
        exit(0);
      }
    }
    if (t.position.y <= 0)
    {
      m.velocity.y *= -1.1;
    }
    if (t.position.y > screen_height - 20)
    {
      m.velocity.y *= -1.1;
    }
  
    t.position.x += m.velocity.x * dT;
    t.position.y += m.velocity.y * dT;
  }
}

void PlayerInputEventSystem::run(SDL_Event event) {
  scene->r.view<NameComponent, PlayerComponent, SpeedComponent>().each(
    [&](const auto& entity, NameComponent& name, PlayerComponent& player, SpeedComponent& speed) {
      if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym) {
          case SDLK_DOWN:
            if (name.name == "right") {
              speed.velocity.y = player.moveSpeed;
            }
            break;
          case SDLK_UP:
            if (name.name == "right") {
              speed.velocity.y = -player.moveSpeed;
            }
            break;
          case SDLK_w:
            if (name.name == "left") {
              speed.velocity.y = -player.moveSpeed;
            }
            break;
          case SDLK_s:
            if (name.name == "left") {
              speed.velocity.y = player.moveSpeed;
            }
            break;
        }
      }
      if (event.type == SDL_KEYUP)
      {
        speed.velocity.y = 0;
      }
    }
  );
}


void CollisionDetectionUpdateSystem::run(double dT) {
    const auto view = scene->r.view<TransformComponent, SizeComponent, ColliderComponent>();
    const auto view2 = scene->r.view<TransformComponent, SizeComponent>();

    view.each([&](auto e1, TransformComponent& t1, SizeComponent& s1, ColliderComponent& c1) {
        // Create a bounding box for the first entity
        SDL_Rect box1 = { t1.position.x, t1.position.y, s1.w, s1.h };

        // Check against all other entities
        view2.each([&](auto e2, TransformComponent& t2, SizeComponent& s2) {
            if (e1 == e2) return;  // Skip self

            // Create a bounding box for the second entity
            SDL_Rect box2 = { t2.position.x, t2.position.y, s2.w, s2.h };

            // Check for intersection
            if (SDL_HasIntersection(&box1, &box2)) {
              c1.triggered = true;
            }
        });
    });
}


void BounceUpdateSystem::run(double dT) {
    const auto view = scene->r.view<ColliderComponent, SpeedComponent>();

    view.each([&](auto e, ColliderComponent& c, SpeedComponent& s) {
      if (c.triggered) {
        c.triggered = false;
        s.velocity.x *= -1.1;
      }
    });
}