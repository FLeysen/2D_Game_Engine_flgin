#include "FlginPCH.h"
#include "StateComponent.h"
#include "GameObject.h"

flgin::StateComponent::StateComponent(GameObject* pOwnerObject)
	: BaseComponent(pOwnerObject)
	, m_pCurrentState{ nullptr }
{}

void flgin::StateComponent::Update()
{
#ifdef _DEBUG
	if (!m_pCurrentState) FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to update simple state component with no current state!", this });
#endif
	if (m_pCurrentState->Update())
	{
		m_pCurrentState->Exit();
		m_pCurrentState = m_pCurrentState->GetTargetState();
		m_pCurrentState->Enter();
	}
}

void flgin::StateComponent::SetCurrentState(State* pNewState)
{
	m_pCurrentState = pNewState;
	pNewState->Enter();
}