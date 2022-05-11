#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //DONE: (Req 4) Complete this function
            // generate texture names 
            // n - specifies the number of texture name to be generated 
            // textures - specifies an array in which the generated texture names are stored
            glGenTextures(1 , &name);
            
            
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //DONE: (Req 4) Complete this function
            // n : represent the nuber of textures yo be deleted 
            // textures : array of textures to be deleted 
            glDeleteTextures(1 , &name );
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //DONE: (Req 4) Complete this function
            // binding a named texture to atecturing target
            // target - specifies the target to which the texture is bound in our case we 
            // used GL_TEXTURE_2D bound 
            // texture - the name of the texture
             glBindTexture(GL_TEXTURE_2D, name);

        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //DONE: (Req 4) Complete this function
            glBindTexture(GL_TEXTURE_2D , 0);

        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}