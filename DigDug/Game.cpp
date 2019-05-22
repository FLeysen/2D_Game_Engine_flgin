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
#include "Invoker.h"
#include "ObserverManager.h"
#include "LivesObserver.h"
#include "ScoreObserver.h"
#include "GameOverObserver.h"
#include "ColliderComponent.h"
#include "CollisionManager.h"
#include "RockStates.h"
#include "FreeMover.h"

DigDug::Game::Game()
	: m_Engine{}
	, m_HasTwoScores{ false }
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

	if (!FSceneManager.IsSceneSet()) return;
	m_Engine.Run();
}

void DigDug::Game::InitGameScene()
{
	using namespace flgin;
	Scene* scene{ FSceneManager.CreateScene("GameScene") };
	FInvoker.CancelAllInvokes();
	FInputManager.ClearCommands();
	FCollisionManager.ClearColliders();

	GameObject* go{  };
#ifdef _DEBUG
	go = new GameObject{};
	RenderComponent* renderComponent{ scene->CreateRenderComponent(go, 4) };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene->AddGameObject(go);
#endif

	go = new GameObject{};
	go->SetPosition(15.f, 45.0f);
	MovementGrid* grid{ new MovementGrid{ go, 13, 21, 30.0f } };
	go->AddComponent(grid);
	go->AddComponent(new GridRenderer{ go, scene, grid });
	scene->AddGameObject(go);

	go = new GameObject{};
	SpriteComponent* livesSprite{ scene->CreateSpriteComponent(go) };
	livesSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texLives")));
	LivesObserver* livesObserver{ new LivesObserver{ 4, livesSprite } };
	go->SetPosition(0.f, 425.f);
	scene->AddGameObject(go);

	go = new GameObject{};
	RenderComponent* scoreRenderer{ scene->CreateRenderComponent(go) };
	go->AddComponent(scoreRenderer);
	TextComponent* textRenderer{ new TextComponent{ go, FLocalizer.Get("fontDefault"), 30, {255, 255, 255} } };
	go->AddComponent(textRenderer);
	ScoreObserver* scoreObserver{ new ScoreObserver{ textRenderer } };
	go->SetPosition(170.f, 425.f);
	scene->AddGameObject(go);
	
	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(0) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid} };
	inputComponent->Clear();
	QuitCommand* quitCommand{ new QuitCommand{} };
	ReturnToMenuCommand* returnCommand{ new ReturnToMenuCommand{*this} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_BACK, returnCommand);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);

	inputComponent->AddKeyboardMapping(SDLK_d, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_a, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_s, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_w, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);
	inputComponent->AddKeyboardMapping(SDLK_r, returnCommand);
	
	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(go, 2) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(4, 1, 30.0f, 30.0f, 1.0f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	
	Player* playerComponent{ new Player{ go } };
	playerComponent->AddObserver(livesObserver);
	playerComponent->AddObserver(scoreObserver);
	playerComponent->AddObserver(new GameOverObserver{ this });
	ToggleAngryCommand* angryToggleCommand{ new ToggleAngryCommand{playerComponent} };
	inputComponent->AddKeyboardMapping(SDLK_e, angryToggleCommand);
	inputComponent->AttachToGameObject(go);
	
	StateComponent* stateComponent{ new StateComponent{ go } };
	IdleState* idleState{ new IdleState{} };
	idleState->SetAttachedSprite(spriteComponent);
	idleState->SetPlayer(playerComponent);
	stateComponent->SetCurrentState(idleState);

	ColliderComponent* colliderComponent{ new ColliderComponent{ go, "Player", 30.f, 30.f } };
	colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{ []() {} });

	go->AddComponent(colliderComponent);
	go->AddComponent(spriteComponent);
	go->AddComponent(gridMover);
	go->AddComponent(inputComponent);
	go->AddComponent(playerComponent);
	go->AddComponent(stateComponent);
	scene->AddGameObject(go);

	go = new GameObject{};
	GridNode* node{ grid->GetNodeNearestTo(150.0f, 150.0f) };
	go->SetPosition(node->GetPosition().x, node->GetPosition().y);
	FreeMover* freeMoveComponent{ new FreeMover{go, 0.f} };
	ColliderComponent* rockColliderComponent{ new ColliderComponent{ go, "Rock", 30.f, 30.f } };
	renderComponent = scene->CreateRenderComponent(go, 3);
	renderComponent->SetPositionOffset(-15.f, -15.f);
	renderComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texRock")));

	stateComponent = new StateComponent{ go };
	StuckState* stuckState{ new StuckState{} };
	stuckState->SetAttachedCollider(rockColliderComponent);
	stuckState->SetAttachedMover(freeMoveComponent);
	stuckState->SetNode(node);
	stuckState->SetPlayer1Collider(colliderComponent);
	stateComponent->SetCurrentState(stuckState);

	go->AddComponent(freeMoveComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(rockColliderComponent);
	go->AddComponent(stateComponent);
	scene->AddGameObject(go);
}

void DigDug::Game::InitMenuScene()
{
	using namespace flgin;
	Scene* scene{ FSceneManager.CreateScene("MenuScene") };
	FSceneManager.RemoveCurrentScene();
	FSceneManager.ActivateSceneByName("MenuScene");
	FInvoker.CancelAllInvokes();
	FObserverManager.Clear();
	FInputManager.ClearCommands();	
	FCollisionManager.ClearColliders();
	if (FInputManager.GetPlayer(1)) FInputManager.GetPlayer(1)->Clear();

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
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 36, {255, 255, 255 }, FLocalizer.Get("stringGameName") });
	go->SetPosition(260.f, 20.f);
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
	QuitCommand* quitCommand{ new QuitCommand{} };

	inputComponent->Clear();
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);
	inputComponent->AddKeyboardMapping(SDLK_w, menuPreviousCommand);
	inputComponent->AddKeyboardMapping(SDLK_s, menuNextCommand);
	inputComponent->AddKeyboardMapping(SDLK_RETURN, menuConfirmCommand);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, menuNextCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, menuPreviousCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_A, menuConfirmCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);

	inputComponent->AttachToGameObject(go);
	go->AddComponent(inputComponent);
	go->AddComponent(menu);
	scene->AddGameObject(go);

	std::function<void()> launch{ std::bind(&Game::SetSwappingToSingle, this) };
	FunctionHolder<void>* actionSwap{ new FunctionHolder<void>{ launch } };
	go = Button::Create(scene, menu, "PLAY", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionSwap);
	go->SetPosition(200.0f, 80.0f);

	std::function<void()> launchTwoPlayer{ std::bind(&Game::SetSwappingToTwoPlayer, this) };
	FunctionHolder<void>* actionSwapToTP{ new FunctionHolder<void>{ launchTwoPlayer } };
	go = Button::Create(scene, menu, "2 PLAYER", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionSwapToTP);
	go->SetPosition(200.0f, 170.0f);

	std::function<void()> launchVersus{ std::bind(&Game::SetSwappingToVerus, this) };
	FunctionHolder<void>* actionSwapToVersus{ new FunctionHolder<void>{ launchVersus } };
	go = Button::Create(scene, menu, "VERSUS", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionSwapToVersus);
	go->SetPosition(200.0f, 260.0f);

	FunctionHolder<void>* actionQuit{ new FunctionHolder<void>{ []() { FInputManager.Quit(); } } };
	go = Button::Create(scene, menu, "QUIT", { 255, 255, 255 }, FLocalizer.Get("fontDefault"), FLocalizer.Get("texButton"), FLocalizer.Get("texButtonSelected"), actionQuit);
	go->SetPosition(200.0f, 360.0f);

	menu->SelectDefault();
}

void DigDug::Game::InitSinglePlayer()
{
	InitGameScene();
	FSceneManager.RemoveCurrentScene();
	FSceneManager.ActivateSceneByName("GameScene");
	m_HasTwoScores = false;
}

void DigDug::Game::InitTwoPlayer()
{
	using namespace flgin;

	InitSinglePlayer();
	
	m_HasTwoScores = true;
	Scene* scene{ FSceneManager.GetCurrentScene() };
	GameObject* go{ new GameObject{} };
	MovementGrid* grid{ scene->FindComponentOfType<MovementGrid>() };
	go->SetPosition(650.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(1) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid} };
	QuitCommand* quitCommand{ new QuitCommand{} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };
	
	inputComponent->Clear();
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
	inputComponent->AttachToGameObject(go);

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

	go = new GameObject{};
	SpriteComponent* livesSprite{ scene->CreateSpriteComponent(go) };
	livesSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texLives")));
	livesSprite->SetRotation(180.0f);
	livesSprite->SetPositionOffset(45.0f, 30.f);
	livesSprite->SetFlips(false, true);
	LivesObserver* livesObserver{ new LivesObserver{ 4, livesSprite } };
	go->SetPosition(585.f, 425.f);
	scene->AddGameObject(go);
	playerComponent->AddObserver(livesObserver);
	playerComponent->AddObserver(new GameOverObserver{ this });

	go = new GameObject{};
	RenderComponent* scoreRenderer{ scene->CreateRenderComponent(go) };
	go->AddComponent(scoreRenderer);
	TextComponent* textRenderer{ new TextComponent{ go, FLocalizer.Get("fontDefault"), 30, {255, 255, 255} } };
	go->AddComponent(textRenderer);
	ScoreObserver* scoreObserver{ new ScoreObserver{ textRenderer } };
	go->SetPosition(380.f, 425.f);
	scene->AddGameObject(go);
	playerComponent->AddObserver(scoreObserver);
}

void DigDug::Game::InitVersus()
{
	InitSinglePlayer();
}

void DigDug::Game::InitEndScene()
{
	using namespace flgin;
	
	UINT scoreP1{ FInputManager.GetPlayer(0)->GetGameObject()->GetComponent<Player>()->GetScore() };
	UINT scoreP2{ m_HasTwoScores ? FInputManager.GetPlayer(1)->GetGameObject()->GetComponent<Player>()->GetScore() : 0 };

	InitMenuScene();

	Scene* scene{ FSceneManager.GetCurrentScene() };
	GameObject* go{ new GameObject{} };
	RenderComponent* renderComponent{ scene->CreateRenderComponent(go, 3) };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 36, {255, 255, 255}, FLocalizer.Get("stringP1") });
	go->SetPosition(10.f, 380.f);
	scene->AddGameObject(go);

	go = new GameObject{};
	renderComponent = scene->CreateRenderComponent(go, 3);
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 36, {255, 255, 255}, std::to_string(scoreP1) });
	go->SetPosition(10.f, 420.f);
	scene->AddGameObject(go);

	if (m_HasTwoScores)
	{
		go = new GameObject{};
		renderComponent = scene->CreateRenderComponent(go, 3);
		go->AddComponent(renderComponent);
		go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 36, {255, 255,255}, FLocalizer.Get("stringP2") });
		go->SetPosition(450.f, 380.f);
		scene->AddGameObject(go);

		go = new GameObject{};
		renderComponent = scene->CreateRenderComponent(go, 3);
		go->AddComponent(renderComponent);
		go->AddComponent(new TextComponent{ go, FLocalizer.Get("fontDefault"), 36, {255,255,255 }, std::to_string(scoreP2) });
		go->SetPosition(450.f, 420.f);
		scene->AddGameObject(go);
	}
}

void DigDug::Game::SetSwappingToSingle()
{
	FSceneManager.SwapScene(new flgin::FunctionHolder<void>{ std::bind(&Game::InitSinglePlayer, this) });
}

void DigDug::Game::SetSwappingToTwoPlayer()
{
	FSceneManager.SwapScene(new flgin::FunctionHolder<void>{ std::bind(&Game::InitTwoPlayer, this) });
}

void DigDug::Game::SetSwappingToVerus()
{
	FSceneManager.SwapScene(new flgin::FunctionHolder<void>{ std::bind(&Game::InitVersus, this) });
}