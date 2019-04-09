#pragma once
namespace flgin
{
	class GameObject;
	class Command
	{
	public:
		Command();
		virtual ~Command() = default;
		virtual bool Execute(GameObject& attachedObject) = 0;
	};
}