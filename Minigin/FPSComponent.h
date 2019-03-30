#pragma once
#include "BaseComponent.h"
class FPSComponent final : public BaseComponent
{
public:
	FPSComponent(dae::GameObject* const ownerObject, const float timeBetweenUpdates);
	~FPSComponent() = default;
	void Update() override;

private:
	int m_ElapsedFrames;
	float m_TimeBetweenUpdates;
	float m_ElapsedTime;
};