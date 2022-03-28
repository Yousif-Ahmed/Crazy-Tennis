#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

void our::ShaderProgram::create()
{
    // Create Shader Program
    program = glCreateProgram();
}

void our::ShaderProgram::destroy()
{
    // Delete Shader Program
    if (program != 0)
        glDeleteProgram(program);
    program = 0;
}

// Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const
{
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCStr = sourceString.c_str();
    file.close();
    // creating an emty shader with type 
    //and returns a non-zero value by which it can be refrenced ShaderID
    // parameter shaderTypes : Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
    // GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
    GLuint shaderID = glCreateShader(type);

    // DONE: send the source code to the shader and compile it
    // this function to get the shader source code 
    // 1st parameter : Shader ID which created used gl.
    // 2nd parameter : How many source code strings for the shader.
    // 3rd parameter : Sending a pointer to source code string. 
    // 4th parameter : Size of the String of the source code but Source code string has nullptr at the end can get its end using it.
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    // compile the source code strings of that have been stored in the shader object
    // paramter : shaderID  
    glCompileShader(shaderID);

    // Here we check for compilation errors
    // DONE: Uncomment this if block
    if (std::string error = checkForShaderCompilationErrors(shaderID); error.size() != 0)
    {
        std::cerr << "ERROR IN " << filename << std::endl;
        std::cerr << error << std::endl;
        glDeleteShader(shaderID);
        return false;
    }

    // DONE: attach the shader to the program then delete the shader
    // this function to attach the shader object to a program object 
    // 1st parameter : program object 
    // 2st parameter : shader object 
    glAttachShader(program , shaderID);
    // After attaching the shader object with the program object 
    // There is no need for the shader object and we can free the memory form the shader object
    glDeleteShader(shaderID);
    // We return true since the compilation succeeded
    return true;
}

bool our::ShaderProgram::link() const
{
    // DONE: call opengl to link the program identified by this->program
    // this function is to link the program object 
    glLinkProgram(program);
    // Here we check for linking errors
    // DONE: Uncomment this if block
    if(auto error = checkForLinkingErrors(program); error.size() != 0){
        std::cerr << "LINKING ERROR" << std::endl;
        std::cerr << error << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader)
{
    // Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program)
{
    // Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}