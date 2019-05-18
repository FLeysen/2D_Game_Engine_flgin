#pragma once
#include "Flgin.h"
namespace DigDug
{
	class Game
	{
	public:
		Game();
		~Game();
		void Run();
		void InitTestScene();
		void InitMenuScene();
		void InitPlayers();

	private:
		flgin::Flgin m_Engine;
	};
}

