#include "pch.h"
#include "DigDugCommands.h"
#include "Player.h"
#include "MenuComponent.h"
#include "InputManager.h"

void DigDug::ToggleAngryCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if(!isInverseAction) m_pPlayer->SetAngry(!m_pPlayer->IsAngry());
}

void DigDug::MenuPreviousCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if (!isInverseAction) m_pMenu->SelectPrevious();
}

void DigDug::MenuNextCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if (!isInverseAction) m_pMenu->SelectNext();
}

void DigDug::MenuConfirmCommand::Execute(flgin::GameObject&, bool isInverseAction)
{
	if (!isInverseAction) m_pMenu->PressCurrentButton();
}
