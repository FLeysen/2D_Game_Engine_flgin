#include "pch.h"
#include "Game.h"
#include "TextLocalizer.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "TextLocalizer.h"
#include "FPSComponent.h"
#include "MovementGrid.h"
#include "GridRenderer.h"
#include "InputComponent.h"
#include "GridMovementComponent.h"
#include "PlayerCommands.h"
#include "MovementCommands.h"
#include "SpriteComponent.h"
#include "DigDugStates.h"
#include "Player.h"
#include "DigDugCommands.h"
#include "InputManager.h"
#include "Button.h"
#include "MenuComponent.h"
#include "FunctionHolder.h"

DigDug::Game::Game()
	: m_Engine{}
{}


DigDug::Game::~Game()
{
}

void DigDug::Game::Run()
{
	FLocalizer.Load("loca.csv", "en-uk");
	m_Engine.Initialize();

	InitPlayers();
	InitMenuScene();
	InitTestScene();
	FSceneManager.ActivateSceneByName("MenuScene");

	if (!FSceneManager.IsSceneSet()) return;
	m_Engine.Run();
}

void DigDug::Game::InitTestScene()
{
	using namespace flgin;
	Scene* scene{ FSceneManager.CreateScene("TestScene") };

	GameObject* go{ new flgin::GameObject{} };
	RenderComponent* renderComponent{ scene->CreateRenderComponent(go, 4) };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene->AddGameObject(go);

	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	MovementGrid* grid{ new MovementGrid{ go, 16, 21, 30.0f } };
	go->AddComponent(grid);
	go->AddComponent(new GridRenderer{ go, scene, grid });
	scene->AddGameObject(go);
	
	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(0) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid} };
	QuitCommand* quitCommand{ new QuitCommand{} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };
	
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });
	
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);
	
	inputComponent->AddKeyboardMapping(SDLK_d, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_a, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_s, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_w, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);
	
	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(go, 2) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(4, 1, 30.0f, 30.0f, 1.0f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	
	Player* playerComponent{ new Player{ go } };
	ToggleAngryCommand* angryToggleCommand{ new ToggleAngryCommand{playerComponent} };
	inputComponent->AddKeyboardMapping(SDLK_i, angryToggleCommand);
	
	StateComponent* stateComponent{ new StateComponent{ go } };
	DigDugState::SetAttachedSprite(spriteComponent);
	DigDugState::SetPlayer(playerComponent);
	stateComponent->SetCurrentState(DigDugState::GetDefaultState());
	
	go->AddComponent(spriteComponent);
	go->AddComponent(gridMover);
	go->AddComponent(inputComponent);
	go->AddComponent(playerComponent);
	go->AddComponent(stateComponent);
	scene->AddGameObject(go);
	
	go = new GameObject{};
	inputComponent = FInputManager.GetPlayer(1);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_B, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 1 });
	go->AddComponent(inputComponent);
	scene->AddGameObject(go);

	//go = new GameObject{};
	//PathfinderComponent* pathfinderComponent{ new PathfinderComponent{ go, 100.0f, grid} };
	//spriteComponent = new SpriteComponent{ go, scene };
	//spriteComponent->SetTexture(FResourceManager.LoadTexture("ActualSprite.png"));
	//spriteComponent->SetPositionOffset(-7.5f, -7.5f);
	//spriteComponent->SetSpriteInfo(4, 1, 15.0f, 15.0f, 1.0f);
	//spriteComponent->SetDimensions(25.0f, 25.0f);
	//go->AddComponent(spriteComponent);
	//go->AddComponent(pathfinderComponent);
	//go->SetPosition(15.0f, 15.0f);
	//scene.AddGameObject(go);
	//FInputManager.AddPathfinder(pathfinderComponent);
}

void DigDug::Game::InitMenuScene()
{
	using namespace flgin;
	Scene* scene{ FSceneManager.CreateScene("MenuScene") };

	GameObject* go{ new flgin::GameObject{} };
	RenderComponent* renderComponent{ scene->CreateRenderComponent(go, 0) };
	renderComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texBg")));
	go->AddComponent(renderComponent);
	scene->AddGameObject(go);

	go = new GameObject{};
	renderComponent = scene->CreateRenderComponent(go, 0);
	renderComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texLogo")));
	go->AddComponent(renderComponent);
	go->SetPosition(216, 180);
	scene->AddGameObject(go);

	go = new GameObject{};
	renderComponent = scene->CreateRenderComponent(go, 3);
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 36, {255, 0,0 }, FLocalizer.Get("stringGameName") });
	go->SetPosition(80.f, 20.f);
	scene->AddGameObject(go);

	go = new GameObject{};
	renderComponent = scene->CreateRenderComponent(go, 4);
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene->AddGameObject(go);

	go = new GameObject{};
	MenuComponent* menu{ new MenuComponent{ go } };
	InputComponent* inputComponent{ FInputManager.GetPlayer(0) };
	inputComponent->AddKeyboardMapping(SDLK_w, new MenuPreviousCommand{ menu });
	inputComponent->AddKeyboardMapping(SDLK_s, new MenuNextCommand{ menu });
	MenuConfirmCommand* menuConfirmCommand{ new MenuConfirmCommand{ menu } };
	inputComponent->AddKeyboardMapping(SDLK_RETURN, menuConfirmCommand);
	go->AddComponent(menu);
	scene->AddGameObject(go);

	FunctionHolder<void>* action{ new FunctionHolder<void>{ []() { FSceneManager.ActivateSceneByName("TestScene"); } } };
	go = Button::Create(scene, menu, "PLAY", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), action);
	go->SetPosition(200.0f, 120.0f);

	std::function<void()> setReturnFalse{ std::bind(&DigDug::MenuConfirmCommand::SetExecuteReturn, menuConfirmCommand, false) };
	FunctionHolder<void>* actionSetReturn{ new FunctionHolder<void>{ setReturnFalse } };
	go = Button::Create(scene, menu, "QUIT", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionSetReturn);
	go->SetPosition(200.0f, 280.0f);
	menu->SelectDefault();
}

void DigDug::Game::InitPlayers()
{
	using namespace flgin;
	FInputManager.AddPlayer();
	FInputManager.AddPlayer();
}