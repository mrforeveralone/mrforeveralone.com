
glm::mat4 rotated(1.0f);

static GLvoid key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods)
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_W:
		if (action == GLFW_RELEASE){
			rotated = glm::rotate(cube.getModelMatrix(), glm::radians(45.0f), Z_AXIS);
			cube.setModelMatrix(rotated);
		}
		break;
	case GLFW_KEY_S:
		if (action == GLFW_RELEASE){
			rotated = glm::rotate(cube.getModelMatrix(), -glm::radians(45.0f), Z_AXIS);
			cube.setModelMatrix(rotated);
		}
		break;
	case GLFW_KEY_A:
		if (action == GLFW_RELEASE){
			rotated = glm::rotate(cube.getModelMatrix(), glm::radians(45.0f), Y_AXIS);
			cube.setModelMatrix(rotated);
		}
		break;
	case GLFW_KEY_D:
		if (action == GLFW_RELEASE){
			rotated = glm::rotate(cube.getModelMatrix(), -glm::radians(45.0f), Y_AXIS);
			cube.setModelMatrix(rotated);
		}
		break;
	default:
		break;
	}
}