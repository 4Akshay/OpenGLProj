//#include <stdio.h>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//window diemensions
const GLint WIDTH = 800, HEIGHT = 800;

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialise window" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//No Backward compatibility;
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allowed forward compatibility;
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the instance of GLFW window;
	GLFWwindow* mainWidow = glfwCreateWindow(WIDTH, HEIGHT, "Demo", NULL, NULL);
	if (!mainWidow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferwidth, bufferheight;
	glfwGetFramebufferSize(mainWidow, &bufferwidth, &bufferheight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWidow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;


	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize the glew" << std::endl;
		glfwDestroyWindow(mainWidow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferwidth, bufferheight);
	
	// Loop until window close;

	while (!glfwWindowShouldClose(mainWidow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		// clear window
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWidow);
	}

	return 0;
}