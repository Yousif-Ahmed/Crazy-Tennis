#pragma once

#include <glm/glm.hpp>

#include "../ecs/component.hpp"

namespace our {

// This component denotes that the PlayerControllerSystem will move the owning
// entity using user inputs. It will also control the camera field of view
// accroding to the scrolling of the mouse wheel This component is added as a
// slightly complex example for how use the ECS framework to implement logic.
// For more information, see "common/systems/player-controller.hpp"
// For a more simple example of how to use the ECS framework, see "movement.hpp"
class CollisionControllerComponent : public Component {
 public:
  glm::vec3 linearVelocity = { 0,0,10};  // Each frame, the entity should move as follows: position +=
                 // linearVelocity * deltaTime
  int g = 9.8;

  ;  // The ID of this component type is "Free Camera Controller"
  static std::string getID() { return "Collision Controller"; }

  // Reads sensitivities & speedupFactor from the given json object
  void deserialize(const nlohmann::json& data) override;

  void intialize (int direction){
    linearVelocity = {0,0,direction *10};
  }
};

}  // namespace our