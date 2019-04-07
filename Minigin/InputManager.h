#pragma once
#include <Xinput.h>
#include <vector>
#include "Singleton.h"

class Command;
namespace flgin
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		bool IsPressed(WORD button) const;
		void AddCommand(Command* const command);
		
		template <class T>
		void RemoveCommand()
		{
			const type_info& typeInfo{ typeid(T) };
			for (size_t i{}, commandAmount{ m_pCommands.size() }; i < commandAmount; ++i)
			{
				if (m_pCommands[i])
				{
					if (typeid(*m_pCommands[i]) == typeInfo)
					{
						delete m_pCommands[i];
						m_pCommands[i] = nullptr;
					}
				}
			}
		}

		template<class T>
		void UpdateButtonMapping(WORD newMapping)
		{
			const type_info& typeInfo{ typeid(T) };
			for (Command* const command : m_pCommands)
			{
				if (command)
				{
					if (typeid(*command) == typeInfo)
						command->SetMapping(newMapping);
				}
			}
		}

	private:
		XINPUT_STATE m_InputState;
		std::vector<Command*> m_pCommands;
	};
}