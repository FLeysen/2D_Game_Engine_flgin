#pragma once
#include "GameObject.h"
namespace flgin
{
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* ownerObject) : m_pOwnerObject{ ownerObject } {};
		virtual ~BaseComponent() = default;

		virtual void Update() = 0;
		GameObject* GetGameObject() { return m_pOwnerObject; };

		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) = delete;

	protected:
		GameObject* m_pOwnerObject;
	};
}