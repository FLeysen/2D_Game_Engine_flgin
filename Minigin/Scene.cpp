#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"

unsigned int dae::Scene::idCounter = 0;

dae::Scene::Scene(const std::string& name) : m_Name(name) {}

dae::Scene::~Scene()
{
	Logger& logger{ Logger::GetInstance() };
	for (GameObject* gameObject : m_pGameObjects)
		logger.SafeDelete(gameObject);
};

void dae::Scene::AddGameObject(GameObject* const object)
{
	m_pGameObjects.push_back(object);
}

void dae::Scene::AddRenderComponent(RenderComponent* const renderComponent)
{
	m_pRenderComponents.push_back(renderComponent);
}

void dae::Scene::Update()
{
	for(GameObject* const gameObject : m_pGameObjects)
	{
		gameObject->Update();
	}
}

void dae::Scene::Render() const
{
	for (RenderComponent* const renderComponent : m_pRenderComponents)
	{
		renderComponent->Render();
	}
}

