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
		void InitGameScene();
		void InitMenuScene();
		void InitSinglePlayer();
		void InitTwoPlayer();
		void InitVersus();
		void InitEndScene();

	private:
		flgin::Flgin m_Engine;
	};
}

