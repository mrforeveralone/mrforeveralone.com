#include "Cube3D.h"

/*
Two different ways of building 3D objects.
First way is cubeVertices and cubeColours which are two separate arrays that are accessed via a third index array that points to the data row in the first two.
Second way is single array that includes all the data needed for all the vertices in draw order.

Neither of these ways is the best it’s just a simple way of highlighting the difference.

The best way would be a combination of the two, as you can see data is repeated in the cubeModel array wasting space.
Having all the data for each vertex (position/texture/colour etc.) all packed into a single array/buffer is very efficient.
Combine this with an index array for best effect.
*/

GLfloat cubeVertices[] = {
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f
};
GLfloat cubeColours[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};
GLuint cubeIndex[] = {
	0, 1, 2,
	2, 3, 0,
	1, 5, 6,
	6, 2, 1,
	7, 6, 5,
	5, 4, 7,
	4, 0, 3,
	3, 7, 4,
	4, 5, 1,
	1, 0, 4,
	3, 2, 6,
	6, 7, 3
};

float cubeModel[] = {
//	Vertex|						Texture|		Colour
//	Vx |	Vy |	Vz |		Tu | Tv |		Cr | Cg | Cb | Ca |
	-1.0f,	-1.0f,	1.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,	-1.0f,	1.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,	1.0f,	1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,	1.0f,	1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	1.0f,	-1.0f,	1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	1.0f,	1.0f,	1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,	1.0f,	1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	1.0f,	1.0f,	1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f,
	1.0f,	1.0f,	1.0f,		1.0f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f,
	1.0f,	1.0f,	-1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,	1.0f,	-1.0f,		1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,
	1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,	-1.0f,	-1.0f,		1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,	-1.0f,	-1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f,
	1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,
	1.0f,	-1.0f,	-1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,	-1.0f,	-1.0f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,	-1.0f,	-1.0f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,	-1.0f,	1.0f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,	-1.0f,	1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,
	1.0f,	-1.0f,	-1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, 1.0f,
	1.0f,	-1.0f,	1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,
	1.0f,	-1.0f,	1.0f,		0.0f, 0.0f,		0.5f, 0.0f, 0.0f, 1.0f,
	1.0f,	-1.0f,	-1.0f,		1.0f, 0.0f,		0.5f, 0.0f, 0.0f, 1.0f,
	1.0f,	1.0f,	1.0f,		0.0f, 1.0f,		0.5f, 0.0f, 0.0f, 1.0f,
	1.0f,	1.0f,	1.0f,		0.0f, 1.0f,		0.0f, 0.5f, 0.0f, 1.0f,
	1.0f,	-1.0f,	-1.0f,		1.0f, 0.0f,		0.0f, 0.5f, 0.0f, 1.0f,
	1.0f,	1.0f,	-1.0f,		1.0f, 1.0f,		0.0f, 0.5f, 0.0f, 1.0f,
	-1.0f,	-1.0f,	-1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.5f, 1.0f,
	-1.0f,	-1.0f,	1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.5f, 1.0f,
	-1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.5f, 1.0f,
	-1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		0.5f, 0.5f, 0.5f, 1.0f,
	-1.0f,	-1.0f,	1.0f,		1.0f, 0.0f,		0.5f, 0.5f, 0.5f, 1.0f,
	-1.0f,	1.0f,	1.0f,		1.0f, 1.0f,		0.5f, 0.5f, 0.5f, 1.0f
};

Cube3D::Cube3D(){
	this->shader = NULL;
	textureID = 0;
	cubeVAO = 0;
	cubeVertexBuffer = 0;
	cubeColourBuffer = 0;
	cubeTextureBuffer = 0;
	cubeIndexBuffer = 0;
	cubeNormalBuffer = 0;
	cubeModelBuffer = 0;
	modelMatrix = glm::mat4(1.0f);
}
Cube3D::Cube3D(Shader* shader){
	this->shader = shader;
	textureID = 0;
	cubeVAO = 0;
	cubeVertexBuffer = 0;
	cubeColourBuffer = 0;
	cubeTextureBuffer = 0;
	cubeIndexBuffer = 0;
	cubeNormalBuffer = 0;
	cubeModelBuffer = 0;
	modelMatrix = glm::mat4(1.0f);
}
Cube3D::Cube3D(Shader* shader, GLuint textureID){
	this->shader = shader;
	textureID = textureID;
	cubeVAO = 0;
	cubeVertexBuffer = 0;
	cubeColourBuffer = 0;
	cubeTextureBuffer = 0;
	cubeIndexBuffer = 0;
	cubeNormalBuffer = 0;
	cubeModelBuffer = 0;
	modelMatrix = glm::mat4(1.0f);
}

Cube3D::~Cube3D(){

}

GLvoid Cube3D::createSeparateBuffers(){
	
	glGenVertexArrays(1, &cubeVAO);	//Generate a VAO
	glBindVertexArray(cubeVAO);		//Bind this VAO to the current state.
		
		glGenBuffers(1, &cubeIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * sizeof(cubeIndex), cubeIndex, GL_STATIC_DRAW);

		glGenBuffers(1, &cubeVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),	cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_ATTRIBUTE,	3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_ATTRIBUTE);

		glGenBuffers(1, &cubeColourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cubeColourBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColours), cubeColours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_ATTRIBUTE,	3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_ATTRIBUTE);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	isIndexed = true;

}
GLvoid Cube3D::createInterleavedBuffers(){
	glGenVertexArrays(1, &cubeVAO);	//Generate a VAO
	glBindVertexArray(cubeVAO);		//Bind this VAO to the current state.

	glGenBuffers(1, &cubeModelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeModelBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,	//Buffer type
		sizeof(cubeModel),	//Size of the buffer
		cubeModel,			//Data for the buffer to hold
		GL_STATIC_DRAW		//What to do with the buffer
		);

	glVertexAttribPointer(VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 9, (void*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE);
	
	glVertexAttribPointer(TEX_UV_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 9, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(TEX_UV_ATTRIBUTE);

	glVertexAttribPointer(COLOUR_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 9, (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(COLOUR_ATTRIBUTE);

	// unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	isIndexed = false;
}

GLvoid Cube3D::renderCube(){
	useShader(this->shader);
	shader->setUniformModel(modelMatrix);
	shader->setUniformMVP();

	glBindVertexArray(cubeVAO);

	if (isIndexed){
		if (textureID != 0)
			glBindTexture(GL_TEXTURE_2D, textureID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBuffer);
		glDrawElements(GL_TRIANGLES, sizeof(cubeIndex), GL_UNSIGNED_INT, 0);
	} 
	else{
		if (textureID != 0)
			glBindTexture(GL_TEXTURE_2D, textureID);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeModel));
	}
	glBindVertexArray(0);
}

glm::mat4 Cube3D::getModelMatrix(){
	return modelMatrix;
}

GLvoid Cube3D::setModelMatrix(glm::mat4 modelMatrix){
	this->modelMatrix = modelMatrix;
}

