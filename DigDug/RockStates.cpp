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
	if (!m_pCollidedWith) return false;

	AwaitingFallState* fallState{ new AwaitingFallState{ m_pCollidedWith } };
	fallState->SetAttachedCollider(m_pCollider);
	fallState->SetAttachedMover(m_pFreeMover);
	fallState->SetNode(m_pNode);
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
	if (m_StillColliding)
	{
		m_StillColliding = false;
		return false;
	}

	FallingState* fallState{ new FallingState{ m_pCollidedWith } };
	fallState->SetAttachedCollider(m_pCollider);
	fallState->SetAttachedMover(m_pFreeMover);
	fallState->SetNode(m_pNode);
	m_pTargetState = fallState;
	return true;
}

void DigDug::AwaitingFallState::CheckPlayerHit()
{
	flgin::ColliderComponent* other{ m_pCollider->GetCollisionHit() };
	if (other->GetGameObject()->CompareTag("Player"))
		m_StillColliding = true;
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
	if (m_pCollider->GetGameObject()->GetPosition().y < 500.f) 
		return false;
	m_pCollider->GetGameObject()->SetActive(false);
	return false;
}

void DigDug::FallingState::CheckPlayerHit()
{
	flgin::ColliderComponent* other{ m_pCollider->GetCollisionHit() };
	if (other->GetGameObject()->CompareTag("Player"))
		other->GetGameObject()->GetComponent<Player>()->ChangeLives(-1);
	else
	{
		//TODO: INSERT ENEMY DESTRUCTION CODE HERE, SHOULD COLLIDE ONLY WITH ENEMIES AND PLAYERS
	}
	m_pCollider->GetGameObject()->SetActive(false);
}

void DigDug::StuckState::CheckPlayerHit()
{
	if (m_pNode->GetDownNode()->IsBlocked()) return;

	flgin::ColliderComponent* other{ m_pCollider->GetCollisionHit() };
	if (other->GetGameObject()->CompareTag("Player"))
		m_pCollidedWith = other;
}
