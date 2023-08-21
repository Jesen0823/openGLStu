#include "TestBatchRender.h"

#include "Renderer.h"
#include "vendor/imgui/imgui.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

namespace test
{
	TestBatchRender::TestBatchRender()
		:m_Proj(glm::ortho(-0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
		m_Translation(0, 0, 0)
	{
		glm::vec3 m_Translation(glm::vec3(0, 0, 0));

		float positions[] = {
			100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			300.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			400.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			300.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall2(glEnable(GL_BLEND));
		GLCall2(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_vAo = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 11 * 8 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(4); // 坐标 x, y, z, w  w 齐次坐标, 对xyz进行缩放
		layout.Push<float>(4); // 颜色
		layout.Push<float>(2); // 纹理坐标
		layout.Push<float>(1); // 纹理插槽
		m_vAo->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("res/shaders/Batch.shader");
		m_Shader->Bind();

		m_Texture[0] = std::make_unique<Texture>("res/logoimg.png");
		m_Texture[1] = std::make_unique<Texture>("res/demo.png");
		for (size_t i = 0; i < 2; i++)
		{
			m_Texture[i]->Bind(i);
		}
		int samplers[2] = { 0, 1 };
		m_Shader->setUniform1iv("u_Textures", 2, samplers);
	}

	TestBatchRender::~TestBatchRender()
	{
	}

	void TestBatchRender::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRender::OnRender()
	{
		GLCall2(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall2(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;

		m_Shader->Bind();
		m_Shader->setUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_vAo, *m_IndexBuffer, *m_Shader);
	}

	void TestBatchRender::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}