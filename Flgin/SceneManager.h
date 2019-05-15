#pragma once
#include "Singleton.h"
#include <unordered_map>
#define FSceneManager flgin::SceneManager::GetInstance()

namespace flgin
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);
		~SceneManager();

		bool IsSceneSet() { return m_pCurrScene; }
		void FixedUpdate();
		void Update();
		void Render() const;
		void ActivateSceneByName(std::string&& name);

	private:
		Scene* m_pCurrScene{ nullptr };
		std::unordered_map<std::string, Scene*> m_pScenes{};
	};

}
