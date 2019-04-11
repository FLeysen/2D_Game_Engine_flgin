#include "MiniginPCH.h"
#include "MovementCommands.h"
#include "GridMovementComponent.h"

flgin::DirectionalGridMove::DirectionalGridMove(GridMovementComponent* attachedMover, bool isHorizontal, bool isPositiveDirection)
	: m_IsHorizontal{ isHorizontal }
	, m_IsPositiveDirection{ isPositiveDirection }
	, m_pGridMover{ attachedMover }
{}

bool flgin::DirectionalGridMove::Execute(GameObject&, bool isInverseAction)
{
	if (isInverseAction)
	{
		m_pGridMover->StopMoving();
		return true;
	}
	m_pGridMover->SetMoving(m_IsHorizontal, m_IsPositiveDirection);
	return true;
}