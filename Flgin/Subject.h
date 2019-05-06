#pragma once
#include "Observer.h"
#include <vector>
namespace flgin
{
	class Subject
	{
	public:
		Subject();
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void Notify(Observer::Event event);

	private:
		std::vector<Observer*> m_pObservers;
	};


	class TestEnemyWithScore : public Subject
	{
	public:
		int score{ 100 };
	};
}