#include "pch.h"
#include "FygarController.h"
#include "Fygar.h"
#include "GridMovementComponent.h"
#include "MovementGrid.h"
#include "MovementCommands.h"

DigDug::FygarController::FygarController(flgin::GameObject* ownerObject, Fygar* fygar, flgin::GridMovementComponent* mover)
	: BaseComponent(ownerObject)
	, m_pFygar{ fygar }
	, m_pMover{ mover }
	, m_pLastNode{}
	, m_pTargetNode{}
	, m_pUpMove{}
	, m_pDownMove{}
	, m_pLeftMove{}
	, m_pRightMove{}
{}

void DigDug::FygarController::Update()
{
	
}
