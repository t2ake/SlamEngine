#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

SandboxLayer::SandboxLayer()
{
	SetName("SandboxLayer");

	m_camera.GetData().SetPosition(glm::vec3{ 0.0f, 0.0f, 5.0f });
	m_camera.GetData().SetRotationDegrees(glm::vec3{ 0.0f, -90.0f, 0.0f });

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};
	uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };

	sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
	pVertexBuffer->SetLayout(sl::VertexLayout
	{
		{"Position", sl::AttribType::Float, 3},
		{"UV", sl::AttribType::Float, 2},
	});
	sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));

	m_pVertexArray = sl::VertexArray::Create();
	m_pVertexArray->SetVertexBuffer(pVertexBuffer);
	m_pVertexArray->SetIndexBuffer(pIndexBuffer);

	m_pShader = sl::Shader::Creat("Test Shader",
		sl::Path::FromeAsset("Shader/vs_Test.glsl"),
		sl::Path::FromeAsset("Shader/fs_Test.glsl"));

	m_pTextureJoucho = sl::Texture2D::Create(sl::Path::FromeAsset("Texture/jc.png"));
	m_pTextureLogo = sl::Texture2D::Create(sl::Path::FromeAsset("Texture/logo.png"));
}

SandboxLayer::~SandboxLayer()
{
	delete m_pVertexArray;
	delete m_pShader;
	delete m_pTextureJoucho;
	delete m_pTextureLogo;
}

void SandboxLayer::OnAttach()
{

}

void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnEvent(sl::Event &event)
{
	m_camera.OnEvent(event);
}

void SandboxLayer::BeginFrame()
{

}

void SandboxLayer::OnUpdate(float deltaTime)
{
	m_camera.Update(deltaTime);
}

void SandboxLayer::OnRender()
{
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			glm::vec3 trans{ i, j, 0.0f };
			glm::mat4 modelMat = glm::translate(glm::mat4{ 1.0f }, trans * 1.1f);
			glm::mat4 mvp = m_camera.GetData().GetViewProjection() * modelMat;

			m_pShader->Bind();
			m_pShader->UploadUniform("u_ModelViewProjection", std::move(mvp));
			m_pTextureJoucho->Bind(0);
			sl::RenderCore::Submit(m_pVertexArray, m_pShader);
		}
	}

	glm::mat4 modelMat2 = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ 0.0f, 0.0f, 0.25f });
	modelMat2 = glm::scale(modelMat2, glm::vec3(4.0f, 2.0f, 0.0f));
	glm::mat4 mvp2 = m_camera.GetData().GetViewProjection() * modelMat2;
	m_pShader->Bind();
	m_pShader->UploadUniform("u_ModelViewProjection", std::move(mvp2));
	m_pTextureLogo->Bind(0);
	sl::RenderCore::Submit(m_pVertexArray, m_pShader);
}

void SandboxLayer::EndFrame()
{

}
