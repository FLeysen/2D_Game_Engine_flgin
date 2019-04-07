#pragma once
#include "Singleton.h"

namespace flgin
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene & CreateScene(const std::string& name);

		void Update();
		void Render() const;

	private:
		std::vector<std::shared_ptr<Scene>> m_spScenes;
	};

}
