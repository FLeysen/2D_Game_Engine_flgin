#include "FlginPCH.h"
#include "ScoreObserver.h"
#include "Subject.h"

void flgin::ScoreObserver::Notify(Event event, const Subject* subject)
{
	if (event == Event::ScoreAwarded)
	{
		const TestEnemyWithScore* enemy{ reinterpret_cast<const TestEnemyWithScore*>(subject) };
		if (enemy == nullptr)
		{
			FLogger.Log(StatusCode{ StatusCode::Status::WARNING, "Sent Event::ScoreAwarded from object that does not hold score.", (void*)subject });
			return;
		}
		m_Score += enemy->score;
	}
}