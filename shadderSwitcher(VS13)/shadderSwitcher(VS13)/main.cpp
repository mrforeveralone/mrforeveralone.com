/*
* Created by Chris Lanham.
*/

// must be defined before including stbi_image 
// stbi_image is used in simpleTexture (texture loader)
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION

// libraries and headers
#include "AdditionalLibraries(VS13)\GLEW\include\glew.h"
#include "AdditionalLibraries(VS13)\GLFW\include\glfw3.h"
#include "AdditionalLibraries(VS13)\GLM\glm.hpp"
#include "AdditionalLibraries(VS13)\GLM\ext.hpp"
#include "AdditionalLibraries(VS13)\shader.h"
#include "AdditionalLibraries(VS13)\simpTex.h"
#include "plainCharShaders.h"
#include "Triangle2D.h"
using namespace std;
////////////////////////////////////////////////////////////////////////////////
//optional line to remove the Console Window popup                            //
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //
////////////////////////////////////////////////////////////////////////////////

// Function prototypes
GLuint  main(GLvoid);
GLvoid initialize(GLvoid);
GLvoid render(GLvoid);
GLvoid update(GLvoid);
GLvoid gameLoop(GLvoid);
GLvoid cleanUp(GLvoid);

// Window paramaters //
GLFWwindow* gameWindow;							// window instance
static const std::string WINDOW_TITLE =
"Switching shaders and multiple triangles";						// game title
GLuint windowWidth = 1280;						// size width
GLuint windowHeight = 720;						// size height

// Main Variables
Shader
shaderBasic,
shaderColour,
shaderTexture,
shaderTexColMix = Shader();

Triangle2D
topLeftTri,
botLeftTri,
botRightTri,
topRightTri = Triangle2D();

GLint
pngWithAlpha,
brickJPG;

GLvoid initialize(GLvoid){
	/*
	*	set up openGL enviroment
	*/
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	*	create and compile shader(s)
	*/
	shaderBasic.createShader(VertexShader, FragmentShader_white);
	shaderColour.createShader(VertexShader, FragmentShader_colour);
	shaderTexture.createShader(VertexShader, FragmentShader_texture);
	shaderTexColMix.createShader(VertexShader, FragmentShader_textureAndColourMix);

	/*
	*	load textures for triangles
	*/
	pngWithAlpha = simpTex_loadTexture("Textures/ringsAlpha.png");
	brickJPG = simpTex_loadTexture("Textures/brick.jpg");

	/*
	*	create triangles
	*/
	topLeftTri = Triangle2D(
		&shaderBasic,
		glm::vec3(-0.5f, 0.8f, 0.0f),
		glm::vec3(-0.8f, 0.2f, 0.0f),
		glm::vec3(-0.2f, 0.2f, 0.0f)
		);
	botLeftTri = Triangle2D(
		&shaderColour,
		glm::vec3(-0.5f, -0.2, 0.0f),
		glm::vec3(-0.8f, -0.8, 0.0f),
		glm::vec3(-0.2f, -0.8, 0.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
		);
	botRightTri = Triangle2D(
		&shaderTexture,
		glm::vec3(0.5f, -0.2f, 0.0f),
		glm::vec3(0.2f, -0.8f, 0.0f),
		glm::vec3(0.8f, -0.8f, 0.0f),
		pngWithAlpha
		);
	topRightTri = Triangle2D(
		&shaderTexColMix,
		glm::vec3(0.5f, 0.8f, 0.0f),
		glm::vec3(0.2f, 0.2f, 0.0f),
		glm::vec3(0.8f, 0.2f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		brickJPG
		);
}

GLvoid render(GLvoid){
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);	// background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// objects to render
	topLeftTri.renderTriangle();
	botLeftTri.renderTriangle();
	botRightTri.renderTriangle();
	topRightTri.renderTriangle();
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);		// OpenGL minor version to use
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
	shaderColour.deleteShader();
	shaderTexture.deleteShader();
	shaderTexColMix.deleteShader();
	simpTex_deleteTexture(pngWithAlpha);
	simpTex_deleteTexture(brickJPG);
}