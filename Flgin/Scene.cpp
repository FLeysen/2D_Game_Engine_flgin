#include "FlginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"

unsigned int flgin::Scene::idCounter{ 0 };

flgin::Scene::Scene(const std::string& name) 
	: m_Name(name) 
{}

flgin::Scene::~Scene()
{
	Logger& logger{ FLogger };
	for (GameObject* gameObject : m_pGameObjects)
		logger.SafeDelete(gameObject);
};

void flgin::Scene::AddGameObject(GameObject* const object)
{
	m_pGameObjects.push_back(object);
}

void flgin::Scene::AddRenderComponent(RenderComponent* const renderComponent, unsigned int layer)
{
	if (layer > MAX_RENDERLAYERS)
	{
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add render component to nonexistant layer: " + layer, renderComponent });
		return;
	}
	m_pRenderComponents[layer].push_back(renderComponent);
}

void flgin::Scene::Update()
{
	for(GameObject* const gameObject : m_pGameObjects)
	{
		gameObject->Update();
	}
}

void flgin::Scene::FixedUpdate()
{
	for (GameObject* const gameObject : m_pGameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void flgin::Scene::Render() const
{
	for (unsigned int i{}; i < MAX_RENDERLAYERS; ++i)
	{
		for (RenderComponent* const renderComponent : m_pRenderComponents[i])
		{
			renderComponent->Render();
		}
	}
}

