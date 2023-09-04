#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <cmath>
//window diemensions
const GLint WIDTH = 800, HEIGHT = 800;

GLuint VAO, VBO, shader, uniformXMove;

bool direction = true;
float trioffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;

//vertex shader
static const char* vShader = "                                              \n\
#version 330                                                                \n\
                                                                            \n\
layout (location = 0) in vec3 pos;                                          \n\
                                                                            \n\
uniform float xMove;                                                        \n\
                                                                            \n\
void main ()                                                                \n\
{                                                                           \n\
    gl_Position	= vec4 (0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);      \n\
}";

//fragment shader
static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
out vec4 color;                                                 \n\
                                                                \n\
void main ()                                                    \n\
{                                                               \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);                           \n\
}";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint Result = 0;
	GLchar log[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &Result);
	if (!Result)
	{
		glGetShaderInfoLog(theShader, sizeof(log), NULL, log);
		std::cout << "Error: Failed to compile " << shaderType << " program " << log << std::endl;
		return;
	}
	glAttachShader(theProgram, theShader);
	return;
}
void CompileShaders()
{
	shader = glCreateProgram();
	if (!shader)
	{
		std::cout << "Error: Failed to create shader program !\n";
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint Result = 0;
	GLchar log[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &Result);
	if (!Result)
	{
		glGetProgramInfoLog(shader, sizeof(log), NULL, log);
		std::cout << "Error: Failed to link program " << log << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &Result);
	if (!Result)
	{
		glGetProgramInfoLog(shader, sizeof(log), NULL, log);
		std::cout << "Error: Failed to validate program " << log << std::endl;
		return;
	}

	uniformXMove = glGetUniformLocation(shader, "xMove");
}

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

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
	CreateTriangle();
	CompileShaders();


	// Loop until window close;

	while (!glfwWindowShouldClose(mainWidow))
	{
		// Get + Handle user input events
		glfwPollEvents();
		if (direction)
		{
			trioffset += triIncrement;
		}
		else {
			trioffset -= triIncrement;
		}

		if (abs(trioffset) >= triMaxoffset)
			direction = !direction;

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glUniform1f(uniformXMove, trioffset);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWidow);
	}
	return 0;
}