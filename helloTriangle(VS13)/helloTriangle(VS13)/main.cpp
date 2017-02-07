/*Created by Chris Lanham*/

// Standard C++ libraries
#include <string>

// Additional and third-party libraries
#include "AdditionalLibraries(VS13)\GLEW\Include\glew.h"
#include "AdditionalLibraries(VS13)\GLFW\Include\glfw3.h"
#include "AdditionalLibraries(VS13)\GLM\glm.hpp"
#include "AdditionalLibraries(VS13)\GLM\ext.hpp"
#include "AdditionalLibraries(VS13)\shader.h"

////////////////////////////////////////////////////////////////////////////////
//optional live to remove the Console Window popup                            //
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //
////////////////////////////////////////////////////////////////////////////////

// Shader code
const GLchar* VertexShader =
{
	"#version 400\n"\

	"layout(location = 0) in vec3 vertexPosition;\n"\
	"layout(location = 1) in vec3 vertexNormal;\n"\
	"layout(location = 2) in vec2 vertexTextureUV;\n"\
	"layout(location = 3) in vec4 vertexColour;\n"\

	"out vec4 fragColour;\n"\

	"void main(void)\n"\
	"{\n"\
	"gl_Position = vec4(vertexPosition,1);\n"\
	"fragColour = vertexColour;\n"\
	"}\n"
};
const GLchar* FragmentShader =
{
	"#version 400\n"\

	"in vec4 fragColour;\n"\
	"out vec4 out_Colour;\n"\

	"void main(void)\n"\
	"{\n"\
	"  out_Colour = vec4(0.2,0.4,0.6,1);\n"\
	"  //out_Colour = fragColour;\n"\
	"}\n"
};

// Main Variables
Shader basicShader = Shader();	// the shader to be used

GLuint triangleVertexArrayObject = 0;
GLuint triangleVertexBuffer = 0;
GLuint triangleColourBuffer = 0;

// Window paramaters //
GLFWwindow* gameWindow;										// window instance
static const std::string WINDOW_TITLE = "First Triangle";	// game title
GLuint windowWidth = 1280;									// size width
GLuint windowHeight = 720;									// size height

// Function prototypes
GLvoid initialize(GLvoid);
GLvoid render(GLvoid);
GLvoid update(GLvoid);
GLvoid gameLoop(GLvoid);
GLvoid key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods);
GLvoid resizeWindow(GLFWwindow* window, GLint width, GLint height);
GLvoid cleanUp(GLvoid);

GLvoid initialize(GLvoid){

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	basicShader.createShader(VertexShader, FragmentShader);
	useShader(&basicShader);

	// Create triangle
	GLfloat vertexArray[] = {
		 0.0f,  0.7f, 0.0f,
		-0.7f, -0.7f, 0.0f,
		 0.7f, -0.7f, 0.0f
	};

	GLfloat colourArray[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &triangleVertexArrayObject);	//Generate a VAO
	glBindVertexArray(triangleVertexArrayObject);		//Bind this VAO to the current state.

	// Create vertex buffer to add to VAO
	glGenBuffers(1, &triangleVertexBuffer);
	glBindBuffer(
		GL_ARRAY_BUFFER,
		triangleVertexBuffer
		);
	glBufferData(
		GL_ARRAY_BUFFER,		//Buffer type
		sizeof(vertexArray),	//Size of the buffer
		vertexArray,			//Data for the buffer to hold
		GL_STATIC_DRAW			//What to do with the buffer
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

	// Create colour buffer to add to VAO
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

GLvoid render(GLvoid){

	// Dark blue background
	glClearColor(1.0f, 1.0f, 0.4f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLvoid gameLoop(GLvoid){
	printf("starting game loop\n");
	while (!glfwWindowShouldClose(gameWindow)){
		render();						// render a new frame
		glfwSwapBuffers(gameWindow);	// swap draw buffers
		glfwPollEvents();				// check for user input, screen resize etc.
	}
	printf("exiting game loop\n");
}

GLint main(GLvoid)
{
	// Initialise GLFW
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("GLFW initialized\n");

	// GLFW window setup
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);					 // OpenGL major version to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);					 // OpenGL minor version to use
	glfwWindowHint(GLFW_SAMPLES, 4);								 // GLFW antialiasing

	// Create a window and make it our current context
	gameWindow = glfwCreateWindow(windowWidth, windowHeight, WINDOW_TITLE.c_str(), NULL, NULL);
	if (!gameWindow) {
		fprintf(stderr, "Failed to create a GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else
		glfwMakeContextCurrent(gameWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("GLEW initialized\n");

	//Initialize game
	initialize();

	//Enter game loop
	gameLoop();

	// Free up resources
	cleanUp();
	glfwDestroyWindow(gameWindow);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

GLvoid cleanUp(GLvoid){
	basicShader.deleteShader();
}