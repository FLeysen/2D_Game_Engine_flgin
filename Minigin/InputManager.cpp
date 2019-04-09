#include "MiniginPCH.h"
#include "InputManager.h"
#pragma comment(lib, "XInput.lib")
#include "Command.h"
#include "SDL.h"
#include "InputComponent.h"

flgin::InputManager::InputManager()
	: m_pCommands{}
	, m_InputStates{}
{}

flgin::InputManager::~InputManager()
{
	Logger& logger{ Logger::GetInstance() };
	for (Command* command : m_pCommands)
		logger.SafeDelete(command);
}

bool flgin::InputManager::ProcessInput()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
		{
			return false;
		}
	}
	for (size_t i{}, playerAmount{ m_pPlayers.size() }; i < playerAmount; ++i)
	{
		if (m_pPlayers[i])
		{
			XInputGetState(static_cast<DWORD>(i), &m_InputStates[i]);
			if (!m_pPlayers[i]->ProcessInput(m_InputStates[i].Gamepad.wButtons))
				return false;
		}
	}

	return true;	
}

void flgin::InputManager::AddCommand(Command* const command)
{
	m_pCommands.push_back(command);
}

void flgin::InputManager::AddPlayer(InputComponent* player)
{
	m_pPlayers.push_back(player);
	m_InputStates.push_back({});
}

void flgin::InputManager::RemovePlayer(InputComponent* player)
{
	for (InputComponent* pl : m_pPlayers)
	{
		if (pl == player)
			Logger::GetInstance().SafeDelete(pl);
	}
}