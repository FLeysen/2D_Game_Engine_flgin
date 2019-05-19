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
		void InitSinglePlayer();
		void InitTwoPlayer();
		void InitVersus();

	private:
		flgin::Flgin m_Engine;
		bool m_TwoPlayer;
		bool m_Versus;
	};
}

