#pragma once
#include <memory>

#include "Transform.h"
#include "Texture2D.h"
namespace flgin
{
	class BaseComponent;
	class GameObject final
	{
	public:
		GameObject();

		void Update();
		void FixedUpdate();

		void SetPosition(float x, float y);
		glm::vec2 GetPosition() const;
		void AddComponent(BaseComponent* const component);
		void RemoveComponent(BaseComponent* const component);

		template <class T>
		void RemoveComponentsOfType()
		{
			const type_info& typeInfo{ typeid(T) };
			for (flgin::BaseComponent* const component : m_pComponents)
			{
				if (typeid(*component) == typeInfo)
					FLogger.SafeDelete(component);
			}
		}
		template <class T> 
		T* GetComponent()
		{
			const type_info& typeInfo{ typeid(T) };
			for (flgin::BaseComponent* const component : m_pComponents)
			{
				if (typeid(*component) == typeInfo)					
					return static_cast<T*>(component);
			}
			return nullptr;
		}

		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
	    Transform m_Transform;
		std::vector<BaseComponent*> m_pComponents;
	};
}