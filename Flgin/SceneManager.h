#pragma once
#include "Singleton.h"
#define FSceneManager flgin::SceneManager::GetInstance()

namespace flgin
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void FixedUpdate();
		void Update();
		void Render() const;

	private:
		std::vector<std::shared_ptr<Scene>> m_spScenes;
	};

}
