#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our
{
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState
    {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not
        // and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and
        //  how it will be configured
        struct
        {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true;                           // To know how to use it, check glDepthMask

        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const
        {
            // TODO: (Req 3) Write this function

            // 1. Checking for face culling
            if (faceCulling.enabled)
            {
                glEnable(GL_CULL_FACE);
                // Pase the front face and which face to be culled
                glCullFace(faceCulling.culledFace);
                glFrontFace(faceCulling.frontFace);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }

            // 2. Checking for depth testing
            if (depthTesting.enabled)
            {
                glEnable(GL_DEPTH_TEST);
                // Pase the function used in depth testing calculation
                glDepthFunc(depthTesting.function);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }

            // 3. Checking for blending
            if (blending.enabled)
            {
                glEnable(GL_BLEND);
                // Pass the factors to be multiped by for the source and destination
                glBlendFunc(blending.sourceFactor, blending.destinationFactor);
                // Pass the operation for blending ex: ADD
                glBlendEquation(blending.equation);
                // Pass the blend color
                glBlendColor(blending.constantColor[0], blending.constantColor[1], blending.constantColor[2],
                             blending.constantColor[3]);
            }
            else
            {
                glDisable(GL_BLEND);
            }

            // 4. Checking for color mask
            // Enable or disable the writhing over every channel
            glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);

            // 5. Checking for depth mask
            // Enable or disable writing into the depth buffer
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json &data);
    };

}