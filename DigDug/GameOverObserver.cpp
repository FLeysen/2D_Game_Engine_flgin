#include "pch.h"
#include "GameOverObserver.h"
#include "ObserverEvents.h"
#include "Player.h"
#include "Game.h"
#include "SceneManager.h"
#include "FunctionHolder.h"

void DigDug::GameOverObserver::Notify(Event event, const flgin::Subject* const subject)
{
	if (event != ObserverEvent::LivesChanged) return;

	const Player* const player{ static_cast<const Player* const>(subject) };
	if (player->GetLives() == 0)
		FSceneManager.SwapScene(new flgin::FunctionHolder<void>{ std::bind(&Game::InitEndScene, m_pGame) });
}
