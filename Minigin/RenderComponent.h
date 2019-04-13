#pragma once
#include "BaseComponent.h"
namespace flgin
{
	class Texture2D;
	class Scene;
	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent(GameObject* const ownerObject, Scene& scene);
		virtual ~RenderComponent();
		virtual void Render() const;
		virtual void Update() override;

		void SetTexture(Texture2D* const newTexture);
		void SetPositionOffset(float x, float y);
		void SetDimensions(float width, float height);
		void ResetDimensions();

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;
		RenderComponent& operator=(RenderComponent&&) = delete;

	protected:
		float m_XOffset;
		float m_YOffset;
		float m_Width;
		float m_Height;
		Texture2D* m_pTexture;
	};
}
