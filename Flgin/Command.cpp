#include "FlginPCH.h"
#include "Command.h"
#include "InputManager.h"

flgin::Command::Command()
{
	InputManager::GetInstance().AddCommand(this);
}
