#include "pch.h"
#include "Fygar.h"

DigDug::Fygar::Fygar(flgin::GameObject* pOwnerObject, flgin::GameObject* pFire)
	: BaseComponent(pOwnerObject)
	, m_pFire{ pFire }
	, m_ScoreWorth{ 200 }
	, m_IsFiring{ false }
	, m_IsDying{ false }
{
	m_pFire->SetActive(false);
}
