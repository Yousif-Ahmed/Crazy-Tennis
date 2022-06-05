#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <systems/forward-renderer.hpp>

#include "../application.hpp"
#include "../components/collision-controller.hpp"
#include "../ecs/world.hpp"
namespace our {
// responsible for moving players which have PlayerControllerComponent by the
// velocity

class CollisionControllerSystem {
  Application *app;  // The application in which the state runs

 public:
  // When a state enters, it should call this function and give it the pointer
  // to the application
  void enter(Application *app) { this->app = app; }

  // This should be called every frame to update all entities containing a
  // FreeCameraControllerComponent
  void update(World *world, float deltaTime, ForwardRenderer *renderer) {
    CollisionControllerComponent *controller = nullptr;

    if (app->isCollision == 0 && renderer->getCollisionPostProcessing()) {
      renderer->setCollusionPostProcessing(false);
      renderer->makePostProcessEffect();
    }

    Entity *collisionEntity = nullptr;
    for (auto Currententity : world->getEntities()) {
      controller = Currententity->getComponent<CollisionControllerComponent>();
      if (controller) {
        collisionEntity = Currententity;
        break;
      }
    }
    // If there is no entity with both a CameraComponent and a
    // FreeCameraControllerComponent, we can do nothing so we return
    if (!(collisionEntity && controller)) return;

    // getting the assigned velocity from the controller
    auto &motionVelocity_ref = controller->linearVelocity;

    auto &gravitationalAcc_ref = controller->g;
    auto &collisionEntityPosition_ref =
        collisionEntity->localTransform.position;

    auto horizontal_Z = glm::vec3(0, 0, 1);

    // Change the velocity
    motionVelocity_ref.y += -gravitationalAcc_ref * deltaTime;

    // r_z = r_z0 + v_z*t ---> delta_r_z = v_z* delta_t
    collisionEntityPosition_ref.z += 5 * motionVelocity_ref.z * deltaTime;

    // r_y = r_y0 + t(v_y0 - 0.5*g*t) ---> delta_r_z ~= v_y*delta_t
    collisionEntityPosition_ref.y += motionVelocity_ref.y * deltaTime;

    // collision detection
    for (auto Currententity : world->getEntities()) {
      if (!Currententity->getComponent<CollisionControllerComponent>()) {
        auto position = Currententity->localTransform.position;

        bool collisionX =
            abs(position.x - collisionEntityPosition_ref.x) <= 4.0;
        bool collisionZ =
            abs(position.z - collisionEntityPosition_ref.z) <= 4.0;
        bool collisionY =
            abs(position.y - collisionEntityPosition_ref.y) <= 1.0;

        bool collisionY_grid =
            abs(position.y - collisionEntityPosition_ref.y) <= 2.0;

        // collision detection with ground
        bool groundCollision = collisionY && Currententity->name == "court";

        bool grid_Collision =
            collisionY_grid && collisionZ && (Currententity->name == "grid");
        bool otherEntitiesCollision = collisionX && collisionZ;

        if (groundCollision) {
          motionVelocity_ref.y *= -1;
        } else if (otherEntitiesCollision &&
                   (Currententity->name == "racket1" ||
                    Currententity->name == "racket2")) {
          std::cout << Currententity->name << std::endl;
          motionVelocity_ref.y *= -1;
          motionVelocity_ref.z *= -1;

          renderer->setCollusionPostProcessing(true);
          renderer->makePostProcessEffect();

          app->collision = 1;
        } else if (grid_Collision) {
          std::cout << Currententity->name << std::endl;
          motionVelocity_ref.y *= -1;
          motionVelocity_ref.z *= -1;
        }
        // score handling
        // then player 2 score a goal
        if (collisionEntity->localTransform.position.z > 80) {
          app->player2_score += 1;
          collisionEntity->localTransform.position = glm::vec3(15, 20, -60);
          controller->intialize(1);
        } else if (collisionEntity->localTransform.position.z < -80) {
          app->player1_score += 1;
          collisionEntity->localTransform.position = glm::vec3(15, 20, 60);
          controller->intialize(-1);
        }
      }
    }
  }
};

}  // namespace our
