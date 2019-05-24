#pragma once
#include "StateComponent.h"

namespace flgin
{
	class SpriteComponent;
	class Texture2D;
	class GridMovementComponent;
	enum class MovementDirection : UINT8;
}

namespace DigDug
{
	class Player;
	class IdleState;
	class AngryState;

	class DigDugState : public flgin::State
	{
	public:
		DigDugState() : flgin::State(), m_pSpriteComponent{}, m_pPlayer{} {}
		virtual ~DigDugState() = default;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update() = 0;

		void SetAttachedSprite(flgin::SpriteComponent* pSprite);
		void SetPlayer(Player* pPlayer);

	protected:
		flgin::SpriteComponent* m_pSpriteComponent;
		Player* m_pPlayer;
	};

	class IdleState final : public DigDugState
	{
	public:
		IdleState();
		~IdleState() = default;
		void Enter() override;
		bool Update() override;
		void Exit() override {}

	private:
		flgin::GridMovementComponent* m_pGridMover;
	};

	class MovingState final : public DigDugState
	{
	public:
		MovingState() : DigDugState() {}
		~MovingState() = default;
		void Enter() override;
		void Exit() override {}
		bool Update() override;

	private:
		flgin::MovementDirection m_PreviousDirection;
		flgin::GridMovementComponent* m_pGridMover;
	};
}