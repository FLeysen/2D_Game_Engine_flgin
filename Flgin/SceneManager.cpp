#include "FlginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void flgin::SceneManager::Update()
{
	for(auto scene : m_spScenes)
	{
		scene->Update();
	}
}

void flgin::SceneManager::FixedUpdate()
{
	for (auto scene : m_spScenes)
	{
		scene->FixedUpdate();
	}
}

void flgin::SceneManager::Render() const
{
	for (const auto scene : m_spScenes)
	{
		scene->Render();
	}
}

flgin::Scene& flgin::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_spScenes.push_back(scene);
	return *scene;
}
