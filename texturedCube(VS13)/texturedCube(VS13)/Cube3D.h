/*
* Created by Chris Lanham.
*/
#ifndef CUBE3D_H
#define CUBE3D_H

#include "AdditionalLibraries(VS13)\GLEW\Include\glew.h"
#include "AdditionalLibraries(VS13)\GLM\glm.hpp"
#include "AdditionalLibraries(VS13)\GLM\ext.hpp"
#include "AdditionalLibraries(VS13)\shader.h"
using namespace std;

class Cube3D{
private:
	Shader*
		shader;
	ShaderMaterial
		cubeMaterial;
	GLuint
		textureID,
		cubeVAO,
		cubeVertexBuffer,
		cubeColourBuffer,
		cubeTextureBuffer,
		cubeIndexBuffer,
		cubeNormalBuffer,
		cubeModelBuffer;
	GLboolean
		isIndexed;
	glm::mat4 modelMatrix;
public:
	Cube3D();
	Cube3D(Shader* shader);
	Cube3D(Shader* shader, GLuint textureID);
	~Cube3D();

	glm::mat4 getModelMatrix();
	GLvoid setModelMatrix(glm::mat4 modelMatrix);

	GLvoid createInterleavedBuffers();
	GLvoid createSeparateBuffers();
	GLvoid renderCube();
};

////////////////////////////////////////////////////////////////////////////////
#endif