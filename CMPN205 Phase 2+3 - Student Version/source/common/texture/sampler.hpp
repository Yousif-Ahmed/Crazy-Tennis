#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our {

    // This class defined an OpenGL sampler
    class Sampler {
        // The OpenGL object name of this sampler 
        GLuint name;
    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name" 
        Sampler() {
            //DONE: (Req 5) Complete this function
            // n - specifies the number of smapler object names to  generate 
            // samplers - specifies an array in which the generated smapler object names are stored
        
            glGenSamplers(1, &name);

            
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler() { 
            //DONE: (Req 5) Complete this function
            // n - specifies the number of smapler object names to be deleted 
            // samplers - specifies an array in which the generated smapler object to be deleted 
            glDeleteSamplers(1, &name );
            
         }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const {
            //DONE: (Req 5) Complete this function
            // unit - Specifies the index of the texture unit to which the smapler is bound
            // smapler  - smapler name  
            glBindSampler(textureUnit , name);
            
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit){
            //DONE: (Req 5) Complete this function
            // unit - Specifies the index of the texture unit to which the smapler is bound
            // smapler  - smapler name  (null ) for unbind 
            glBindSampler(textureUnit , 0);

        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const {
            //DONE: (Req 5) Complete this function
            // sampler : specifies the smapler object whose parameter to modify
            // pname : symbolic name of a smapler parameter can be (GL_TEXTURE_WRAP_S ,GL_TEXTURE_MIN_FILTER , ..)
            // param : value of pname 
            glSamplerParameteri(name , parameter , value);
            
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const {
            //DONE: (Req 5) Complete this function
            // sampler : specifies the smapler object whose parameter to modify
            // pname : symbolic name of a smapler parameter can be (GL_TEXTURE_WRAP_S ,GL_TEXTURE_MIN_FILTER , ..)
            // param : value of pname 
            glSamplerParameterf(name , parameter , value );
            
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const {
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json& data);

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

}