#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp" /// for player
#include "../components/player-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{
    // responsible for moving players which have PlayerControllerComponent by the velocity

    class PlayerControllerSystem {
        Application* app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            std::cout << "i am in player" ;

            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            
            PlayerControllerComponent *controller = nullptr;
            Entity * entity = nullptr ;
            for(auto Currententity : world->getEntities()){
                std::cout << "i am in player" ;
                controller = Currententity->getComponent<PlayerControllerComponent>();
                if(controller){ 
                    entity = Currententity;
                    break;
                }
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if(!(entity && controller)) return;
            std::cout << "i am in hereee" ;

            // getting the assigned velocity from the controller
            auto velocity = controller->linearVelocity;
            
            // getting the position of the player
            // auto &position = entity->localTransform.position;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if(app->getKeyboard().isPressed(GLFW_KEY_UP)) entity->localTransform.position.z +=(deltaTime * velocity.z);
            if(app->getKeyboard().isPressed(GLFW_KEY_DOWN)) entity->localTransform.position.z -=(deltaTime * velocity.z);
            
            // A & D moves the player left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_LEFT)) entity->localTransform.position.x += (deltaTime * velocity.x);
            if(app->getKeyboard().isPressed(GLFW_KEY_RIGHT)) entity->localTransform.position.x -=  (deltaTime * velocity.x);
        }


    };

}
