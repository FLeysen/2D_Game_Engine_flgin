#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class GridMovementComponent;
	class GridNode;
	class DirectionalGridMove;
}

namespace DigDug
{
	class Fygar;

	class FygarController final : public flgin::BaseComponent
	{
	public:
		FygarController(flgin::GameObject* ownerObject, Fygar* fygar, flgin::GridMovementComponent* mover);
		~FygarController() = default;
		void Update() override;

	private:
		Fygar* m_pFygar;
		flgin::GridMovementComponent* m_pMover;
		flgin::GridNode* m_pLastNode;
		flgin::GridNode* m_pTargetNode;
		flgin::DirectionalGridMove* m_pLeftMove;
		flgin::DirectionalGridMove* m_pRightMove;
		flgin::DirectionalGridMove* m_pUpMove;
		flgin::DirectionalGridMove* m_pDownMove;
	};
}

