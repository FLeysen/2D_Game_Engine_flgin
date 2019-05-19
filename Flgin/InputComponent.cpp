#include "FlginPCH.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Command.h"

flgin::InputComponent::InputComponent(GameObject* const ownerObject)
	: BaseComponent(ownerObject)
	, m_ControllerMappings{}
	, m_KeyboardMappings{}
	, m_ShouldClear{ false }
{}

void flgin::InputComponent::AddControllerMapping(UINT8 key, Command* command)
{
	m_ControllerMappings.emplace(key, command);
	//	FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::AddKeyboardMapping(int key, Command* command)
{
	m_KeyboardMappings.emplace(key, command);
	//FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::Update()
{
	if (m_ShouldClear)
	{
		m_ShouldClear = false;
		m_KeyboardMappings = m_BufferedKeyboardMappings;
		m_ControllerMappings = m_BufferedControllerMappings;
		m_BufferedKeyboardMappings.clear();
		m_BufferedControllerMappings.clear();
	}
}

void flgin::InputComponent::ProcessKeyboardKey(int key, bool isKeyUp)
{
	for (const std::pair<int, Command*>& mapping : m_KeyboardMappings)
	{
		if (key == mapping.first)
			mapping.second->Execute(*m_pOwnerObject, isKeyUp);
	}
}

void flgin::InputComponent::ProcessControllerKey(UINT8 key, bool isKeyUp)
{
	for (const std::pair<UINT8, Command*>& mapping : m_ControllerMappings)
	{
		if (key == mapping.first)
			mapping.second->Execute(*m_pOwnerObject, isKeyUp);
	}
}

void flgin::InputComponent::BufferedAddControllerMapping(UINT8 key, Command * command)
{
	m_BufferedControllerMappings.emplace(key, command);
}

void flgin::InputComponent::BufferedAddKeyboardMapping(int key, Command * command)
{
	m_BufferedKeyboardMappings.emplace(key, command);
}

void flgin::InputComponent::BufferedClear()
{
	m_ShouldClear = true;
}