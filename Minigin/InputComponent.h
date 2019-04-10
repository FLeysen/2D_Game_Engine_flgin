#pragma once
#include "BaseComponent.h"
#include <map>
namespace flgin
{
	class Command;
	class InputComponent : public BaseComponent
	{
	public:
		InputComponent(GameObject* const ownerObject);
		~InputComponent();
		void AddControllerMapping(UINT8 key, Command* command);
		void AddKeyboardMapping(int key, Command* command);
		void Update() override;
		bool KeyboardButtonDown(int key);
		bool ControllerButtonDown(UINT8 key);

	private:
		std::map<int, Command*> m_KeyboardMappings;
		std::map<UINT8, Command*> m_ControllerMappings;
	};
}