#pragma once
#include "Observer.h"
#include <iostream>
namespace flgin
{
	class ScoreObserver final : public Observer
	{
	public:
		void Notify(Event event, const Subject* subject) override;

	private:
		int m_Score;
	};
}