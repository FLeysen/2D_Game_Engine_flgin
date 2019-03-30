#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

RenderComponent::RenderComponent(dae::GameObject* const ownerObject)
	: BaseComponent(ownerObject)
	, m_pTexture{ nullptr }
{}

RenderComponent::~RenderComponent()
{
	 Logger::GetInstance().SafeDelete(m_pTexture);
}

void RenderComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const glm::vec2& pos{ m_pOwnerObject->GetPosition() };
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void RenderComponent::SetTexture(dae::Texture2D* const newTexture)
{
	Logger::GetInstance().SafeDelete(m_pTexture);
	m_pTexture = newTexture;
}
