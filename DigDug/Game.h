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

		void SetSwappingToSingle();
		void SetSwappingToTwoPlayer();
		void SetSwappingToVerus();

	private:
		bool m_HasTwoScores;
		flgin::Flgin m_Engine;
	};
}

