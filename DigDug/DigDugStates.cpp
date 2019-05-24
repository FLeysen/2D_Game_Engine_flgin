#include "pch.h"
#include "DigDugStates.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "ResourceManager.h"
#include "TextLocalizer.h"
#include "GridMovementComponent.h"

void DigDug::DigDugState::SetAttachedSprite(flgin::SpriteComponent* pSprite)
{
	m_pSpriteComponent = pSprite;
}

void DigDug::DigDugState::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

DigDug::IdleState::IdleState()
	: DigDugState()
{}

void DigDug::IdleState::Enter()
{
	m_pSpriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")), true);
	m_pGridMover = m_pPlayer->GetGameObject()->GetComponent<flgin::GridMovementComponent>();
}

bool DigDug::IdleState::Update()
{
	if (m_pGridMover->GetVelocity().x != 0.f || m_pGridMover->GetVelocity().y != 0.f)
	{
		MovingState* movingState{ new MovingState{} };
		movingState->SetAttachedSprite(m_pSpriteComponent);
		movingState->SetPlayer(m_pPlayer);
		m_pTargetState = movingState;
		return true;
	}
	return false;
}

void DigDug::MovingState::Enter()
{
	m_pGridMover = m_pPlayer->GetGameObject()->GetComponent<flgin::GridMovementComponent>();
	m_pSpriteComponent->SetRotationalOffset(0.f, 0.f);
	m_PreviousDirection = m_pGridMover->GetMovementDirection();
}

bool DigDug::MovingState::Update()
{
	if (m_pGridMover->GetVelocity().x != 0.f || m_pGridMover->GetVelocity().y != 0.f)
	{
		switch (m_pGridMover->GetMovementDirection())
		{
		case flgin::MovementDirection::Up:
			m_pSpriteComponent->SetRotation(270.f);
			if (m_PreviousDirection == flgin::MovementDirection::Right) m_pSpriteComponent->SetFlips(false, false);
			else if (m_PreviousDirection == flgin::MovementDirection::Down) m_pSpriteComponent->SetFlips(false, !m_pSpriteComponent->GetFlippedVertical());
			break;
		case flgin::MovementDirection::Down:
			m_pSpriteComponent->SetRotation(90.f);
			if (m_PreviousDirection == flgin::MovementDirection::Left) m_pSpriteComponent->SetFlips(false, true);
			else if (m_PreviousDirection == flgin::MovementDirection::Up) m_pSpriteComponent->SetFlips(false, !m_pSpriteComponent->GetFlippedVertical());
			break;
		case flgin::MovementDirection::Left:
			m_pSpriteComponent->SetRotation(180.f);
			m_pSpriteComponent->SetFlips(false, true);
			break;
		case flgin::MovementDirection::Right:
			m_pSpriteComponent->SetRotation(0.f);
			m_pSpriteComponent->SetFlips(false, false);
			break;
		}
		m_PreviousDirection = m_pGridMover->GetMovementDirection();
		return false;
	}
	IdleState* idle{ new IdleState{} };
	idle->SetAttachedSprite(m_pSpriteComponent);
	idle->SetPlayer(m_pPlayer);
	m_pTargetState = idle;
	return true;
}
