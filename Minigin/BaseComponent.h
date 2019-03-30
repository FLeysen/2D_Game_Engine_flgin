#pragma once
#include "GameObject.h"
class BaseComponent
{
public:
	BaseComponent(dae::GameObject* ownerObject) : m_pOwnerObject{ ownerObject }{};
	virtual ~BaseComponent() = default;
	
	virtual void Update() = 0;
	dae::GameObject* GetGameObject() { return m_pOwnerObject; };

	BaseComponent& operator=(const BaseComponent&) = delete;
	BaseComponent& operator=(BaseComponent&&) = delete;

protected:
	dae::GameObject* m_pOwnerObject;
};