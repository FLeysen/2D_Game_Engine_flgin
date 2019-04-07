#include "MiniginPCH.h"
#include "InputManager.h"
#include <winerror.h>
#pragma comment(lib, "XInput.lib")
#include "Command.h"
#include "SDL.h"

flgin::InputManager::InputManager()
	: m_pCommands{}
	, m_InputState{}
{}

flgin::InputManager::~InputManager()
{
	Logger& logger{ Logger::GetInstance() };
	for (Command* command : m_pCommands)
		logger.SafeDelete(command);
}

bool flgin::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	return true;
	//TODO: Update this
	/*
	DWORD packetNumber{ m_InputState.dwPacketNumber };
	if (XInputGetState(0, &m_InputState) == ERROR_DEVICE_NOT_CONNECTED)
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::WARNING, "ProcessInput: Controller not found.", this });
		return true;
	}
	if (m_InputState.dwPacketNumber == packetNumber) return true;
	
	for (Command* const command : m_pCommands)
	{
		if (command)
		{
			if (m_InputState.Gamepad.wButtons & command->GetMapping())
				command->Execute();
		}
	}
	return true;
	*/
}

bool flgin::InputManager::IsPressed(WORD button) const
{
	return m_InputState.Gamepad.wButtons & button;
}

void flgin::InputManager::AddCommand(Command* const command)
{
	m_pCommands.push_back(command);
}