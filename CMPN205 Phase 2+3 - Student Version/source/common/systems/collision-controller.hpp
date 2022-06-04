#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

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
  void update(World *world, float deltaTime) {
    CollisionControllerComponent *controller = nullptr;

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
    glm::vec3 &motionVelocity_ref = controller->linearVelocity;
    auto &gravitationalAcc_ref = controller->g;
    auto &collisionEntityPosition_ref =
        collisionEntity->localTransform.position;
    auto &initialVelocity_ref = controller->initialVelocity;
    auto direction = controller->direction;
    auto horizontal_Z = glm::vec3(0, 0, 1);
    
    // Change the velocity
    motionVelocity_ref.y += -gravitationalAcc_ref * deltaTime;

    // r_z = r_z0 + v_z*t ---> delta_r_z = v_z* delta_t
    collisionEntityPosition_ref.z += motionVelocity_ref.z * deltaTime;

    // r_y = r_y0 + t(v_y0 - 0.5*g*t) ---> delta_r_z = *delta_t
    collisionEntityPosition_ref.y += initialVelocity_ref.y * deltaTime -
                                     0.5 * gravitationalAcc_ref * deltaTime;

    // collision detection
    for (auto Currententity : world->getEntities()) {
      if (!Currententity->getComponent<CollisionControllerComponent>()) {
        auto position = Currententity->localTransform.position;

        auto theta = glm::dot(horizontal_Z, controller->direction);
        bool collisionX =
            abs(position.x - collisionEntityPosition_ref.x) <= 5.0;
        bool collisionZ =
            abs(position.z - collisionEntityPosition_ref.z) <= 5.0;
        bool collisionY =
            abs(position.y - collisionEntityPosition_ref.y) <= 1.0;

        bool collisionY_grid =
            abs(position.y - collisionEntityPosition_ref.y) <= 5.0;

        
        // collision detection with ground
        bool groundCollision = collisionY && Currententity->name == "court";

        bool ads_grid_Collision =
            collisionY_grid && collisionZ &&
            (Currententity->name == "ads" || Currententity->name == "grid");
        bool otherEntitiesCollision = collisionX && collisionZ;

        if (ads_grid_Collision) {
          std::cout << "collision with " << Currententity->name << "\n";
          gravitationalAcc_ref *= -1;
          initialVelocity_ref.y = 0.1;
          initialVelocity_ref.z = 0;
        }
        if (groundCollision) {
          // On Collision the y component of the velocity is reversed
          gravitationalAcc_ref *= -1;
          if (motionVelocity_ref.y < 10) {
            initialVelocity_ref.y = 0;
            initialVelocity_ref.z = 0;
          } else {
            initialVelocity_ref.y = glm::abs(motionVelocity_ref.y) * 0.1;
            initialVelocity_ref.z = 0.1;
          }
        }

        // else if (otherEntitiesCollision) {  //&& Currententity->name !=
        //                                        //"grid") {
        //    //
        //    std::cout << "collision detected with " << Currententity->name
        //              << std::endl;
        //    g = -g;
        //    controller->initialVelocity.y = -controller->linearVelocity.y;
        //    controller->initialVelocity.z = -controller->linearVelocity.z;
        //  }
        //  if (collisionPosition.y > 20) {
        //    velocity.y = glm::abs(velocity.y);
        //  }
      }
    }
  }
};

}  // namespace our
