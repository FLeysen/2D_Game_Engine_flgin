#pragma once
#include "Command.h"

namespace flgin
{
	class QuitCommand final : public Command
	{
	public:
		bool Execute(GameObject& attachedObject) override;
	};

	class RumbleCommand final : public Command
	{
	public:
		RumbleCommand(DWORD userId) : Command(), m_UserID{ userId }, m_IsRumbling{ false }{}
		bool Execute(GameObject& attachedObject) override;

	private:
		DWORD m_UserID;
		bool m_IsRumbling;
	};
}