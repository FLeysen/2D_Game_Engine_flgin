#include "pch.h"
#include "GameOverObserver.h"
#include "ObserverEvents.h"
#include "Player.h"
#include "Game.h"
#include "Invoker.h"

void DigDug::GameOverObserver::Notify(Event event, const flgin::Subject* const subject)
{
	if (event != ObserverEvent::LivesChanged) return;

	const Player* const player{ static_cast<const Player* const>(subject) };
	if (player->GetLives() == 0)
	{
		FInvoker.AddInvoke(new flgin::InvokeHolder<void>{ this, 0.001f, [this]() { m_pGame->InitEndScene(); } });
	}
}
