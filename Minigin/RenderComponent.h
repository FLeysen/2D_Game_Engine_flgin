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

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;
		RenderComponent& operator=(RenderComponent&&) = delete;

	private:
		Texture2D* m_pTexture;
	};
}
