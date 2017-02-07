/*
* Created by Chris Lanham.
*/
#include "Triangle2D.h"

Triangle2D::Triangle2D(){
	this->shader = NULL;
	point1 = glm::vec3(0.0f);
	point2 = glm::vec3(0.0f);
	point3 = glm::vec3(0.0f);
	colour = glm::vec4(0.0f);
	textureID = 0;
	triangleVertexArrayObject= 0;
	triangleVertexBuffer = 0;
	triangleColourBuffer = 0;
	triangleTextureBuffer= 0;
}
Triangle2D::Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3){
	this->shader = shader;
	this->point1 = point1;
	this->point2 = point2;
	this->point3 = point3;
	createBuffers();
}
Triangle2D::Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec4 colour){
	this->shader = shader;
	this->point1 = point1;
	this->point2 = point2;
	this->point3 = point3;
	this->colour = colour;
	createBuffers();
}
Triangle2D::Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, GLuint textureID){
	this->shader = shader;
	this->point1 = point1;
	this->point2 = point2;
	this->point3 = point3;
	this->textureID = textureID;
	createBuffers();
}
Triangle2D::Triangle2D(Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec4 colour, GLuint textureID){
	this->shader = shader;
	this->point1 = point1;
	this->point2 = point2;
	this->point3 = point3;
	this->colour = colour;
	this->textureID = textureID;
	createBuffers();
}
Triangle2D::~Triangle2D(){
}

GLvoid Triangle2D::createBuffers(){
	GLfloat vertArray[] = {
		point1.x, point1.y, point1.z,
		point2.x, point2.y, point2.z,
		point3.x, point3.y, point3.z,
	};

	glGenVertexArrays(1, &triangleVertexArrayObject);	//Generate a VAO
	glBindVertexArray(triangleVertexArrayObject);		//Bind this VAO to the current state.

	glGenBuffers(1, &triangleVertexBuffer);
	glBindBuffer(
		GL_ARRAY_BUFFER,
		triangleVertexBuffer
	);
	glBufferData(
		GL_ARRAY_BUFFER,	//Buffer type
		sizeof(vertArray),	//Size of the buffer
		vertArray,			//Data for the buffer to hold
		GL_STATIC_DRAW		//What to do with the buffer
	);
	glVertexAttribPointer(
		VERTEX_ATTRIBUTE,	//Index of shader layout location 
		3,					//Size of the vertices
		GL_FLOAT,			//Type of the vertices
		GL_FALSE,			//Normalize the verts?
		0,					//Spaces between verts
		0					//Spaces to first vert
	);
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE);

	if (colour != glm::vec4(0.0f)){
		GLfloat colourArray[] = {
			colour.x, colour.y, colour.z, colour.w,
			colour.x, colour.y, colour.z, colour.w,
			colour.x, colour.y, colour.z, colour.w
		};

		glGenBuffers(1, &triangleColourBuffer);
		glBindBuffer(
			GL_ARRAY_BUFFER,
			triangleColourBuffer
		);
		glBufferData(
			GL_ARRAY_BUFFER,		//Buffer type
			sizeof(colourArray),	//Size of the buffer
			colourArray,			//Data for the buffer to hold
			GL_STATIC_DRAW			//What to do with the buffer
		);
		glVertexAttribPointer(
			COLOUR_ATTRIBUTE,	//Index of shader layout location 
			4,					//Size of the vertices
			GL_FLOAT,			//Type of the vertices
			GL_FALSE,			//Normalize the verts?
			0,					//Spaces between verts
			0					//Spaces to first vert
		);
		glEnableVertexAttribArray(COLOUR_ATTRIBUTE);
	}
	
	if (textureID != 0){
		GLfloat texArray[] = {
			0.5f, 0.5f,
			0.0f, 0.0f,
			1.0f, 0.0f
		};

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glGenBuffers(1, &triangleTextureBuffer);
		glBindBuffer(
			GL_ARRAY_BUFFER,
			triangleTextureBuffer
		);
		glBufferData(
			GL_ARRAY_BUFFER,	//Buffer type
			sizeof(texArray),	//Size of the buffer
			texArray,			//Data for the buffer to hold
			GL_STATIC_DRAW		//What to do with the buffer
		);
		glVertexAttribPointer(
			TEX_UV_ATTRIBUTE,	//Index of shader layout location 
			2,					//Size of the vertices
			GL_FLOAT,			//Type of the vertices
			GL_FALSE,			//Normalize the verts?
			0,					//Spaces between verts
			0					//Spaces to first vert
		);
		glEnableVertexAttribArray(TEX_UV_ATTRIBUTE);
	}

	// unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
GLvoid Triangle2D::renderTriangle(){
	useShader(this->shader);
	glBindVertexArray(this->triangleVertexArrayObject);
	if (textureID != 0)
		glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
GLvoid deleteTriangle(){

}

////////////////////////////////////////////////////////////////////////////////