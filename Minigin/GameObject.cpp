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

flgin::GameObject::GameObject()
	: m_pComponents{}
	, m_Transform{}
{}

flgin::GameObject::~GameObject()
{
	Logger& logger{ Logger::GetInstance() };
	for (BaseComponent* component : m_pComponents)
		 logger.SafeDelete(component);
};

void flgin::GameObject::Update()
{
	for (BaseComponent* component : m_pComponents)
		component->Update();
}

void flgin::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

glm::vec2 flgin::GameObject::GetPosition()
{
	return glm::vec2{ m_Transform.GetPosition().x, m_Transform.GetPosition().y };
}

void flgin::GameObject::AddComponent(BaseComponent* const component)
{
	m_pComponents.push_back(component);
}

void flgin::GameObject::RemoveComponent(BaseComponent* const component)
{
	std::vector<BaseComponent*>::const_iterator cIt{ std::find(m_pComponents.cbegin(), m_pComponents.cend(), component) };
	if (cIt != m_pComponents.cend())
		m_pComponents.erase(cIt);
	else
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::WARNING, "Attempted to remove component " + std::string(typeid(component).name()) + ", but it was not found.", component });
}