#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace DigDug
{
	class Player final : public flgin::Subject, public flgin::BaseComponent
	{
	public:
		Player(flgin::GameObject* pOwnerObject) : Subject(), BaseComponent(pOwnerObject), m_Lives{ 4 }, m_IsDying{ false } {}
		~Player() = default;
		void Update() {}

		void ChangeLives(int amount);
		UINT GetLives() const { return m_Lives; }

		void SetDying(bool dying) { m_IsDying = dying; }
		bool IsDying() const { return m_IsDying; }

		void ChangeScore(int amount);
		UINT GetScore() const { return m_Score; }

	private:
		bool m_IsDying;
		UINT m_Lives;
		UINT m_Score;
	};
}

