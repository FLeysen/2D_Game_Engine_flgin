#include "pch.h"
#include "Player.h"
#include "ObserverEvents.h"

void DigDug::Player::LoseLife()
{
	Notify(ObserverEvent::LifeLost);
}