#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"



int main(void)
{
	GLFWwindow* window;


	/* Initialize GLFW library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Init GLEW Library Loader
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	//Vertex positions
	float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, //0
		 0.5f, -0.5f, 1.0f, 0.0f, //1
		 0.5f,  0.5f, 1.0f, 1.0f, //2
		-0.5f,  0.5f, 0.0f, 1.0f  //3
	};

	//Vertex Indices
	unsigned int indices[] = {
		0, 1, 2,
		3, 2, 0
	};

	//Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Vertex Array
	VertexArray vao;

	//Vertex Buffer
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	//Vertex Layout
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vb, layout);
	
	//Index Buffer
	IndexBuffer ib(indices, 6);
	
	//Shaders
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

	//Texture
	Texture tex("res/textures/OpenGL.png");
	tex.Bind();
	shader.SetUniform1i("u_Texture", 0);

	//Unbound buffers and shader
	vao.UnBind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();


	Renderer render;
	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		render.Clear();

		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
	
		render.Draw(vao, ib, shader);
		
		if (r > 1.0f)
			increment = -0.05;
		else if (r < 0.0f)
			increment = 0.05;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}