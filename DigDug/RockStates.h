#pragma once
#include "StateComponent.h"

namespace flgin
{
	class FreeMover;
	class ColliderComponent;
	class GridNode;
}

namespace DigDug
{
	class RockState : public flgin::State
	{
	public:
		RockState() : flgin::State(), m_pFreeMover{}, m_pCollider{}, m_HitPlayerOne{ false }, m_HitPlayerTwo{ false }, m_pNode{}, m_pPlayer1Collider{}, m_pPlayer2Collider{} {}
		virtual ~RockState() = default;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update() = 0;
		virtual void CheckPlayerHit() = 0;

		void SetAttachedMover(flgin::FreeMover* pMover) { m_pFreeMover = pMover; }
		void SetAttachedCollider(flgin::ColliderComponent* pCollider) { m_pCollider = pCollider; }
		void SetPlayer1Collider(flgin::ColliderComponent* pCollider) { m_pPlayer1Collider = pCollider; }
		void SetPlayer2Collider(flgin::ColliderComponent* pCollider) { m_pPlayer2Collider = pCollider; }
		void SetNode(flgin::GridNode* node) { m_pNode = node; }

	protected:
		bool m_HitPlayerOne;
		bool m_HitPlayerTwo;
		flgin::FreeMover* m_pFreeMover;
		flgin::GridNode* m_pNode;
		flgin::ColliderComponent* m_pCollider;
		flgin::ColliderComponent* m_pPlayer1Collider;
		flgin::ColliderComponent* m_pPlayer2Collider;
	};

	class StuckState final : public RockState
	{
	public:
		StuckState() : RockState() {}
		~StuckState() = default;
		
		void Enter() override;
		void Exit() override;
		bool Update() override;
		void CheckPlayerHit() override;
	};

	class AwaitingFallState final : public RockState
	{
	public:
		AwaitingFallState(bool p1Hit, bool p2Hit) : RockState() { m_P1Dropped = p1Hit; m_P2Dropped = p2Hit; m_HitPlayerOne = false; m_HitPlayerTwo = false; }
		~AwaitingFallState() = default;

		void Enter() override;
		void Exit() override;
		bool Update() override;
		void CheckPlayerHit() override;

	private:
		bool m_P1Dropped;
		bool m_P2Dropped;
	};

	class FallingState final : public RockState
	{
	public:
		FallingState(bool p1Hit, bool p2Hit) : RockState() { m_HitPlayerOne = p1Hit; m_HitPlayerTwo = p2Hit; }
		~FallingState() = default;

		void Enter() override;
		void Exit() override;
		bool Update() override;
		void CheckPlayerHit() override;
	};
}