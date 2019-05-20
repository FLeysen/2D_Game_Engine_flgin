#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace DigDug
{
	class Player final : public flgin::Subject, public flgin::BaseComponent
	{
	public:
		Player(flgin::GameObject* pOwnerObject) : Subject(), BaseComponent(pOwnerObject), m_IsAngry{ false }, m_Lives{ 4 } {}
		~Player() = default;
		bool IsAngry() const { return m_IsAngry; }
		void SetAngry(bool angry) { m_IsAngry = angry; }
		void Update() {}

		void ChangeLives(int amount);
		UINT GetLives() const { return m_Lives; }
		
		void ChangeScore(int amount);
		UINT GetScore() const { return m_Score; }

	private:
		bool m_IsAngry;
		UINT m_Lives;
		UINT m_Score;
	};
}

