#include "pch.h"
#include "LivesObserver.h"
#include "ObserverEvents.h"
#include "SpriteComponent.h"

DigDug::LivesObserver::LivesObserver(unsigned int lives, flgin::SpriteComponent* pSprite)
	: Observer()
	, m_Lives{ lives }
	, m_InitLives{ lives }
	, m_SizePerLife{}
	, m_pLivesSprite{ pSprite }
{
	m_pLivesSprite->SetSpriteInfo(m_Lives, 1, m_pLivesSprite->GetWidth(), m_pLivesSprite->GetHeight(), 0.f, false);
	m_SizePerLife = m_pLivesSprite->GetWidth() / m_Lives;
}

void DigDug::LivesObserver::Notify(Event event, const flgin::Subject* const)
{
	--m_Lives;
	if (event == ObserverEvent::LifeLost)
	{
		m_pLivesSprite->SetSpriteInfo(m_InitLives, 1, m_SizePerLife * m_Lives, m_pLivesSprite->GetHeight(), 0.f, false);
		m_pLivesSprite->SetDimensions(m_SizePerLife * m_Lives, m_pLivesSprite->GetHeight());
	}
}
