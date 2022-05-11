#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // DONE: (Req 2) Write this function

        // Generate rotation matrix from euler angles
        // Note: the order of params (x: pitch, y: yaw, z: roll)
        glm::mat4 rotateMat = glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);

        // Create translate matrix with glm::translate the function takes
        // a transaltion vector(tx, ty, tz) as a second param and generate
        // a translate matrix then applay it to the first param (glm::mat4)
        glm::mat4 t1 = glm::translate(glm::mat4(1.0f), position);
        // Applay rotation
        glm::mat4 t2 = t1 * rotateMat;
        // Create scale matrix with glm::scale the function takes
        // a transaltion vector(sx, sy, sz) as a second param and generate
        // a translate matrix then applay it to the first param (glm::mat4)
        glm::mat4 t3 = glm::scale(t2, scale);

        return t3; // Final order: translate * rotate * scale
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);
    }

}