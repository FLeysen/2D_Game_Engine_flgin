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
	, m_TimeBetweenUpdates(timeBetweenUpdates)
	, m_ElapsedTime{ 0.0f }
	, m_ElapsedFrames{ 0 }
{
	//auto setParentPosition{ [&](float x, float y) { m_pOwnerObject->SetPosition(x,y); } };
	//FunctionHolderBase* test{ new FunctionHolder<void, float, float>{3.0f, setParentPosition, 25.0f, 25.0f} };
	//Invoker::GetInstance().AddInvoke(test);

	auto moveParentPosition{ [&]()
	{
		glm::vec2 pos{ m_pOwnerObject->GetPosition() };
		pos.x += 5.0f;
		pos.y += 5.0f;
		m_pOwnerObject->SetPosition(pos.x, pos.y);
	} };
	FunctionHolderBase* test{ new FunctionHolder<void>{0.5f, moveParentPosition} };
	test->SetRepeating(true);
	Invoker::GetInstance().AddInvoke(test);
}

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