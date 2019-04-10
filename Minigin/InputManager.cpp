#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"
#include "SDL.h"
#include "InputComponent.h"

flgin::InputManager::InputManager()
	: m_pCommands{}
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
		else if (e.type == SDL_KEYDOWN)
		{
			for (InputComponent* player : m_pPlayers)
			{
				if (player)
				{
					if (!player->KeyboardButtonDown(e.key.keysym.sym))
						return false;
				}
			}
		}
		else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if (e.cbutton.which > m_pPlayers.size() - 1) continue;
			if (!m_pPlayers[e.cbutton.which]) continue;
			if (!m_pPlayers[e.cbutton.which]->ControllerButtonDown(e.cbutton.button)) return false;
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
}

void flgin::InputManager::RemovePlayer(InputComponent* player)
{
	for (InputComponent* pl : m_pPlayers)
	{
		if (pl == player)
			Logger::GetInstance().SafeDelete(pl);
	}
}