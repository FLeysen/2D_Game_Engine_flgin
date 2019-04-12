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
		GridMovementComponent(GameObject* const ownerObject, float maxVelocity, MovementGrid* pAttachedGrid, float acceptableTurnDist = 1.0f);
		void Update() override;
		void SetMoving(bool isHorizontal, bool isPositiveDirection);
		void StopMoving();
		glm::vec2 GetVelocity();

	private:
		void MoveLeft();
		void MoveRight();
		void MoveDown();
		void MoveUp();
		void ClampVelocity();

		bool m_WasStopped;
		float m_MaxVelocity;
		float m_MaxTurnDistance;
		glm::vec2 m_CurrentVelocity;
		glm::vec2 m_TargetPos;
		MovementGrid* m_pMovementGrid;
	};
}