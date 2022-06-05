#pragma once

#include "../ecs/component.hpp"
#include "glm/glm.hpp"

namespace our{

    class LightComponent : public Component{
    public:
        int lightType;
        
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
        glm::vec3 attenuation = glm::vec3(1.0f);

        glm::vec2 cone_angle = glm::vec2(1.0f);

        glm::vec3 position = glm::vec3(1.0f);
        glm::vec3 direction = glm::vec3(1.0f);

        glm::vec3 top = glm::vec3(1.0f);
        glm::vec3 middle = glm::vec3(0.6f);
        glm::vec3 bottom = glm::vec3(0.3f);

        static std::string getID(){return "Light";}

        void deserialize(const nlohmann::json& data) override;
    };
}