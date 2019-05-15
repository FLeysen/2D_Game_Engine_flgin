#include "FlginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void flgin::SceneManager::Update()
{
	m_pCurrScene->Update();
}

void flgin::SceneManager::FixedUpdate()
{
	m_pCurrScene->FixedUpdate();
}

void flgin::SceneManager::Render() const
{
	m_pCurrScene->Render();
}

void flgin::SceneManager::ActivateSceneByName(std::string&& name)
{
	if (m_pScenes.find(name) == m_pScenes.cend())
	{
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to activate nonexistant scene!" });
		return;
	}
	m_pCurrScene = m_pScenes[name];
}

flgin::Scene* flgin::SceneManager::CreateScene(const std::string& name)
{
	if (m_pScenes.find(name) != m_pScenes.cend())
	{
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to create scene, but a scene with the same name is already present!" });
		return nullptr;
	}
	m_pScenes[name] = new Scene{ name };
	return m_pScenes[name];
}

flgin::SceneManager::~SceneManager()
{
	for (std::pair<std::string, Scene*> scene : m_pScenes)
		FLogger.SafeDelete(scene.second);
}
