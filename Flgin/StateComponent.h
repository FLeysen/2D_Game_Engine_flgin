#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class GameObject;
	class State
	{
	public:
		State() : m_pTargetState{ nullptr } {}
		virtual ~State() = default;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update(GameObject* pOrigin) = 0;
		State* GetTargetState() const { return m_pTargetState; }

	private:
		State* m_pTargetState;
	};

	class StateComponent final : public BaseComponent
	{
	public:
		StateComponent(GameObject* pOwnerObject);
		void Update();
		void SetCurrentState(State* pNewState) { m_pCurrentState = pNewState; }

		~StateComponent() = default;
		StateComponent(StateComponent&&) = delete;
		StateComponent(const StateComponent&) = delete;
		StateComponent& operator=(StateComponent&&) = delete;
		StateComponent& operator=(const StateComponent&) = delete;

	private:
		State* m_pCurrentState;
	};
}

