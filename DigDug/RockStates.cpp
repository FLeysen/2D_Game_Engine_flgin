#include "pch.h"
#include "RockStates.h"
#include "FreeMover.h"
#include "ColliderComponent.h"
#include "FunctionHolder.h"
#include "MovementGrid.h"
#include "Player.h"

void DigDug::StuckState::Enter()
{
	m_pFreeMover->SetMoving(false);
	m_pCollider->SetOnCollisionFunction(new flgin::FunctionHolder<void>{ std::bind(&StuckState::CheckPlayerHit, this) });
}

void DigDug::StuckState::Exit()
{}

bool DigDug::StuckState::Update()
{
	if (!m_HitPlayerOne && !m_HitPlayerTwo) return false;

	AwaitingFallState* fallState{ new AwaitingFallState{ m_HitPlayerOne, m_HitPlayerTwo } };
	fallState->SetAttachedCollider(m_pCollider);
	fallState->SetAttachedMover(m_pFreeMover);
	fallState->SetNode(m_pNode);
	fallState->SetPlayer1Collider(m_pPlayer1Collider);
	fallState->SetPlayer2Collider(m_pPlayer2Collider);
	m_pTargetState = fallState;
	return true;
}

void DigDug::AwaitingFallState::Enter()
{
	m_pCollider->SetOnCollisionFunction(new flgin::FunctionHolder<void>{ std::bind(&AwaitingFallState::CheckPlayerHit, this) });
}

void DigDug::AwaitingFallState::Exit()
{}

bool DigDug::AwaitingFallState::Update()
{
	if (m_HitPlayerOne || m_HitPlayerTwo)
	{
		m_HitPlayerOne = false;
		m_HitPlayerTwo = false;
		return false;
	}

	FallingState* fallState{ new FallingState{ m_P1Dropped, m_P2Dropped } };
	fallState->SetAttachedCollider(m_pCollider);
	fallState->SetAttachedMover(m_pFreeMover);
	fallState->SetNode(m_pNode);
	fallState->SetPlayer1Collider(m_pPlayer1Collider);
	fallState->SetPlayer2Collider(m_pPlayer2Collider);
	m_pTargetState = fallState;
	return true;
}

void DigDug::AwaitingFallState::CheckPlayerHit()
{
	flgin::ColliderComponent* other{ m_pCollider->GetCollisionHit() };
	if (other == m_pPlayer1Collider)
		m_HitPlayerOne = true;
	else if (other == m_pPlayer2Collider)
		m_HitPlayerTwo = true;
}

void DigDug::FallingState::Enter()
{
	m_pFreeMover->SetMaxYVelocity(50.f);
	m_pFreeMover->SetMoving(true);
	m_pCollider->SetOnCollisionFunction(new flgin::FunctionHolder<void>{ std::bind(&FallingState::CheckPlayerHit, this) });
}

void DigDug::FallingState::Exit()
{}

bool DigDug::FallingState::Update()
{
	return false;
}

void DigDug::FallingState::CheckPlayerHit()
{
	flgin::ColliderComponent* other{ m_pCollider->GetCollisionHit() };
	if (other == m_pPlayer1Collider)
	{
		m_pPlayer1Collider->GetGameObject()->GetComponent<Player>()->ChangeLives(-1);
	}
	else if (other == m_pPlayer2Collider)
	{
		m_pPlayer2Collider->GetGameObject()->GetComponent<Player>()->ChangeLives(-1);
	}
	else
	{
		//TODO: INSERT ENEMY DESTRUCTION CODE HERE, SHOULD COLLIDE ONLY WITH ENEMIES AND PLAYERS
		(m_HitPlayerOne ? m_pPlayer1Collider : m_pPlayer2Collider)->GetGameObject()->GetComponent<Player>()->ChangeScore(100);
	}
	m_pCollider->GetGameObject()->SetActive(false);
}

void DigDug::StuckState::CheckPlayerHit()
{
	if (m_pNode->GetDownNode()->IsBlocked()) return;

	flgin::ColliderComponent* other{ m_pCollider->GetCollisionHit() };
	if (other == m_pPlayer1Collider)
		m_HitPlayerOne = true;
	else if (other == m_pPlayer2Collider)
		m_HitPlayerTwo = true;
}
