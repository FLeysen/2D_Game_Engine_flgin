#include "pch.h"
#include "Player.h"
#include "ObserverEvents.h"
#include "SpriteComponent.h"

DigDug::Player::Player(flgin::GameObject * pOwnerObject, flgin::GameObject * pPump)
	: Subject()
	, BaseComponent(pOwnerObject)
	, m_pPump{ pPump }
	, m_Lives{ 4 }
	, m_IsDying{ false }
	, m_IsFiring{ false }
{ m_pPump->SetActive(false); }

void DigDug::Player::ChangeLives(int amount)
{
	m_Lives += amount;
	m_IsDying = true;
	Notify(ObserverEvent::LivesChanged);
}

void DigDug::Player::ChangeScore(int amount)
{
	m_Score += amount;
	Notify(ObserverEvent::ScoreChanged);
}