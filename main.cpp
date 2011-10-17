#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>

int main()
{
	glfwInit();

	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(1280, 720, 0, 0, 0, 0, 32, 0, GLFW_WINDOW);

	glewInit();

	glfwSetWindowTitle("OpenGL");

	glClearColor(1.0f, .0f, .2f, 1.0f);
	
	glfwSwapInterval(1);
	
	bool running = true;

	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers();

		// User input

		// Exit on escape key
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			running = false;

		// Exit on window close
		if (!glfwGetWindowParam(GLFW_OPENED))
			running = false;
	}

	glfwTerminate();
	return 0;
}
