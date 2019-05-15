#pragma once
#include "BaseComponent.h"

namespace flgin
{
	/*
	class FunctionHolderBase;
	class ConditionalFunctionHolderBase;
	class State;

	class StateTransition final
	{
	public:
		StateTransition(std::vector<ConditionalFunctionHolderBase*>&& conditions, std::vector<FunctionHolderBase*>&& actions, State* resultingState)
			: m_pTargetState{ resultingState }
			, m_pConditions{ conditions }
			, m_pActions{ actions } {}
		~StateTransition();

		bool AreConditionsMet() const;
		void ExecuteActions() const;
		State* GetTargetState() const { return m_pTargetState; }

	private:
		State* m_pTargetState;
		std::vector<ConditionalFunctionHolderBase*> m_pConditions;
		std::vector<FunctionHolderBase*> m_pActions;
	};

	class State final
	{
	public:
		State(std::vector<FunctionHolderBase*>&& entryActions, std::vector<FunctionHolderBase*>&& actions, std::vector<FunctionHolderBase*>&& exitActions, std::vector<StateTransition>&& transitions)
			: m_pEntryActions{ entryActions }
			, m_pActions{ actions }
			, m_pExitActions{ exitActions }
			, m_Transitions{ transitions }{}
		~State();

		void ExecuteEntryActions();
		void ExecuteActions();
		void ExecuteExitActions();
		void SetTransitions(std::vector<StateTransition>&& transitions) { m_Transitions = transitions; }
		const std::vector<StateTransition>& GetTransitions() const { return m_Transitions; }

	private:
		std::vector<FunctionHolderBase*> m_pEntryActions;
		std::vector<FunctionHolderBase*> m_pActions;
		std::vector<FunctionHolderBase*> m_pExitActions;
		std::vector<StateTransition> m_Transitions;
	};

	class StateComponent final : public BaseComponent
	{
	public:
		StateComponent(GameObject* pOwnerObject, std::vector<State>&& states, State* initialState)
			: BaseComponent(pOwnerObject)
			, m_CurrentState{ initialState }
			, m_States{ states }{}
		void Update() override;

		~StateComponent() = default;
		StateComponent(StateComponent&&) = delete;
		StateComponent(const StateComponent&) = delete;
		StateComponent& operator=(StateComponent&&) = delete;
		StateComponent& operator=(const StateComponent&) = delete;

	private:
		State* m_CurrentState;
		std::vector<State> m_States;
	};
	*/
}