#include "collision-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads sensitivities & speedupFactor from the given json object
    void CollisionControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        linearVelocity = data.value("linearVelocity", linearVelocity);
        
    }
}