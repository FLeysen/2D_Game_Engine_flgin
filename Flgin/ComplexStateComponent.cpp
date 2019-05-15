#include "FlginPCH.h"
#include "StateComponent.h"
#include "FunctionHolder.h"

/*
void flgin::StateComponent::Update()
{
	const std::vector<StateTransition>& transitions{ m_CurrentState->GetTransitions() };
	StateTransition const* triggeredTransition{ nullptr };
	for (const StateTransition& transition : transitions)
	{
		if (transition.AreConditionsMet())
		{
			triggeredTransition = &transition;
			break;
		}
	}

	if (triggeredTransition)
	{
		State* targetState{ triggeredTransition->GetTargetState() };
		m_CurrentState->ExecuteExitActions();
		triggeredTransition->ExecuteActions();
		targetState->ExecuteEntryActions();
		m_CurrentState = targetState;
	}
	else m_CurrentState->ExecuteActions();
}

flgin::State::~State()
{
	for (FunctionHolderBase* entryAction : m_pEntryActions)
		FLogger.SafeDelete(entryAction);
	for (FunctionHolderBase* action : m_pActions)
		FLogger.SafeDelete(action);
	for (FunctionHolderBase* exitAction : m_pExitActions)
		FLogger.SafeDelete(exitAction);
}

void flgin::State::ExecuteEntryActions()
{
	for (FunctionHolderBase* entryAction : m_pEntryActions)
		entryAction->Call();
}

void flgin::State::ExecuteActions()
{
	for (FunctionHolderBase* action : m_pActions)
		action->Call();
}

void flgin::State::ExecuteExitActions()
{
	for (FunctionHolderBase* exitAction : m_pExitActions)
		exitAction->Call();
}

flgin::StateTransition::~StateTransition()
{
	for (FunctionHolderBase* actions : m_pActions)
		FLogger.SafeDelete(actions);
	for (ConditionalFunctionHolderBase* condition : m_pConditions)
		FLogger.SafeDelete(condition);
}

bool flgin::StateTransition::AreConditionsMet() const
{
	for (ConditionalFunctionHolderBase* condition : m_pConditions)
		if (!condition->Call()) return false;
	return true;
}

void flgin::StateTransition::ExecuteActions() const
{
	for (FunctionHolderBase* action : m_pActions)
		action->Call();
}
*/
