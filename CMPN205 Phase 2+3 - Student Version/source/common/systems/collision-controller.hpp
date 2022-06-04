#pragma once

#include "../ecs/world.hpp"
#include "../components/collision-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{
    // responsible for moving players which have PlayerControllerComponent by the velocity

    class CollisionControllerSystem
    {
        Application *app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            
            CollisionControllerComponent *controller = nullptr;
            Entity *collisionEntity = nullptr;
            for (auto Currententity : world->getEntities())
            {
                controller = Currententity->getComponent<CollisionControllerComponent>();
                if (controller)
                {
                    collisionEntity = Currententity;
                    break;
                }
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(collisionEntity && controller))
                return;

            // getting the assigned velocity from the controller
            glm::vec3 velocity = controller->linearVelocity;
            auto collisionPostion = collisionEntity->localTransform.position;
            auto direction = controller->direction;
            collisionEntity->localTransform.position.z -=direction.z* velocity.z *deltaTime ;
            collisionEntity->localTransform.position.y += direction.y *deltaTime ;
            // collision detection
            for (auto Currententity : world->getEntities())
            {
                if (!Currententity->getComponent<CollisionControllerComponent>()&&!Currententity->getComponent<CameraComponent>())
                {
                    auto position = Currententity->localTransform.position;
                    bool collisionX = abs(position.x  - collisionPostion.x) <= 1.0 ;
                    bool collisionZ = abs(position.z  - collisionPostion.z) <= 1.0;
                    bool collisiony = abs(position.y  - collisionPostion.y) <= 1.0;

                    // std::cout <<(abs(position.y  - collisionPostion.y))<< "  y  "<<std::endl;
                    // std::cout <<(abs(position.z  - collisionPostion.z))<< " z "<<std::endl;
                    std::cout <<(abs(position.y  - collisionPostion.y) )<<std::endl ;
                    // collision detection with ground
                    if (collisiony &&  Currententity->name == "court"  )
                    {
                        std::cout << "collision detected with " <<Currententity->name<<std::endl;
                        // if ( collisionPostion.y <= 10)
                        // {
                            controller->direction.y = -1 *controller->direction.y;

                        // }

                        //controller->direction = - direction;
                    }
                }
            }
        }
    };

}
