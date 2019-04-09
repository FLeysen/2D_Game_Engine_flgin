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
		void AddMapping(WORD key, Command* command);
		void Update() override;
		bool ProcessInput(WORD buttonsPressed);

	private:
		std::map<WORD, Command*> m_Mappings;
	};
}