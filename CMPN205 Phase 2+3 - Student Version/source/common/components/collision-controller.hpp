#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    // This component denotes that the PlayerControllerSystem will move the owning entity using user inputs.
    // It will also control the camera field of view accroding to the scrolling of the mouse wheel
    // This component is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/player-controller.hpp"
    // For a more simple example of how to use the ECS framework, see "movement.hpp"
    class CollisionControllerComponent : public Component {
    public:
        glm::vec3 linearVelocity = {3, 3, 3}; // Each frame, the entity should move as follows: position += linearVelocity * deltaTime
        glm::vec3 direction = {1 ,-1 ,1};  // direction of the collision object 
        // The ID of this component type is "Free Camera Controller"
        static std::string getID() { return "Collision Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}