#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "Scene.h"

flgin::RenderComponent::RenderComponent(GameObject* const ownerObject, Scene& scene)
	: BaseComponent(ownerObject)
	, m_pTexture{ nullptr }
	, m_XOffset{ 0.0f }
	, m_YOffset{ 0.0f }
{
	scene.AddRenderComponent(this);
}

flgin::RenderComponent::~RenderComponent()
{
	 Logger::GetInstance().SafeDelete(m_pTexture);
}

void flgin::RenderComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const glm::vec2& pos{ m_pOwnerObject->GetPosition() };
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x + m_XOffset, pos.y + m_YOffset);
	}
}

void flgin::RenderComponent::SetTexture(Texture2D* const newTexture)
{
	Logger::GetInstance().SafeDelete(m_pTexture, true);
	m_pTexture = newTexture;
}

void flgin::RenderComponent::SetPositionOffset(float x, float y)
{
	m_XOffset = x;
	m_YOffset = y;
}
