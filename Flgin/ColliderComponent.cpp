#include "FlginPCH.h"
#include "ColliderComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

flgin::ColliderComponent::ColliderComponent(GameObject* pOwnerObject, float width, float height)
	: ColliderComponent(pOwnerObject, 0, 0, width, height)
{}

flgin::ColliderComponent::ColliderComponent(GameObject* pOwnerObject, float xOffset, float yOffset, float width, float height)
	: BaseComponent(pOwnerObject)
	, m_XOffset{ xOffset }
	, m_YOffset{ yOffset }
	, m_Width{ width }
	, m_Height{ height }
{}

bool flgin::ColliderComponent::IsColliding(const ColliderComponent& other)
{
	glm::vec2 pos{ m_pOwnerObject->GetPosition() };
	glm::vec2 otherPos{ other.m_pOwnerObject->GetPosition() };
	pos.x += m_XOffset;
	pos.y += m_YOffset;
	otherPos.x += other.m_XOffset;
	otherPos.y += other.m_YOffset;

	return (pos.x < otherPos.x + other.m_Width  && pos.x + m_Width > otherPos.x
		 && pos.y < otherPos.y + other.m_Height && pos.y + m_Height > otherPos.y);
}