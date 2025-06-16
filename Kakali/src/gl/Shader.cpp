#include "Shader.h"

void Shader::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader;
    GLCall(vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertexShader, 1, &vShaderCode, NULL));
    GLCall(glCompileShader(vertexShader));

    int  success;
    char infoLog[512];
    GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        GLCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vShaderCode << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    GLCall(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragmentShader, 1, &fShaderCode, NULL));
    GLCall(glCompileShader(fragmentShader));

    GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        GLCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fShaderCode << infoLog << std::endl;
    }

    /* Create and link program */
    GLCall(m_rendererID = glCreateProgram());

    GLCall(glAttachShader(m_rendererID, vertexShader));
    GLCall(glAttachShader(m_rendererID, fragmentShader));
    GLCall(glLinkProgram(m_rendererID));

    GLCall(glGetProgramiv(m_rendererID, GL_LINK_STATUS, &success));
    if (!success) {
        GLCall(glGetProgramInfoLog(m_rendererID, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
}

void Shader::Delete()
{
    GLCall(glDeleteProgram(m_rendererID));
}

void Shader::Use()
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::SetBool(const std::string& name, bool value) const
{
    GLCall(glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), (int)value));
}

void Shader::SetInt(const std::string& name, int value) const
{
    GLCall(glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), value));
}

void Shader::SetFloat(const std::string& name, float value) const
{
    GLCall(glUniform1f(glGetUniformLocation(m_rendererID, name.c_str()), value));
}

void Shader::SetFloat3(const std::string& name, float v0, float v1, float v2) const
{
    GLCall(glUniform3f(glGetUniformLocation(m_rendererID, name.c_str()), v0, v1, v2));
}

void Shader::SetIntV(const std::string& name, unsigned int count, int* value)
{
    GLCall(glUniform1iv(glGetUniformLocation(m_rendererID, name.c_str()), count, value));
}

void Shader::SetMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &matrix[0][0]));
}
