#include "pch.h"
#include "Player.h"
#include "ObserverEvents.h"

void DigDug::Player::ChangeLives(int amount)
{
	m_Lives += amount;
	Notify(ObserverEvent::LivesChanged);
}

void DigDug::Player::ChangeScore(int amount)
{
	m_Score += amount;
	Notify(ObserverEvent::ScoreChanged);
}
