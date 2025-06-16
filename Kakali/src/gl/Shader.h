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
    Shader(const char* vertexPath, const char* fragmentPath);

    void Use();

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetFloat3(const std::string& name, float v0, float v1, float v2) const;
    void SetIntV(const std::string& name, unsigned int count, int* value);
    void SetMat4f(const std::string& name, const glm::mat4& matrix);

    inline const uint32_t GetRendererID() const { return m_rendererID; }
private:
    uint32_t m_rendererID;
};