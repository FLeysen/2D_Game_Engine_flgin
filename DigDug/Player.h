#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace DigDug
{
	class Player final : public flgin::BaseComponent, public flgin::Subject
	{
	public:
		Player(flgin::GameObject* pOwnerObject) : BaseComponent(pOwnerObject), Subject(), m_IsAngry{ false } {}
		~Player() = default;
		bool IsAngry() const { return m_IsAngry; }
		void SetAngry(bool angry) { m_IsAngry = angry; }
		void Update() {}
		void LoseLife();

	private:
		bool m_IsAngry;
	};
}

