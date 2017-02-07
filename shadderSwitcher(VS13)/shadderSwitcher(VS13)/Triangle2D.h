/*
* Created by Chris Lanham.
*/
#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H

#include "AdditionalLibraries(VS13)\GLEW\Include\glew.h"
#include "AdditionalLibraries(VS13)\GLM\glm.hpp"
#include "AdditionalLibraries(VS13)\GLM\ext.hpp"
#include "AdditionalLibraries(VS13)\shader.h"
using namespace std;

class Triangle2D{
private:
	Shader*		
		shader;
	glm::vec3
		point1,	// top 
		point2,	// bottom left
		point3; // bottom right
	glm::vec4
		colour; // triangle colour
	GLuint
		textureID,
		triangleVertexArrayObject,
		triangleVertexBuffer,
		triangleColourBuffer,
		triangleTextureBuffer;

public:
	Triangle2D();
	Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec4 colour);
	Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, GLuint textureID);
	Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec4 colour, GLuint textureID);
	~Triangle2D();

	GLvoid createBuffers();
	GLvoid renderTriangle();
	GLvoid deleteTriangle();
};

////////////////////////////////////////////////////////////////////////////////
#endif