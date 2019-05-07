#pragma once
#include "BaseComponent.h"
namespace flgin
{
	class Texture2D;
	class Scene;
	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent(GameObject* const ownerObject, Scene& scene, unsigned int renderLayer = 2);
		virtual ~RenderComponent() = default;
		virtual void Render() const;
		virtual void Update() override;

		void SetTexture(std::shared_ptr<Texture2D>& newTexture);
		void SetTexture(std::shared_ptr<Texture2D>&& newTexture);
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
		std::shared_ptr<Texture2D> m_spTexture;
	};
}
