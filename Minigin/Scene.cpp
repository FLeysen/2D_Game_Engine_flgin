#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"

unsigned int flgin::Scene::idCounter{ 0 };

flgin::Scene::Scene(const std::string& name) 
	: m_Name(name) 
{}

flgin::Scene::~Scene()
{
	Logger& logger{ Logger::GetInstance() };
	for (GameObject* gameObject : m_pGameObjects)
		logger.SafeDelete(gameObject);
};

void flgin::Scene::AddGameObject(GameObject* const object)
{
	m_pGameObjects.push_back(object);
}

void flgin::Scene::AddRenderComponent(RenderComponent* const renderComponent)
{
	m_pRenderComponents.push_back(renderComponent);
}

void flgin::Scene::Update()
{
	for(GameObject* const gameObject : m_pGameObjects)
	{
		gameObject->Update();
	}
}

void flgin::Scene::Render() const
{
	for (RenderComponent* const renderComponent : m_pRenderComponents)
	{
		renderComponent->Render();
	}
}

