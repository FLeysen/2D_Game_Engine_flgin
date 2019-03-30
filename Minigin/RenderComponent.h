#pragma once
#include "BaseComponent.h"
class Texture2D;
class RenderComponent final : public BaseComponent
{
public:
	RenderComponent(dae::GameObject* const ownerObject);
	~RenderComponent();
	void Update() override {}
	void Render() const;
	void SetTexture(dae::Texture2D* const newTexture);

	RenderComponent(const RenderComponent&) = delete;
	RenderComponent(RenderComponent&&) = delete;
	RenderComponent& operator=(const RenderComponent&) = delete;
	RenderComponent& operator=(RenderComponent&&) = delete;

private:
	dae::Texture2D* m_pTexture;
};

