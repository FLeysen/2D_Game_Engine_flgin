#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "Log.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "Texture2D.h"

dae::GameObject::GameObject()
	: m_pComponents{}
	, m_Transform{}
{}

dae::GameObject::~GameObject()
{
	Logger& logger{ Logger::GetInstance() };
	for (BaseComponent* component : m_pComponents)
		 logger.SafeDelete(component);
};

void dae::GameObject::Update()
{
	for (BaseComponent* component : m_pComponents)
		component->Update();
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

glm::vec2 dae::GameObject::GetPosition()
{
	return glm::vec2{ m_Transform.GetPosition().x, m_Transform.GetPosition().y };
}

void dae::GameObject::AddComponent(BaseComponent* const component)
{
	m_pComponents.push_back(component);
}

void dae::GameObject::RemoveComponent(BaseComponent* const component)
{
	std::vector<BaseComponent*>::const_iterator cIt{ std::find(m_pComponents.cbegin(), m_pComponents.cend(), component) };
	if (cIt != m_pComponents.cend())
		m_pComponents.erase(cIt);
	else
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::WARNING, "Attempted to remove component " + std::string(typeid(component).name()) + ", but it was not found.", component });
}