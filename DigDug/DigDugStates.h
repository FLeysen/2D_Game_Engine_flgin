#pragma once
#include "StateComponent.h"

namespace flgin
{
	class SpriteComponent;
	class Texture2D;
}

namespace DigDug
{
	class Player;
	class IdleState;
	class AngryState;

	class DigDugState : public flgin::State
	{
	public:
		DigDugState() : flgin::State() {}
		virtual ~DigDugState() = default;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update() = 0;


		static void SetAttachedSprite(flgin::SpriteComponent* pSprite);
		static void SetPlayer(Player* pPlayer);
		static IdleState* GetDefaultState() { return &m_IdleState; };

	protected:
		static IdleState m_IdleState;
		static AngryState m_AngryState;
		static flgin::SpriteComponent* m_pSpriteComponent;
		static Player* m_pPlayer;
	};

	class IdleState final : public DigDugState
	{
	public:
		IdleState();
		~IdleState() = default;
		void Enter() override;
		bool Update() override;
		void Exit() override {}
	};

	class AngryState final : public DigDugState
	{
	public:
		AngryState();
		~AngryState() = default;
		void Enter() override;
		void Exit() override {}
		bool Update() override;
	};
}