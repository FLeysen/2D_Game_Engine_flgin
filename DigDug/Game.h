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

	private:
		flgin::Flgin m_Engine;
	};
}

