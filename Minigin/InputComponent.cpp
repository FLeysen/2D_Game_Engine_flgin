#include "MiniginPCH.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Command.h"

flgin::InputComponent::InputComponent(GameObject* const ownerObject)
	: BaseComponent(ownerObject)
	, m_Mappings{}
{
	InputManager::GetInstance().AddPlayer(this);
}

flgin::InputComponent::~InputComponent()
{
	InputManager::GetInstance().RemovePlayer(this);
}

void flgin::InputComponent::AddMapping(UINT8 key, Command* command)
{
	if (!m_Mappings.emplace(key, command).second)
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::Update()
{}

bool flgin::InputComponent::ProcessInput(UINT8 key)
{
	for (const std::pair<WORD, Command*>& mapping : m_Mappings)
	{
		if (key == mapping.first)
		{
			if (!mapping.second->Execute(*m_pOwnerObject))
				return false;
			return true;
		}
	}
	return true;
}