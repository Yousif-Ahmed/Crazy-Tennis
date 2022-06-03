#pragma once

#include "../ecs/component.hpp"

namespace our{
    enum class LightType{
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component{
    public:
        LightType lightType;
        
        glm::vec3 diffuse = glm::vec3(0.0f);
        glm::vec3 specular = glm::vec3(0.0f);
        glm::vec3 attenuation = glm::vec3(0.0f);

        glm::vec2 cone_angle = glm::vec2(0.0f);

        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f);

        static std::string getID(){return "Light";}

        void deserialize(const nlohmann::json& data) override;
    };
}