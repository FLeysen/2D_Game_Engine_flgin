#pragma once
#include "RenderComponent.h"
#include "SDL.h"

namespace flgin
{
	class SpriteComponent final : public RenderComponent
	{
	public:
		SpriteComponent(GameObject* const pOwnerObject, Scene& scene);
		~SpriteComponent() = default;
		void Update() override;
		void Render() const override;

		void SetSpriteInfo(unsigned int columns, unsigned int rows, float spriteWidth, float spriteHeight, float frameTime);
		void SetFlips(bool horizontal, bool vertical);
		void SetRotation(float angleInDegrees);
		void SetRotationalOffset(float xOffsetFromCenter, float yOffsetFromCenter);
		void IncrementCurrentFrame();

	private:
		unsigned int m_CurrentFrame;
		unsigned int m_Columns;
		unsigned int m_Rows;
		float m_Rotation;
		SDL_RendererFlip m_Flips;
		SDL_Point m_RotationalCenter;
		SDL_Rect m_SourceRect;
	};
}