#include "pch.h"
#include "DigDugCommands.h"
#include "Player.h"
#include "MenuComponent.h"

bool DigDug::ToggleAngryCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if(!isInverseAction) m_pPlayer->SetAngry(!m_pPlayer->IsAngry());
	return true;
}

bool DigDug::MenuPreviousCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if (!isInverseAction) m_pMenu->SelectPrevious();
	return true;
}

bool DigDug::MenuNextCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if (!isInverseAction) m_pMenu->SelectNext();
	return true;
}

bool DigDug::MenuConfirmCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if (!isInverseAction) m_pMenu->PressCurrentButton();
	return m_ExecuteReturn;
}
