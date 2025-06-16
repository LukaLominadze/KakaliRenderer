#pragma once

#include "pch.h"

#include <string>
#include <vector>

#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
    glm::vec4 Colors;
	glm::vec2 TexCoords;
	glm::vec3 Normals;
};

struct TextureData {
	unsigned int id;
	std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureData>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};
