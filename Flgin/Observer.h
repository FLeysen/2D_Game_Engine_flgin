#pragma once
namespace flgin
{
	class Subject;
	class Observer
	{
	public:
		enum class Event
		{
			ScoreAwarded
		};

		virtual ~Observer() {}
		virtual void Notify(Event event, const Subject* const subject) = 0;
	};
}