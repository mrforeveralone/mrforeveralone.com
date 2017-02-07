/*
* Created by Chris Lanham.
*/

// must be defined before including stbi_image 
// stbi_image is used in simpleTexture (SimpTex texture loader)
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
// libraries and headers
#include "AdditionalLibraries(VS13)\GLEW\include\glew.h"
#include "AdditionalLibraries(VS13)\GLFW\include\glfw3.h"
#include "AdditionalLibraries(VS13)\GLM\glm.hpp"
#include "AdditionalLibraries(VS13)\GLM\ext.hpp"
#include "AdditionalLibraries(VS13)\shader.h"
#include "AdditionalLibraries(VS13)\simpTex.h"
#include "Cube3D.h"
using namespace std;
////////////////////////////////////////////////////////////////////////////////
//optional line to remove the Console Window popup                            //
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //
////////////////////////////////////////////////////////////////////////////////

// Function prototypes
GLvoid initialize(GLvoid);
GLvoid render(GLvoid);
GLvoid update(GLvoid);
GLvoid gameLoop(GLvoid);
GLvoid cleanUp(GLvoid);

// Window paramaters //
GLFWwindow* gameWindow;							// window instance
static const std::string WINDOW_TITLE =
		"Textured Cube";						// game title
GLuint windowWidth = 1280;						// size width
GLuint windowHeight = 720;						// size height

// Main Variables
Shader
shaderBasic = Shader();

Cube3D cube = Cube3D();

glm::mat4 projectionMatrix(1.0f);
glm::mat4 modelMatrix(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 MVP(1.0f);

glm::vec3
	cameraPosition,
	lookAtPosition,
	cameraUpVector;

GLuint
	cubeTexture;

//GLfloat zPos = 10.0f;
#include "inputControls.h"
GLvoid initialize(GLvoid){
	/*
	*	set up openGL enviroment
	*/
	glEnable(GL_DEPTH_TEST);// Enable depth test
	glDepthFunc(GL_LESS);	// Accept fragment if it closer to the camera than the former one
	glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera
	glEnable(GL_BLEND);		// Blend alpha component with background (enable transprancy)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Blend function to use

	/*
	*	create and compile shader(s)
	*/
	shaderBasic.createShader("shaders/simple.vertexShader", "shaders/texColourMix.fragmentShader");

	/*
	*	Set up projection matrix for this shader.
	*	Choose between ortho (2D) and perspective (3D)
	*/
	//projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); //2D
	projectionMatrix = glm::perspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f); //3D
	shaderBasic.setUniformPerspective(projectionMatrix);
	
	/*
	*	Set up projection matrix for this shader.
	*/
	cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	lookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	viewMatrix = glm::lookAt(
		cameraPosition,		// Camera origin in World Space
		lookAtPosition,		// Looking at this point
		cameraUpVector		// Top of camera points in this direction
		);
	shaderBasic.setUniformView(viewMatrix);

	/*
	*	Remove texture for plain colours
	*/
	cubeTexture = simpTex_loadTexture("Textures/ringsAlpha.png", FLAG_PRE_MULTI_ALPHA);
	if (cubeTexture != 0){
		cube = Cube3D(&shaderBasic, cubeTexture);
		cube.createInterleavedBuffers();
	}
	else{
		cube = Cube3D(&shaderBasic);
		cube.createSeparateBuffers();
	}
}

GLvoid render(GLvoid){
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);	// background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// objects to render
	cube.renderCube();
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

GLuint main(GLvoid){
	// Initialise GLFW
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("GLFW initialized\n");

	// GLFW window setup
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		// OpenGL major version to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);		// OpenGL minor version to use
	glfwWindowHint(GLFW_SAMPLES, 4);					// GLFW antialiasing

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

	// Register callback(s)
	glfwSetKeyCallback(gameWindow, key_callback);

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
	// clean up at program end.
	shaderBasic.deleteShader();
}