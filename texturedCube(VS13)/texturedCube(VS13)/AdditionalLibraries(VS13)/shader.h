/*
* Created by Chris Lanham. Based on shader code from http://www.opengl-tutorial.org/
*/
#ifndef SHADER_H	
#define SHADER_H

#include <vector>
#include <fstream>
#include "GLEW\Include\glew.h"
#include "GLM\glm.hpp"
#include "GLM\ext.hpp"
using namespace std;

// Some helpful statics // 
static enum SHADER_ATTRIBUTE {
	VERTEX_ATTRIBUTE,
	NORMAL_ATTRIBUTE,
	TEX_UV_ATTRIBUTE,
	COLOUR_ATTRIBUTE
};
static const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
static const glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);

// Class definitions //
typedef struct ShaderMaterial{		// defines material in a model
	GLchar*		name;               // name of material
	glm::vec4	diffuse,			// diffuse component
				ambient,			// ambient component
				specular,			// specular component
				emissive;			// emmissive component
	GLfloat		shininess;			// shininess component
	GLuint		materialTexture;	// texture component
} ShaderMaterial;

#define MAX_LIGHTS = 8;

// function declaration 
static GLuint loadShaders(const GLchar* vertexShader, const GLchar* fragmentShader);
static string shaderToString(const GLchar* shader);

class Shader{
private:
	// OpenGL shader
	GLuint  programID;

	// Uniform locations
	GLuint
		MVP_MatrixUniform,
		modelMatrixUniform,
		viewMatrixUniform,
		projMatrixUniform;

	GLuint  
		materialDiffuseUniform,
		materialAmbientUniform,
		materialEmissiveUniform,
		materialSpecularUniform,
		materialShininessUniform;

	GLuint*  
		lightPosUniforms;
	GLuint	 
		lightPosUniform;

	glm::mat4	
		MVP_matrix,
		projMatrix,
		viewMatrix,
		modelMatrix;

public:
	Shader(){
		programID = 0;

		MVP_MatrixUniform = 0;
		modelMatrixUniform	= 0;
		viewMatrixUniform	= 0;
		projMatrixUniform	= 0;

		materialDiffuseUniform	= 0;
		materialAmbientUniform	= 0;
		materialEmissiveUniform = 0;
		materialSpecularUniform = 0;
		materialShininessUniform = 0;

		lightPosUniform = 0;

		MVP_matrix = glm::mat4(1.0f);
		projMatrix = glm::mat4(1.0f);
		viewMatrix = glm::mat4(1.0f);
		modelMatrix = glm::mat4(1.0f);
	};
	~Shader(){

	};

	GLboolean createShader(const GLchar* vertexShader, const GLchar* fragmentShader){
		// Create and compile our GLSL program from the shaders
		programID = loadShaders(vertexShader, fragmentShader);

		if (programID != 0) {
			// Get a handle for our uniforms
			MVP_MatrixUniform = glGetUniformLocation(programID, "MVP");
			projMatrixUniform = glGetUniformLocation(programID, "projectionMatrix");
			viewMatrixUniform  = glGetUniformLocation(programID, "viewMatrix");
			modelMatrixUniform = glGetUniformLocation(programID, "modelMatrix");

			materialDiffuseUniform   = glGetUniformLocation(programID,
				"MaterialDiffuse");
			materialEmissiveUniform  = glGetUniformLocation(programID,
				"MaterialEmissive");
			materialSpecularUniform  = glGetUniformLocation(programID,
				"MaterialSpecular");
			materialShininessUniform = glGetUniformLocation(programID,
				"MaterialShininess");

			lightPosUniform = glGetUniformLocation(programID, "lightPos");

			return true;
		}
		else
			return false;
	};

	GLboolean deleteShader(){
		GLenum ErrorCheckValue = glGetError();

		// Make sure we're no longer using the program
		glUseProgram(0);

		glDeleteProgram(programID);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR){
			fprintf(stderr, "Shader Error: Could not delete shaders\n");
			return false;
		}
		else
			printf("shaders deleted \n");

		return true;
	};

	// Matrix //
	GLvoid setUniformMVP(){
		MVP_matrix = projMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP_MatrixUniform, 1, GL_FALSE, glm::value_ptr(MVP_matrix));
	};
	GLvoid setUniformModel(glm::mat4 modelMatrix){
		this->modelMatrix = modelMatrix;
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	};
	GLvoid setUniformView(glm::mat4 veiwMatrix){
		this->viewMatrix = veiwMatrix;
		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(veiwMatrix));
	};
	GLvoid setUniformPerspective(glm::mat4 perspectiveMatrix){
		this->projMatrix = perspectiveMatrix;
		glUniformMatrix4fv(projMatrixUniform, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
	};

	// Material //
	GLvoid setMaterial(ShaderMaterial* material){
		glUniform4fv(materialAmbientUniform,   1, glm::value_ptr(material->ambient));
		glUniform4fv(materialDiffuseUniform,   1, glm::value_ptr(material->diffuse));
		glUniform4fv(materialEmissiveUniform,  1, glm::value_ptr(material->emissive));
		glUniform4fv(materialSpecularUniform,  1, glm::value_ptr(material->specular));
		glUniform1fv(materialShininessUniform, 1, &material->shininess);
	};

	// Lights //
	GLvoid setUniformLight0(glm::vec4* lightPosition){
		glUniform4fv(lightPosUniform, 1, glm::value_ptr(lightPosition[0]));
	};
	GLvoid setUniformLight(GLuint lightNumber, glm::vec4* lightPosition){
		MVP_matrix = projMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP_MatrixUniform, 1, GL_FALSE, glm::value_ptr(MVP_matrix));
	};

	GLuint getProgramID(){
		return programID;
	};

	GLuint getMVP_MatrixUniform(){
		return MVP_MatrixUniform;
	};
	GLuint getProjMatrixUniform(){
		return projMatrixUniform;
	};
	GLuint getViewMatrixUniform(){
		return viewMatrixUniform;
	};
	GLuint getModelMatrixUniform(){
		return modelMatrixUniform;
	};

	GLuint getDiffuseUniform(){
		return materialDiffuseUniform;
	};
	GLuint getEmissiveUniform(){
		return materialAmbientUniform;
	};
	GLuint getSpecularUniform(){
		return materialSpecularUniform;
	};
	GLuint getShininessUniform(){
		return materialShininessUniform;
	};

	GLuint getLightPosUniform(){
		return lightPosUniform;
	};
};

static GLuint loadShaders(const GLchar* vertexShader, const GLchar* fragmentShader){
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// variables for containing shader code
	string vertexShaderCode;
	string fragmentShaderCode;
	const char * vertexSourcePointer;
	const char * fragmentSourcePointer;

	// Error checking code
	GLint Result = GL_FALSE;
	int InfoLogLength = 0;

	// Load shaders
	vertexShaderCode = shaderToString(vertexShader);
	fragmentShaderCode = shaderToString(fragmentShader);

	// Compile Vertex Shader
	vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

static string shaderToString(const GLchar* shader){
	string shaderCode;
	// If shader char doesn't start with "#version" assume file location
	if (shader[0] != '#' && shader[7] != 'n'){
		// Read the Vertex Shader code from the file
		ifstream shaderStream(shader, std::ios::in);
		if (shaderStream.is_open()){
			string Line = "";
			while (getline(shaderStream, Line))
				shaderCode += "\n" + Line;
			shaderStream.close();
		}
		else {
			printf("Shader Error: Impossible to open. Check filepath: %s\n", shader);
			return 0;
		}
		return shaderCode;
	}
	else {
		return shaderCode = shader;
	}
}

static GLvoid useShader(Shader* shader){
	glUseProgram(shader->getProgramID());
}

////////////////////////////////////////////////////////////////////////////////
#endif