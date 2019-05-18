#pragma once
#include "Command.h"

namespace DigDug
{
	class Player;
	class ToggleAngryCommand final : public flgin::Command
	{
	public:
		ToggleAngryCommand(Player* pPlayer) : Command(), m_pPlayer{ pPlayer } {}
		bool Execute(flgin::GameObject& attachedObject, bool isInverseAction = false) override;

	private:
		Player* m_pPlayer;
	};
}