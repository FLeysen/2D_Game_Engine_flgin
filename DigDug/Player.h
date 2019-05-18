#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class Player final : public flgin::BaseComponent
	{
	public:
		Player(flgin::GameObject* pOwnerObject) : BaseComponent(pOwnerObject), m_IsAngry{ false } {}
		~Player() = default;
		bool IsAngry() const { return m_IsAngry; }
		void SetAngry(bool angry) { m_IsAngry = angry; }
		void Update() {}

	private:
		bool m_IsAngry;
	};
}

