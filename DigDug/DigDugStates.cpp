#include "pch.h"
#include "DigDugStates.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "ResourceManager.h"
#include "TextLocalizer.h"

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
}

bool DigDug::IdleState::Update()
{
	if (m_pPlayer->IsAngry())
	{
		//TODO: REMOVE TEST CODE 
		m_pPlayer->ChangeLives(-1);
		m_pPlayer->ChangeScore(rand() % 3000);

		AngryState* angryState{ new AngryState{} };
		angryState->SetAttachedSprite(m_pSpriteComponent);
		angryState->SetPlayer(m_pPlayer);
		m_pTargetState = angryState;
		return true;
	}
	return false;
}

DigDug::AngryState::AngryState()
	: DigDugState()
{}

void DigDug::AngryState::Enter()
{
	m_pSpriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texAngryPlayer")), true);
}

bool DigDug::AngryState::Update()
{
	if (!m_pPlayer->IsAngry())
	{
		IdleState* idleState{ new IdleState{} };
		idleState->SetAttachedSprite(m_pSpriteComponent);
		idleState->SetPlayer(m_pPlayer);
		m_pTargetState = idleState;
		return true;
	}
	return false;
}

