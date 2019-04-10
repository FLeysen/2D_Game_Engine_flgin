#include "MiniginPCH.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Command.h"

flgin::InputComponent::InputComponent(GameObject* const ownerObject)
	: BaseComponent(ownerObject)
	, m_ControllerMappings{}
{
	InputManager::GetInstance().AddPlayer(this);
}

flgin::InputComponent::~InputComponent()
{
	InputManager::GetInstance().RemovePlayer(this);
}

void flgin::InputComponent::AddControllerMapping(UINT8 key, Command* command)
{
	if (!m_ControllerMappings.emplace(key, command).second)
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::AddKeyboardMapping(int key, Command* command)
{
	if (!m_KeyboardMappings.emplace(key, command).second)
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::Update()
{}

bool flgin::InputComponent::KeyboardButtonDown(int key)
{
	for (const std::pair<int, Command*>& mapping : m_KeyboardMappings)
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

bool flgin::InputComponent::ControllerButtonDown(UINT8 key)
{
	for (const std::pair<UINT8, Command*>& mapping : m_ControllerMappings)
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