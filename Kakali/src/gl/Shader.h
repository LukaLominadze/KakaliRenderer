#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "pch.h"

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat3(const std::string& name, float v0, float v1, float v2) const;
    void setIntV(const std::string& name, unsigned int count, int* value);
    void setMat4f(const std::string& name, const glm::mat4& matrix);
};