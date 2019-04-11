#pragma once
#include "BaseComponent.h"
namespace flgin
{
	class Texture2D;
	class Scene;
	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(GameObject* const ownerObject, Scene& scene);
		~RenderComponent();
		void Update() override {}
		void Render() const;
		void SetTexture(Texture2D* const newTexture);
		void SetPositionOffset(float x, float y);

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;
		RenderComponent& operator=(RenderComponent&&) = delete;

	private:
		float m_XOffset;
		float m_YOffset;
		Texture2D* m_pTexture;
	};
}
