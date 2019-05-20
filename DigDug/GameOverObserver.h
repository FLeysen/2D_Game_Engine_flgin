#pragma once
#include "Observer.h"

namespace flgin
{
	class SpriteComponent;
}

namespace DigDug
{
	class Game;
	class GameOverObserver : public flgin::Observer
	{
	public:
		GameOverObserver(Game* pGame) : Observer(), m_pGame{ pGame } {}
		void Notify(Event event, const flgin::Subject* const subject) override;
		~GameOverObserver() = default;

	private:
		Game* m_pGame;
	};
}
