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
            auto velocity = controller->linearVelocity;
            auto direction = controller->direction;
            auto collisionPostion = collisionEntity->localTransform.position;
            // collision detection
            for (auto Currententity : world->getEntities())
            {
                controller = Currententity->getComponent<CollisionControllerComponent>();
                if (!controller)
                {
                    auto position = Currententity->localTransform.position;

                    bool collisionX = position.x + 1.0 >= collisionPostion.x && collisionPostion.x + 1.0 >= position.x + 1.0;
                    bool collisionZ = position.z + 1.0 >= collisionPostion.z && collisionPostion.z + 1.0 >= position.z + 1.0;
                    auto print=[](glm::vec3 a){ 
                        std::cout<<a.x<<" , "<<a.y<<","<<a.z <<std::endl;
                    };
                    // collision detection with curret entity
                    if (false)
                    {
                        std::cout <<"i am in the collision";

                        // collision detected
                        // controller->direction = -controller->direction;
                        /*
                        float horizontal = Math.cos(Math.toRadians(pitch)) * wantedSpeedForward;
                        float vertical = Math.sin(Math.toRadians(pitch)) * wantedSpeedForward;
                        loc.x += Math.cos(Math.toRadians(yaw)) * horizontal;
                        loc.z -= Math.sin(Math.toRadians(yaw)) * horizontal;
                        loc.y += vertical;
                        //if you wanted strafing, or non-forward motion add this
                        loc.x -= Math.sin(Math.toRadians(yaw)) * wantedStrafingSpeed;
                        loc.z += Math.cos(Math.toRadians(yaw)) * wantedStrafingSpeed;
                        float pitch = glm::acos(glm::dot(collisionPostion,position) / sqrt(glm::dot(collisionPostion,collisionPosition)*glm::dot(position,position)));
                        // getting the angle with vertical 
                        float yaw  = glm::sin(pitch)
                        
                        float horizontal =glm::cos(glm::radians(pitch)) *velocity.z;
                        float vertical = glm::sin(glm::radians(pitch)) * velocity.x;
                        */
                        // getting the angle with horizotal 
                        print(collisionEntity->localTransform.position);

                        // collisionEntity->localTransform.position.x +=collisionPostion.x + velocity.x ;
                        collisionEntity->localTransform.position.y -= velocity.y ;
                        // collisionEntity->localTransform.position.z +=collisionPostion.z + velocity.z  ;
                        // if ( collisionEntity->localTransform.position.x  >=5 ||  collisionEntity->localTransform.position.x <-5){
                        //     collisionEntity->localTransform.position.x -=collisionPostion.x + velocity.x ;
                        // }
                        if ( collisionEntity->localTransform.position.y <=0){
                            collisionEntity->localTransform.position.y += 2* velocity.y ;

                        }
                        // if (collisionEntity->localTransform.position.z >=90 || collisionEntity->localTransform.position.y <=-90){
                        //     collisionEntity->localTransform.position.z -=collisionPostion.z + velocity.z ;
                        // }
                    }
                }
            }
        }
    };

}
