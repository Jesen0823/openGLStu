#include "TestTexture2D.h"

#include <GL/glew.h>
#include "Renderer.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

namespace test{

	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_Proj(glm::ortho(-0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		float position2[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			50.0f, -50.0f, 1.0f, 0.0f, //1
			50.0f, 50.0f, 1.0f, 1.0f, //2
			-50.0f, 50.0f, 0.0f, 1.0f  //3
		};
		// 索引
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// 透明度的混合
		GLCall2(glEnable(GL_BLEND));
		GLCall2(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_vAo = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(position2, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_vAo->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic3.shader");
		m_Shader->Bind();
		m_Shader->setUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 1.0f);
	
		m_Texture = std::make_unique<Texture>("res/demo.png");
		m_Shader->setUniform1i("u_Texture", 0);
	}
	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}
	void TestTexture2D::OnRender()
	{
		GLCall2(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall2(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();
		
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->setUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_vAo, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->setUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_vAo, *m_IndexBuffer, *m_Shader);
		}
	}
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}