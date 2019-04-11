#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace flgin
{
	class MovementGrid;
	class GridMovementComponent : public BaseComponent
	{
	public:
		GridMovementComponent(GameObject* const ownerObject, float maxVelocity, MovementGrid* pAttachedGrid)
			: BaseComponent(ownerObject)
			, m_WasStopped{ false }
			, m_CurrentVelocity{ 0,0 }
			, m_MaxVelocity{ maxVelocity }
			, m_pMovementGrid{ pAttachedGrid }
		{}
		void Update() override 
		{
			m_WasStopped = false;
		}
		void SetMoving(bool isHorizontal, bool isPositiveDirection) 
		{
			StopMoving();
			(isHorizontal ? m_CurrentVelocity.x : m_CurrentVelocity.y)
				= m_MaxVelocity * (isPositiveDirection ? 1 : -1);
		}
		void StopMoving()
		{
			if (!m_WasStopped) m_CurrentVelocity = { 0,0 };
			m_WasStopped = true;
		}
		glm::vec2 GetVelocity() 
		{ 
			return m_CurrentVelocity; 
		}

	private:
		bool m_WasStopped;
		float m_MaxVelocity;
		glm::vec2 m_CurrentVelocity;
		MovementGrid* m_pMovementGrid;
	};
}