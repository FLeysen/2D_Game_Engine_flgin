#include "pch.h"
#include "DigDugStates.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "ResourceManager.h"
#include "TextLocalizer.h"

DigDug::IdleState DigDug::DigDugState::m_IdleState{};
DigDug::AngryState DigDug::DigDugState::m_AngryState{};
DigDug::Player* DigDug::DigDugState::m_pPlayer{};
flgin::SpriteComponent* DigDug::DigDugState::m_pSpriteComponent{ nullptr };

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
		m_pTargetState = &m_AngryState;
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
		m_pTargetState = &m_IdleState;
		return true;
	}
	return false;
}

