#include "FlginPCH.h"
#include "Button.h"
#include "FunctionHolder.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "TextLocalizer.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "ButtonComponent.h"

flgin::GameObject* flgin::Button::Create(Scene* pScene, std::string&& text, std::string&& fontName, std::string&& texName, std::string&&, FunctionHolderBase* action)
{
	GameObject* go{ new GameObject{} };
	RenderComponent* renderComponent{ pScene->CreateRenderComponent( go, 1 ) };
	renderComponent->SetTexture(FResourceManager.LoadTexture(texName));
	ButtonComponent* buttonComponent{ new ButtonComponent{ go, action } };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, fontName, 36, {255, 0,0 }, text });
	go->AddComponent(buttonComponent);
	pScene->AddGameObject(go);
	return go;
}
