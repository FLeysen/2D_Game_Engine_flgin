#pragma once
#include "StateComponent.h"

namespace flgin
{
	class SpriteComponent;
	class GridMovementComponent;
}

namespace DigDug
{
	class Fygar;

	class FygarState : public flgin::State
	{
	public:
		FygarState() : flgin::State(), m_pFygar{}, m_pSprite{} {}
		virtual ~FygarState() = default;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update() = 0;

		void SetFygar(Fygar* pFygar);
		void SetSprite(flgin::SpriteComponent* pSprite);
		void SetMover(flgin::GridMovementComponent* pMover);

	protected:
		Fygar* m_pFygar;
		flgin::SpriteComponent* m_pSprite;
		flgin::GridMovementComponent* m_pMover;
	};

	class FygarIdleState final : public FygarState
	{
	public:
		FygarIdleState() : FygarState() {}
		~FygarIdleState() = default;
		void Enter() override;
		void Exit() override;
		bool Update() override;
	};

	class FygarFiringState final : public FygarState
	{
	public:
		FygarFiringState() : FygarState() {}
		~FygarFiringState() = default;
		void Enter() override;
		bool Update() override;
		void Exit() override;
	};

	class FygarDyingState final : public FygarState
	{
	public:
		FygarDyingState() : FygarState() {}
		~FygarDyingState() = default;
		void Enter() override;
		bool Update() override;
		void Exit() override;
	};
};