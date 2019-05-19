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
	, m_TwoPlayer{ false }
	, m_Versus{ false }
{}


DigDug::Game::~Game()
{
}

void DigDug::Game::Run()
{
	FLocalizer.Load("loca.csv", "en-uk");
	m_Engine.Initialize();

	FInputManager.AddPlayer();
	FInputManager.AddPlayer();

	InitMenuScene();
	InitTestScene();
	FSceneManager.ActivateSceneByName("MenuScene");

	if (!FSceneManager.IsSceneSet()) return;
	m_Engine.Run();
}

void DigDug::Game::InitTestScene()
{
	using namespace flgin;
	Scene* scene{ FSceneManager.CreateScene("GameScene") };

#ifdef _DEBUG
	GameObject* go{ new flgin::GameObject{} };
	RenderComponent* renderComponent{ scene->CreateRenderComponent(go, 4) };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene->AddGameObject(go);
#endif

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
	inputComponent->AddKeyboardMapping(SDLK_e, angryToggleCommand);
	
	StateComponent* stateComponent{ new StateComponent{ go } };
	IdleState* idleState{ new IdleState{} };
	idleState->SetAttachedSprite(spriteComponent);
	idleState->SetPlayer(playerComponent);
	stateComponent->SetCurrentState(idleState);
	
	go->AddComponent(spriteComponent);
	go->AddComponent(gridMover);
	go->AddComponent(inputComponent);
	go->AddComponent(playerComponent);
	go->AddComponent(stateComponent);
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

#ifdef _DEBUG
	go = new GameObject{};
	renderComponent = scene->CreateRenderComponent(go, 4);
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene->AddGameObject(go);
#endif

	go = new GameObject{};
	MenuComponent* menu{ new MenuComponent{ go } };
	InputComponent* inputComponent{ FInputManager.GetPlayer(0) };
	MenuConfirmCommand* menuConfirmCommand{ new MenuConfirmCommand{ menu } };
	MenuPreviousCommand* menuPreviousCommand{ new MenuPreviousCommand{ menu } };
	MenuNextCommand* menuNextCommand{ new MenuNextCommand{ menu } };

	inputComponent->AddKeyboardMapping(SDLK_w, menuPreviousCommand);
	inputComponent->AddKeyboardMapping(SDLK_s, menuNextCommand);
	inputComponent->AddKeyboardMapping(SDLK_RETURN, menuConfirmCommand);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, menuNextCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, menuPreviousCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_A, menuConfirmCommand);

	go->AddComponent(menu);
	scene->AddGameObject(go);

	std::function<void()> launch{ std::bind(&Game::InitSinglePlayer, this) };
	FunctionHolder<void>* actionLaunch{ new FunctionHolder<void>{ launch } };
	//FunctionHolder<void>* action{ new FunctionHolder<void>{ []() { FSceneManager.ActivateSceneByName("GameScene"); } } };
	go = Button::Create(scene, menu, "PLAY", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionLaunch);
	go->SetPosition(200.0f, 80.0f);

	std::function<void()> launchTwoPlayer{ std::bind(&Game::InitTwoPlayer, this) };
	FunctionHolder<void>* actionLaunchTP{ new FunctionHolder<void>{ launchTwoPlayer } };
	go = Button::Create(scene, menu, "2 PLAYER", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionLaunchTP);
	go->SetPosition(200.0f, 170.0f);

	std::function<void()> launchVersus{ std::bind(&Game::InitVersus, this) };
	FunctionHolder<void>* actionLaunchVersus{ new FunctionHolder<void>{ launchVersus } };
	go = Button::Create(scene, menu, "VERSUS", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionLaunchVersus);
	go->SetPosition(200.0f, 260.0f);

	//std::function<void()> quit{ std::bind(&InputManager::Quit, FInputManager) };
	FunctionHolder<void>* actionQuit{ new FunctionHolder<void>{ []() { FInputManager.Quit(); } } };
	go = Button::Create(scene, menu, "QUIT", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionQuit);
	go->SetPosition(200.0f, 360.0f);

	menu->SelectDefault();
}

void DigDug::Game::InitSinglePlayer()
{
	FSceneManager.ActivateSceneByName("GameScene");
}

void DigDug::Game::InitTwoPlayer()
{
	using namespace flgin;

	FSceneManager.ActivateSceneByName("GameScene");
	if (!FInputManager.GetPlayer(1))
	{
		InitSinglePlayer();
		return;
	}

	Scene* scene{ FSceneManager.GetCurrentScene() };
	GameObject* go{ new GameObject{} };
	MovementGrid* grid{ scene->FindComponentOfType<MovementGrid>() };
	go->SetPosition(150.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(1) };
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
	
	inputComponent->AddKeyboardMapping(SDLK_RIGHT, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_LEFT, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_DOWN, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_UP, gridMoveUp);
	
	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(go, 2) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(4, 1, 30.0f, 30.0f, 1.0f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	
	Player* playerComponent{ new Player{ go } };
	ToggleAngryCommand* angryToggleCommand{ new ToggleAngryCommand{playerComponent} };
	inputComponent->AddKeyboardMapping(SDLK_p, angryToggleCommand);
	
	StateComponent* stateComponent{ new StateComponent{ go } };
	IdleState* idleState{ new IdleState{} };
	idleState->SetAttachedSprite(spriteComponent);
	idleState->SetPlayer(playerComponent);
	stateComponent->SetCurrentState(idleState);
	
	go->AddComponent(spriteComponent);
	go->AddComponent(gridMover);
	go->AddComponent(inputComponent);
	go->AddComponent(playerComponent);
	go->AddComponent(stateComponent);
	scene->AddGameObject(go);
}

void DigDug::Game::InitVersus()
{
	FSceneManager.ActivateSceneByName("GameScene");
	if (!FInputManager.GetPlayer(1))
	{
		InitSinglePlayer();
		return;
	}
}
