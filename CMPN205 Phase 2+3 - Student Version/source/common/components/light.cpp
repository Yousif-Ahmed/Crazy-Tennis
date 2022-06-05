#include "light.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"
#include <iostream>

namespace our{
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        diffuse = data.value("diffuse", glm::vec3(0.0f));
        specular = data.value("specular", glm::vec3(0.0f));
        position = data.value("position", glm::vec3(0.0f));
        direction = data.value("direction", glm::vec3(0.0f));

        std::string type = data.value("lightType", "DIRECTIONAL");
        if (type == "DIRECTIONAL"){
            lightType = 0;
        }else if (type == "POINT"){
            lightType = 1;
            attenuation = data.value("attenuation", glm::vec3(0.0f));
        }else if (type == "SPOT"){
            lightType = 2;
            attenuation = data.value("attenuation", glm::vec3(0.0f));
            cone_angle = data.value("cone_angle", glm::vec2(0.0f));
        }
    }
}