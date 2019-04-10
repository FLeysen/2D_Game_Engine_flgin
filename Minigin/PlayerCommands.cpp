#include "MiniginPCH.h"
#include "PlayerCommands.h"
#include "GameObject.h"
#include <Xinput.h>

bool flgin::QuitCommand::Execute(GameObject&, bool)
{
	return false;
}

bool flgin::RumbleCommand::Execute(GameObject&, bool isInverseAction)
{
	if (!isInverseAction)
	{
		_XINPUT_VIBRATION vib{};
		WORD rumbleValue{ 0 };
		m_IsRumbling = !m_IsRumbling;
		if (m_IsRumbling)
		{
			rumbleValue = 65535;
			rumbleValue = 65535;
		}
		vib.wLeftMotorSpeed = rumbleValue;
		vib.wRightMotorSpeed = rumbleValue;
		XInputSetState(m_UserID, &vib);
	}
	return true;
}
