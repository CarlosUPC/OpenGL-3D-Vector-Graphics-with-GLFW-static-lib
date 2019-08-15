#include "TestTexture2D.h"
#include "GL/glew.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D()
		:m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200,200,0), m_TranslationB(400,200,0)
	{
		//Vertex positions
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			 50.0f, -50.0f, 1.0f, 0.0f, //1
			 50.0f,  50.0f, 1.0f, 1.0f, //2
			-50.0f,  50.0f, 0.0f, 1.0f  //3
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
		m_VAO = std::make_unique<VertexArray>();

		//Vertex Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		//Vertex Layout
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//Index Buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		//Shaders
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		//Texture
		m_Texture2D = std::make_unique<Texture>("res/textures/OpenGL.png");
		m_Texture2D->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);

		
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float dt)
	{

	}

	void TestTexture2D::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer render;
		m_Texture2D->Bind();

		{

			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		}


	}

	void TestTexture2D::OnImGUIRender()
	{
		ImGui::SliderFloat3("Translation A", (float*)&m_TranslationA, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", (float*)&m_TranslationB, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}