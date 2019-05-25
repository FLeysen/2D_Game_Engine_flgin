#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class Fygar final : public flgin::BaseComponent
	{
	public:
		Fygar(flgin::GameObject* pOwnerObject, flgin::GameObject* pFire);
		~Fygar() = default;
		void Update() override {}

		void SetFiring(bool firing) { m_IsFiring = firing; }
		bool IsFiring() const { return m_IsFiring; }

		void Die() { m_IsDying = true; }
		bool IsDying() const { return m_IsDying; }

		flgin::GameObject* GetFire() const { return m_pFire; }

	private:
		bool m_IsFiring;
		bool m_IsDying;
		UINT m_ScoreWorth;
		flgin::GameObject* m_pFire;
	};
}

