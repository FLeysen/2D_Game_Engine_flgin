#pragma once
#include "MovementGrid.h"
#include "GridMovementComponent.h"

namespace flgin
{
	class PathfinderComponent final : public GridMovementComponent
	{
	public:
		PathfinderComponent(GameObject* const pOwnerObject, float maxVelocity, MovementGrid* pAttachedGrid, float acceptableTurnDist = 1.0f);
		void Update() override;
		void SetTarget(float x, float y);
		void SetActive(bool isActive);

	private:
		void FindPath();
		void SetVelocities();

		bool m_TargetChanged;
		bool m_IsActive;
		glm::vec2 m_Target;
		MovementGrid* m_pGrid;
		std::vector<GridNode const*> m_Path;
	};
}