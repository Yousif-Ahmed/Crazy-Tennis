#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle
        GLuint program;

    public:
        void create();
        void destroy();

        ShaderProgram(){ program = 0; }
        ~ShaderProgram(){ destroy(); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            //DONE: call opengl to use the program identified by this->program
            // This function is to installs a program object as a part of current state
            // parameter : program object 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //DONE: call opengl to get the uniform location for the uniform defined by name from this->program
            // This function is to git the location of a uniform variable within a program object
            // 1st parameter : program object 
            // 2nd parameter : uniform variable name as strng  
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // This function is to specify a value of 1float uniform varaiable of the current program
            // 1st parameter : location of the uniform variable 
            // 2nd parameter : value of the uniform variable to be set
            glUniform1f(this->getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // This function is to specify a value of 2float uniform varaiable of the current program
            // 1st parameter : location of the uniform variable 
            // 2nd parameter : 1st value of the uniform variable to be set
            // 3rd parameter : 2st value of the uniform variable to be set
            glUniform2f(this->getUniformLocation(uniform) , value.x , value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // This function is to specify a value of 3float uniform varaiable of the current program
            // 1st parameter : location of the uniform variable 
            // 2nd parameter : 1st value of the uniform variable to be set
            // 3rd parameter : 2st value of the uniform variable to be set
            // 4th parameter : 3st value of the uniform variable to be set

            glUniform3f(this->getUniformLocation(uniform), value.x , value.y , value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // This function is to specify a value of 4float uniform varaiable of the current program
            // 1st parameter : location of the uniform variable 
            // 2nd parameter : 1st value of the uniform variable to be set
            // 3rd parameter : 2st value of the uniform variable to be set
            // 4th parameter : 3st value of the uniform variable to be set
            // 5th parameter : 4st value of the uniform variable to be set

            glUniform4f(this->getUniformLocation(uniform) , value.x , value.y , value.z, value.w);
        }


        //DONE: Delete the copy constructor and assignment operator
        //Question: Why do we do this? Hint: Look at the deconstructor
        // because the default copy constructor is a shallow copy constructor as only represented as pointer to the object 
        // then if we delete the original object this case an exception due to the copy version points to the same place 
        // so if we need to do a copy constructor we need to create new object and get all info of the object to the new object
        // which is computational expensive.
        ShaderProgram(ShaderProgram const &) = delete ;
        ShaderProgram & operator =(ShaderProgram const &) = delete ;
    };

}

#endif