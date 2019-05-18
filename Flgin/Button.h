#pragma once
namespace flgin
{
	class GameObject;
	class FunctionHolderBase;
	class Scene;

	class Button
	{
	public:
		static GameObject* Create(Scene* pScene, std::string&& text, std::string&& fontName, std::string&& texName, std::string&& selectedTexName, FunctionHolderBase* action);
	};
}

