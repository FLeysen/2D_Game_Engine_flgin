#pragma once
#include "Command.h"

namespace flgin
{
	class MenuComponent;
}

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

	class MenuNextCommand final : public flgin::Command 
	{
	public:
		MenuNextCommand(flgin::MenuComponent* pMenu) : Command(), m_pMenu{ pMenu }{}
		bool Execute(flgin::GameObject& attachedObject, bool isInverseAction = false) override;

	private:
		flgin::MenuComponent* m_pMenu;
	};

	class MenuPreviousCommand final : public flgin::Command
	{
	public:
		MenuPreviousCommand(flgin::MenuComponent* pMenu) : Command(), m_pMenu{ pMenu }{}
		bool Execute(flgin::GameObject& attachedObject, bool isInverseAction = false) override;

	private:
		flgin::MenuComponent* m_pMenu;
	};

	class MenuConfirmCommand final : public flgin::Command
	{
	public:
		MenuConfirmCommand(flgin::MenuComponent* pMenu) : Command(), m_pMenu{ pMenu }, m_ExecuteReturn{ true }{}
		bool Execute(flgin::GameObject& attachedObject, bool isInverseAction = false) override;
		void SetExecuteReturn(bool value) { m_ExecuteReturn = value; }

	private:
		bool m_ExecuteReturn;
		flgin::MenuComponent* m_pMenu;
	};
}