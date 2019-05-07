#include "FlginPCH.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "Scene.h"
#include "SDL.h"

flgin::RenderComponent::RenderComponent(GameObject* const ownerObject, Scene& scene, unsigned int renderLayer)
	: BaseComponent(ownerObject)
	, m_spTexture{ nullptr }
	, m_XOffset{ 0.0f }
	, m_YOffset{ 0.0f }
	, m_Height{}
	, m_Width{}
{
	scene.AddRenderComponent(this, renderLayer);
}

void flgin::RenderComponent::Render() const
{
	if (!m_spTexture)
	{
		FLogger.Log(StatusCode{ StatusCode::Status::WARNING, "RenderComponent does not have an attached texture!", (void*)this });
		return;
	}
	const glm::vec2& pos{ m_pOwnerObject->GetPosition() };
	FRenderer.RenderTexture(*m_spTexture, pos.x + m_XOffset, pos.y + m_YOffset, m_Width, m_Height);
}

void flgin::RenderComponent::Update()
{}

void flgin::RenderComponent::SetTexture(std::shared_ptr<flgin::Texture2D>& newTexture)
{
	m_spTexture = newTexture;
	ResetDimensions();
}

void flgin::RenderComponent::SetTexture(std::shared_ptr<flgin::Texture2D>&& newTexture)
{
	m_spTexture = newTexture;
	ResetDimensions();
}

void flgin::RenderComponent::SetPositionOffset(float x, float y)
{
	m_XOffset = x;
	m_YOffset = y;
}

void flgin::RenderComponent::ResetDimensions()
{
	if (!m_spTexture)
	{
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to reset dimensions on nullpointer texture!" });
		return;
	}
	int width{};
	int height{};
	SDL_QueryTexture(m_spTexture->GetSDLTexture(), nullptr, nullptr, &width, &height);
	m_Width = float(width);
	m_Height = float(height);
}

void flgin::RenderComponent::SetDimensions(float width, float height)
{
	m_Width = width;
	m_Height = height;
}