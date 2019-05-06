#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class ColliderComponent final : public BaseComponent
	{
	public:
		ColliderComponent(GameObject* pOwnerObject, float width, float height);
		ColliderComponent(GameObject* pOwnerObject, float xOffset, float yOffset, float width, float height);
		
		void SetWidth(float width) { m_Width = width; }
		void SetHeight(float height) { m_Height = height; }
		void SetXOffset(float value) { m_XOffset = value; }
		void SetYOffset(float value) { m_YOffset = value; }

		bool IsColliding(const ColliderComponent& other);

	private:
		float m_XOffset;
		float m_YOffset;
		float m_Width;
		float m_Height;
	};
}