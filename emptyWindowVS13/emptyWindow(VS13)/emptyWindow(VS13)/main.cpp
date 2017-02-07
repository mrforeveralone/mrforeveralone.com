/*
* Created by Chris Lanham.
*/

// Standard C libraries 
#include <string>

// libraries and headers
#include "AdditionalLibraries(VS13)\GLEW\Include\glew.h"
#include "AdditionalLibraries(VS13)\GLFW\Include\glfw3.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
//optional line to remove the Console Window                                  //
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //
////////////////////////////////////////////////////////////////////////////////

// Function prototypes
GLuint main(GLvoid);
GLvoid initialize(GLvoid);
GLvoid render(GLvoid);
GLvoid update(GLvoid);
GLvoid gameLoop(GLvoid);
GLvoid cleanUp(GLvoid);

// Window paramaters //
GLFWwindow* gameWindow;						// window instance
static const std::string WINDOW_TITLE
= "first openGL window";				// game title
GLuint windowWidth = 1280;					// size width
GLuint windowHeight = 720;					// size height


GLvoid initialize(GLvoid){
	/*
	*	nothing to set up for now. just make a window.
	*/
}

GLvoid render(GLvoid){
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);	// background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glfwWindowHint(GLFW_SAMPLES, 4);					// GLFW antialiasing x4

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
	/*
	* nothing created so nothing extra to clean up
	*/
}