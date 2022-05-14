#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

namespace our {

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //TODO: (Req 9) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            //TODO: (Req 10) Create a framebuffer

            //TODO: (Req 10) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits).
            
            //TODO: (Req 10) Unbind the framebuffer just to be safe
            
            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler* postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram* postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world){
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //TODO: (Req 8) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        glm::vec3 cameraForward = glm::vec3(camera->getViewMatrix() * glm::vec4(0, 0, -1, 0));
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
            //TODO: (Req 8) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 


            // calculate first distance
            glm::vec3 firstDistance = cameraForward * glm::dot(cameraForward, first.center );
            // calculate second distance
            glm::vec3 secondDistance = cameraForward * glm::dot(cameraForward, second.center );
            // return true if first distance is less than second distance
            return firstDistance.z < secondDistance.z;

        });

        //TODO: (Req 8) Get the camera ViewProjection matrix and store it in VP
        //  Get the camera ViewProjection matrix and store it in VP
        glm::mat4 VP = camera->getViewMatrix() * camera->getProjectionMatrix(windowSize);
        
        //TODO: (Req 8) Set the OpenGL viewport using windowSize
        //  Set the OpenGL viewport using windowSize
        glViewport(0, 0, windowSize.x, windowSize.y);

        //TODO: (Req 8) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        //TODO: (Req 8) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);


        

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //TODO: (Req 10) bind the framebuffer

        }

        //TODO: (Req 8) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        //TODO: (Req 8) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command

        //  Draw all the opaque commands
        for(auto command : opaqueCommands){
            // Set the transform uniform
            command.material->shader->set("transform", VP * command.localToWorld);
            // Draw the mesh
            command.mesh->draw();


        }



        

        //glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix() * entity->getLocalToWorldMatrix();
        
        //glm::mat4 MV = camera->getViewMatrix() * entity->getLocalToWorldMatrix();
        
        //glm::mat4 M = entity->getLocalToWorldMatrix();




        
        // If there is a sky material, draw the sky
        if(this->skyMaterial){
            //TODO: (Req 9) setup the sky material
            //  setup the sky material
            skyMaterial->shader->set("transform", VP);

            

            // Setup the sky material
            /*
            skyMaterial->shader->use();
            skyMaterial->shader->setUniform("MVP", MVP);
            skyMaterial->shader->setUniform("MV", MV);
            skyMaterial->shader->setUniform("M", M);
            skyMaterial->shader->setUniform("cameraPosition", camera->getOwner()->getPosition());
            skyMaterial->shader->setUniform("cameraForward", cameraForward);
            skyMaterial->shader->setUniform("time", time);
            skyMaterial->shader->setUniform("sunDirection", sunDirection);
            skyMaterial->shader->setUniform("sunColor", sunColor);
            skyMaterial->shader->setUniform("skyColor", skyColor);
            skyMaterial->shader->setUniform("skyIntensity", skyIntensity);
            skyMaterial->shader->setUniform("skyTint", skyTint);
            */




            
            //TODO: (Req 9) Get the camera position
            // Calculate the camera position
            glm::vec3 cameraPosition = glm::vec3(camera->getViewMatrix() * glm::vec4(0, 0, 0, 1));
            


            //TODO: (Req 9) Create a model matrix for the sy such that it always follows the camera (sky sphere center = camera position)
            glm::mat4 skyModelMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);


            //TODO: (Req 9) We want the sky to be drawn behind everything (in NDC space, z=1)

            skyModelMatrix = glm::scale(skyModelMatrix, glm::vec3(1.0f, 1.0f, -1.0f));

            
            //glm::vec4 NDC = MVP * glm::vec4(position, 1.0f);
            //NDC.z = 1.0f;

            // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            glm::mat4 alwaysBehindTransform = glm::mat4(
            //  Row1, Row2, Row3, Row4
                1.0f, 0.0f, 0.0f, 0.0f, // Column1
                0.0f, 1.0f, 0.0f, 0.0f, // Column2
                0.0f, 0.0f, 1.0f, 0.0f, // Column3
                0.0f, 0.0f, 0.0f, 1.0f  // Column4
            );
            //TODO: (Req 9) set the "transform" uniform
            //  set the "transform" uniform
            skyMaterial->shader->set("transform", VP * alwaysBehindTransform * skyModelMatrix);


            /*
            // Set transform to be uniform
            glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(MVP * alwaysBehindTransform));

            
            //TODO: (Req 9) draw the sky sphere

            // Draw the sky sphere
            skyMaterial->shader->use();
            skyMaterial->sampler->bind();
            skyMaterial->shader->setUniform("u_texture", 0);
            skyMaterial->shader->setUniform("u_model", skyModelMatrix);
            skyMaterial->shader->setUniform("u_view", camera->getViewMatrix());
            skyMaterial->shader->setUniform("u_projection", camera->getProjectionMatrix());
            skyMaterial->shader->setUniform("u_cameraPosition", cameraPosition);
            skyMaterial->shader->setUniform("u_time", (float)glfwGetTime());
            skyMaterial->shader->setUniform("u_sunDirection", glm::vec3(0.0, 0.0, 1.0));
            skyMaterial->shader->setUniform("u_sunColor", glm::vec3(1.0, 1.0, 1.0));
            skyMaterial->shader->setUniform("u_skyColor", glm::vec3(0.5, 0.5, 0.5));
            skyMaterial->shader->setUniform("u_fogColor", glm::vec3(0.5, 0.5, 0.5));
            skyMaterial->shader->setUniform("u_fogDensity", 0.01f);
            skyMaterial->shader->setUniform("u_fogStart", 0.0f);
            skyMaterial->shader->setUniform("u_fogEnd", 100.0f);
            skyMaterial->shader->setUniform("u_fogEnabled", true);
            skyMaterial->shader->setUniform("u_gamma", 2.2f);
            skyMaterial->shader->setUniform("u_exposure", 1.0f);
            skyMaterial->shader->setUniform("u_toneMapping", 0);
            skyMaterial->shader->setUniform("u_bloomEnabled", false);
            skyMaterial->shader->setUniform("u_bloomThreshold", 0.0f);
            skyMaterial->shader->setUniform("u_bloomIntensity", 0.0f);
            skyMaterial->shader->setUniform("u_bloomKernelSize", 0);
            skyMaterial->shader->setUniform("u_bloomBlurSigma", 0.0f);
            */

            
        }
        //TODO: (Req 8) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        //  draw all the transparent commands
        for(auto command : transparentCommands){
            
            command.material->shader->set("transform", VP);
            // Draw the command mesh
            command.material->shader->use();
            command.mesh->draw();
            
        }

        


        

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //TODO: (Req 10) Return to the default framebuffer
            
            //TODO: (Req 10) Setup the postprocess material and draw the fullscreen triangle
            
        }
    }

}