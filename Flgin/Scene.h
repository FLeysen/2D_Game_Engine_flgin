#pragma once
#include "SceneManager.h"
#include "SpriteComponent.h"
#include "RenderComponent.h"

namespace flgin
{
	class RenderComponent;
	class GameObject;
	class SpriteComponent;

	class Scene
	{
	public:
		void AddGameObject(GameObject* const object);
		RenderComponent* CreateRenderComponent(GameObject* const ownerObject, unsigned int layer = 2);
		SpriteComponent* CreateSpriteComponent(GameObject* const ownerObject, unsigned int layer = 2);

		void FixedUpdate();
		void Update();
		void Render() const;

		template <class T>
		T* FindComponentOfType()
		{
			for (GameObject* const go : m_pGameObjects)
			{
				T* component{ go->GetComponent<T>() };
				if (component) return component;
			}
			return nullptr;
		}

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		friend Scene* SceneManager::CreateScene(const std::string& name);
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<GameObject*> m_pGameObjects;
		static const unsigned int MAX_RENDERLAYERS{ 5 };
		static const unsigned int MAX_RENDERERS_PER_LAYER{ 300 };
		RenderComponent m_RenderComponents[MAX_RENDERLAYERS * MAX_RENDERERS_PER_LAYER];
		SpriteComponent m_SpriteComponents[MAX_RENDERLAYERS * MAX_RENDERERS_PER_LAYER];
		unsigned int m_RenderCompCount[MAX_RENDERLAYERS];
		unsigned int m_SpriteCompCount[MAX_RENDERLAYERS];
	};

}
