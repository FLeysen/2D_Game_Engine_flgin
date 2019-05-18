#include "pch.h"
#include "DigDugCommands.h"
#include "Player.h"

bool DigDug::ToggleAngryCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if(!isInverseAction) m_pPlayer->SetAngry(!m_pPlayer->IsAngry());
	return true;
}