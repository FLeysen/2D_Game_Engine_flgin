#pragma once
#include "SceneManager.h"

namespace flgin
{
	class RenderComponent;
	class GameObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void AddGameObject(GameObject* const object);
		void AddRenderComponent(RenderComponent* const renderComponent);

		void FixedUpdate();
		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name{};
		std::vector<GameObject*> m_pGameObjects{};
		std::vector<RenderComponent*> m_pRenderComponents{};

		static unsigned int idCounter; 
	};

}
