#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
};

struct Texture 
{
	unsigned int id;
	std::string type;
};	

class Mesh 
{
private:
	unsigned int VAO;
	unsigned int VBO;
	void setupMesh();
public:

	std::vector<Vertex> vertices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
	~Mesh();

	void draw(Shader& shader);
};