#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "Time.h"
#include "TextComponent.h"
#include <string>
#include "Invoker.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)


flgin::FPSComponent::FPSComponent(GameObject* const ownerObject, const float timeBetweenUpdates)
	: BaseComponent(ownerObject)
	, m_TimeBetweenUpdates{ timeBetweenUpdates }
	, m_ElapsedTime{ 0.0f }
	, m_ElapsedFrames{ 0 }
{}

void flgin::FPSComponent::Update()
{
	m_ElapsedTime += Time::GetInstance().GetDeltaTime();
	if (m_ElapsedTime > m_TimeBetweenUpdates)
	{
		int fps{ int(m_ElapsedFrames / m_ElapsedTime) };
		m_ElapsedTime = 0.0f;
		TextComponent* textComponent{ m_pOwnerObject->GetComponent<TextComponent>() };

		if (textComponent != nullptr)
			textComponent->SetText(std::to_string(fps) + " FPS");
		else
			Logger::GetInstance().Log(StatusCode{ StatusCode::Status::WARNING, "FPSComponent could not find an attached TextComponent.", this });
		
		m_ElapsedFrames = 0;
	}
	++m_ElapsedFrames;
}