#pragma once
namespace flgin
{
	class GameObject;
	class Command
	{
	public:
		Command();
		virtual ~Command() = default;
		virtual bool Execute(GameObject& attachedObject, bool isInverseAction = false) = 0;
	};
}