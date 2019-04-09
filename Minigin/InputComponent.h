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
		void AddMapping(UINT8 key, Command* command);
		void Update() override;
		bool ProcessInput(UINT8 key);

	private:
		std::map<UINT8, Command*> m_Mappings;
	};
}