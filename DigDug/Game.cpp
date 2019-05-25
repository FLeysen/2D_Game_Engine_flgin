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
#include "Rock.h"
#include "Fygar.h"
#include "FygarCommands.h"
#include "FygarStates.h"
#include "NPCFygar.h"
#include "NextLevelObserver.h"

#undef max
#undef min

DigDug::Game::Game()
	: m_Engine{}
	, m_GameState{ GameState::Single }
	, m_Level{ 0 }
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

	m_Level = 0;
	NextLevelObserver* ob{ new NextLevelObserver{} };
	ob->SetNextLevelInit(new FunctionHolder<void>{
		[this]()
	{ FInvoker.AddInvoke(new InvokeHolder<void>{ this, 5.f, [this]() { new flgin::FunctionHolder<void>{ std::bind(&Game::SetSwappingToNextLevel, this)}; } });  } });

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
	Fygar::SetGrid(grid);
	go->AddComponent(grid);
	go->AddComponent(new GridRenderer{ go, scene, grid });
	scene->AddGameObject(go);

	go = new GameObject{};
	SpriteComponent* livesSprite{ scene->CreateSpriteComponent(go) };
	livesSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texLives")));
	LivesObserver* livesObserver{ new LivesObserver{ 3, livesSprite } };
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

	GameObject* pump{ new GameObject{} };
	pump->SetTag("Pump");
	SpriteComponent* pumpSprite{ scene->CreateSpriteComponent(pump) };
	pumpSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPump")));
	pumpSprite->SetDimensions(180.f, 18.f);
	pumpSprite->SetSpriteInfo(1, 10, 180.f, 18.f, 0.1f, true);
	FreeMover* pumpMover{ new FreeMover{pump, 0.f} };
	ColliderComponent* pumpCollider{ new ColliderComponent{pump, "Pump", 30.f, 30.f} };
	pump->AddComponent(pumpMover);
	pump->AddComponent(pumpSprite);
	pump->AddComponent(pumpCollider);
	scene->AddGameObject(pump);
	
	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(0) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid, 90.f, true } };
	go->AddComponent(gridMover);
	inputComponent->Clear();
	QuitCommand* quitCommand{ new QuitCommand{} };
	ReturnToMenuCommand* returnCommand{ new ReturnToMenuCommand{*this} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };
	FireCommand* fireCommand{ new FireCommand{} };

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_BACK, returnCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_X, fireCommand);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);

	AxisRange range{};
	range.max = std::numeric_limits<short int>::max();
	range.min = 15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveDown);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveRight);
	range.min = std::numeric_limits<short int>::min();
	range.max = -15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveUp);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveLeft);

	inputComponent->AddKeyboardMapping(SDLK_d, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_a, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_s, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_w, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);
	inputComponent->AddKeyboardMapping(SDLK_r, returnCommand);
	inputComponent->AddKeyboardMapping(SDLK_q, fireCommand);
	
	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(go, 4) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(6, 1, 30.0f, 30.0f, 0.4f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	
	Player* playerComponent{ new Player{ go, pump } };
	playerComponent->AddObserver(livesObserver);
	playerComponent->AddObserver(scoreObserver);
	playerComponent->AddObserver(new GameOverObserver{ *this });
	playerComponent->SetInitPos(go->GetPosition().x, go->GetPosition().y);

	//TODO: REMOVE TEST CODE
	DieCommand* angryToggleCommand{ new DieCommand{playerComponent} };
	inputComponent->AddKeyboardMapping(SDLK_e, angryToggleCommand);
	inputComponent->AttachToGameObject(go);
	NPCFygar::Create(scene, { 300.f, 40.f });
	FCollisionManager.AddIgnore("Flame", "Fygar");

	StateComponent* stateComponent{ new StateComponent{ go } };
	IdleState* idleState{ new IdleState{} };
	idleState->SetAttachedSprite(spriteComponent);
	idleState->SetPlayer(playerComponent);
	stateComponent->SetCurrentState(idleState);

	ColliderComponent* colliderComponent{ new ColliderComponent{ go, "Player", 30.f, 30.f } };
	colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{ []() {} });
	pumpCollider->SetOnCollisionFunction(new FunctionHolder<void>{
	[playerComponent, pumpCollider, pumpSprite, pumpMover, stateComponent]()
{
	if (pumpCollider->GetCollisionHit()->GetGameObject()->CompareTag("Fygar"))
	{
		Fygar* hit{ pumpCollider->GetCollisionHit()->GetGameObject()->GetComponent<Fygar>() };
		if (hit->IsBloating()) return;
		hit->SetBloating(true);
		hit->SetHitBy(playerComponent);
		pumpSprite->StopAnimating();
		pumpMover->SetMoving(false);
		FInvoker.CancelOwnedInvokes(stateComponent->GetCurrentState());
		static_cast<FiringState*>(stateComponent->GetCurrentState())->SetHit(hit);
	}
	else
		playerComponent->SetFiring(false); } });
	FCollisionManager.AddIgnore("Pump", "Player");

	go->SetTag("Player");
	go->AddComponent(colliderComponent);
	go->AddComponent(spriteComponent);
	go->AddComponent(inputComponent);
	go->AddComponent(playerComponent);
	go->AddComponent(stateComponent);
	scene->AddGameObject(go);

	Rock::Create(scene, grid->GetNodeNearestTo(150.f, 150.f), FLocalizer.Get("texRock"));

	RenderComponent* renComp{};
	for (unsigned int i{ 0 }; i < grid->GetGridSize() - 21; ++i)
	{
		if (rand() % 4 != 0)
		{
			if (grid->GetGrid()[i + 21].IsBlocked()) continue;
			grid->GetGrid()[i + 21].SetBlocked(true);
			go = new GameObject{};
			renComp = scene->CreateRenderComponent(go, 2);
			renComp->SetPositionOffset(-15.f, -15.f);
			renComp->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texDirt" + std::to_string(i * 4 / (grid->GetGridSize() - 21)))));

			colliderComponent = new ColliderComponent{ go, "Dirt", 7.5f, 7.5f, 15.f, 15.f };
			colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{ [go, grid, colliderComponent]() { if (!colliderComponent->GetCollisionHit()->GetGameObject()->CompareTag("Player")) return; go->SetActive(false); grid->GetNodeNearestTo(go->GetPosition().x, go->GetPosition().y)->SetBlocked(false); } });
			FCollisionManager.AddIgnore("Dirt", "Dirt");

			go->SetTag("Dirt");
			go->AddComponent(colliderComponent);
			go->AddComponent(renComp);
			go->SetPosition(grid->GetGrid()[i].GetPosition().x, grid->GetGrid()[i + 21].GetPosition().y);
			scene->AddGameObject(go);
		}
	}
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

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, menuPreviousCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, menuNextCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_A, menuConfirmCommand);

	AxisRange range{};
	range.max = std::numeric_limits<short int>::max();
	range.min = range.max - 10;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, menuNextCommand);
	range.min = std::numeric_limits<short int>::min();
	range.max = range.min + 10;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, menuPreviousCommand);

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
	m_GameState = GameState::Single;
}

void DigDug::Game::InitTwoPlayer()
{
	using namespace flgin;

	InitSinglePlayer();
	m_GameState = GameState::Coop;

	Scene* scene{ FSceneManager.GetCurrentScene() };

	GameObject* pump{ new GameObject{} };
	pump->SetTag("Pump");
	SpriteComponent* pumpSprite{ scene->CreateSpriteComponent(pump) };
	pumpSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPump")));
	pumpSprite->SetDimensions(180.f, 18.f);
	pumpSprite->SetSpriteInfo(1, 10, 180.f, 18.f, 0.1f, true);
	FreeMover* pumpMover{ new FreeMover{pump, 0.f} };
	ColliderComponent* pumpCollider{ new ColliderComponent{pump, "Pump", 30.f, 30.f} };
	pump->AddComponent(pumpMover);
	pump->AddComponent(pumpSprite);
	pump->AddComponent(pumpCollider);
	scene->AddGameObject(pump);

	GameObject* go{ new GameObject{} };
	MovementGrid* grid{ scene->FindComponentOfType<MovementGrid>() };
	go->SetPosition(650.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(1) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid, 90.0f, true } };
	go->AddComponent(gridMover);
	QuitCommand* quitCommand{ new QuitCommand{} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };
	ReturnToMenuCommand* returnCommand{ FInputManager.GetCommand<ReturnToMenuCommand>() };
	FireCommand* fireCommand{ FInputManager.GetCommand<FireCommand>() };

	inputComponent->Clear();
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_BACK, returnCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_X, fireCommand);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);

	inputComponent->AddKeyboardMapping(SDLK_RIGHT, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_LEFT, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_DOWN, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_UP, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_p, fireCommand);
	inputComponent->AddKeyboardMapping(SDLK_o, returnCommand);

	AxisRange range{};
	range.max = std::numeric_limits<short int>::max();
	range.min = 15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveDown);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveRight);
	range.min = std::numeric_limits<short int>::min();
	range.max = -15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveUp);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveLeft);
	
	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(go, 4) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(6, 1, 30.0f, 30.0f, 0.4f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	spriteComponent->SetRotationalOffset(0.f, 0.f);
	spriteComponent->SetRotation(180.f);
	spriteComponent->SetFlips(false, true);

	Player* playerComponent{ new Player{ go, pump } };
	playerComponent->SetInitPos(go->GetPosition().x, go->GetPosition().y);
	//TODO: REMOVE TEST CODE
	DieCommand* angryToggleCommand{ new DieCommand{playerComponent} };
	inputComponent->AddKeyboardMapping(SDLK_i, angryToggleCommand);
	inputComponent->AttachToGameObject(go);

	StateComponent* stateComponent{ new StateComponent{ go } };
	IdleState* idleState{ new IdleState{} };
	idleState->SetAttachedSprite(spriteComponent);
	idleState->SetPlayer(playerComponent);
	stateComponent->SetCurrentState(idleState);

	ColliderComponent* colliderComponent{ new ColliderComponent{ go, "Player", 30.f, 30.f } };
	colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{ []() {} });
	pumpCollider->SetOnCollisionFunction(new FunctionHolder<void>{
		[playerComponent, pumpCollider, pumpSprite, pumpMover, stateComponent]()
	{ 
		if (pumpCollider->GetCollisionHit()->GetGameObject()->CompareTag("Fygar"))
		{
			Fygar* hit{ pumpCollider->GetCollisionHit()->GetGameObject()->GetComponent<Fygar>() };
			if (hit->IsBloating()) return;
			hit->SetBloating(true);
			hit->SetHitBy(playerComponent);
			pumpSprite->StopAnimating();
			pumpMover->SetMoving(false);
			FInvoker.CancelOwnedInvokes(stateComponent->GetCurrentState());
			static_cast<FiringState*>(stateComponent->GetCurrentState())->SetHit(hit);
		}
		else
			playerComponent->SetFiring(false); } });

	go->AddComponent(colliderComponent);
	go->SetTag("Player");
	go->AddComponent(spriteComponent);
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
	LivesObserver* livesObserver{ new LivesObserver{ 3, livesSprite } };
	go->SetPosition(585.f, 425.f);
	scene->AddGameObject(go);
	playerComponent->AddObserver(livesObserver);
	playerComponent->AddObserver(new GameOverObserver{ *this });

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
	using namespace flgin;

	InitSinglePlayer();
	m_GameState = GameState::Versus;

	Scene* scene{ FSceneManager.GetCurrentScene() };
	
	GameObject* flame{ new GameObject{} };
	flame->SetTag("Flame");
	SpriteComponent* flameSprite{ scene->CreateSpriteComponent(flame) };
	flameSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texFire")));
	flameSprite->SetDimensions(30.f, 24.f);
	flameSprite->SetPositionOffset(-15.f, -12.f);
	flameSprite->SetSpriteInfo(6, 1, 30.f, 24.f, 0.3f, true);
	ColliderComponent* flameCollider{ new ColliderComponent{flame, "Flame", 20.f, 17.f} };
	flameCollider->SetOnCollisionFunction(new FunctionHolder<void>{
	[flameCollider]() {
	if (flameCollider->GetCollisionHit()->GetGameObject()->CompareTag("Player"))
		flameCollider->GetCollisionHit()->GetGameObject()->GetComponent<Player>()->ChangeLives(-1); } });
	FCollisionManager.AddIgnore("Pump", "Flame");

	flame->AddComponent(flameCollider);
	flame->AddComponent(flameSprite);
	scene->AddGameObject(flame);


	GameObject* fygar{ new GameObject{} };
	MovementGrid* grid{ scene->FindComponentOfType<MovementGrid>() };
	fygar->SetPosition(650.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(1) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ fygar, 100.0f, grid, 90.0f, false } };
	fygar->AddComponent(gridMover);
	QuitCommand* quitCommand{ new QuitCommand{} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };
	ReturnToMenuCommand* returnCommand{ FInputManager.GetCommand<ReturnToMenuCommand>() };
	BreatheFireCommand* fireBreatheCommand{ FInputManager.GetCommand<BreatheFireCommand>() };
	if (!fireBreatheCommand) fireBreatheCommand = new BreatheFireCommand{};

	inputComponent->Clear();
	inputComponent->AttachToGameObject(fygar);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_X, fireBreatheCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_BACK, returnCommand);

	inputComponent->AddKeyboardMapping(SDLK_RIGHT, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_LEFT, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_DOWN, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_UP, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_p, fireBreatheCommand);
	inputComponent->AddKeyboardMapping(SDLK_o, returnCommand);

	AxisRange range{};
	range.max = std::numeric_limits<short int>::max();
	range.min = 15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveDown);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveRight);
	range.min = std::numeric_limits<short int>::min();
	range.max = -15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveUp);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveLeft);

	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(fygar, 4) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texFygar")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(4, 1, 30.0f, 30.0f, 0.4f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	spriteComponent->SetRotationalOffset(0.f, 0.f);
	spriteComponent->SetRotation(180.f);
	spriteComponent->SetFlips(false, true);

	Fygar* fygarComponent{ new Fygar{ fygar, flame } };
	StateComponent* stateComponent{ new StateComponent{ fygar } };
	FygarIdleState* idleState{ new FygarIdleState{} };
	idleState->SetSprite(spriteComponent);
	idleState->SetFygar(fygarComponent);
	idleState->SetMover(gridMover);
	stateComponent->SetCurrentState(idleState);

	ColliderComponent* colliderComponent{ new ColliderComponent{ fygar, "Fygar", 30.f, 30.f } };
	colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{
		[colliderComponent, fygarComponent]() {
		if (colliderComponent->GetCollisionHit()->GetGameObject()->CompareTag("Player") && !fygarComponent->IsDeflating())
			colliderComponent->GetCollisionHit()->GetGameObject()->GetComponent<Player>()->ChangeLives(-1); } });

	fygar->SetTag("Fygar");
	fygar->AddComponent(colliderComponent);
	fygar->AddComponent(spriteComponent);
	fygar->AddComponent(inputComponent);
	fygar->AddComponent(fygarComponent);
	fygar->AddComponent(stateComponent);
	scene->AddGameObject(fygar);

	NextLevelObserver* ob{ FObserverManager.Get<NextLevelObserver>() };
	ob->AddEnemy();
	fygarComponent->AddObserver(ob);
}

void DigDug::Game::InitEndScene()
{
	using namespace flgin;
	
	UINT scoreP1{ FInputManager.GetPlayer(0)->GetGameObject()->GetComponent<Player>()->GetScore() };
	UINT scoreP2{ m_GameState == GameState::Coop ? FInputManager.GetPlayer(1)->GetGameObject()->GetComponent<Player>()->GetScore() : 0 };

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

	if (m_GameState == GameState::Coop)
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

void DigDug::Game::InitNextLevel()
{
	/*
	using namespace flgin;
	Scene* scene{ FSceneManager.CreateScene("GameScene" + std::to_string(m_Level)) };
	FInvoker.CancelAllInvokes();
	FInputManager.ClearCommands();
	FCollisionManager.ClearColliders();

	UINT scoreP1{ FInputManager.GetPlayer(0)->GetGameObject()->GetComponent<Player>()->GetScore() };
	UINT scoreP2{ m_GameState == GameState::Coop ? FInputManager.GetPlayer(1)->GetGameObject()->GetComponent<Player>()->GetScore() : 0 };
	UINT livesP1{ FInputManager.GetPlayer(0)->GetGameObject()->GetComponent<Player>()->GetLives() };
	UINT livesP2{ m_GameState == GameState::Coop ? FInputManager.GetPlayer(1)->GetGameObject()->GetComponent<Player>()->GetLives() : 0 };

	FSceneManager.RemoveCurrentScene();
	FSceneManager.ActivateSceneByName("MenuScene");


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
	Fygar::SetGrid(grid);
	go->AddComponent(grid);
	go->AddComponent(new GridRenderer{ go, scene, grid });
	scene->AddGameObject(go);

	go = new GameObject{};
	SpriteComponent* livesSprite{ scene->CreateSpriteComponent(go) };
	livesSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texLives")));
	LivesObserver* livesObserver{ new LivesObserver{ 3, livesSprite } };
	go->SetPosition(0.f, 425.f);
	scene->AddGameObject(go);

	go = new GameObject{};
	RenderComponent* scoreRenderer{ scene->CreateRenderComponent(go) };
	go->AddComponent(scoreRenderer);
	TextComponent* textRenderer{ new TextComponent{ go, FLocalizer.Get("fontDefault"), 30, {255, 255, 255} } };
	go->AddComponent(textRenderer);
	ScoreObserver* scoreObserver{ };
	go->SetPosition(170.f, 425.f);
	scene->AddGameObject(go);

	GameObject* pump{ new GameObject{} };
	pump->SetTag("Pump");
	SpriteComponent* pumpSprite{ scene->CreateSpriteComponent(pump) };
	pumpSprite->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPump")));
	pumpSprite->SetDimensions(180.f, 18.f);
	pumpSprite->SetSpriteInfo(1, 10, 180.f, 18.f, 0.1f, true);
	FreeMover* pumpMover{ new FreeMover{pump, 0.f} };
	ColliderComponent* pumpCollider{ new ColliderComponent{pump, "Pump", 30.f, 30.f} };
	pump->AddComponent(pumpMover);
	pump->AddComponent(pumpSprite);
	pump->AddComponent(pumpCollider);
	scene->AddGameObject(pump);

	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	InputComponent* inputComponent{ FInputManager.GetPlayer(0) };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid, 90.f, true } };
	go->AddComponent(gridMover);
	inputComponent->Clear();
	QuitCommand* quitCommand{ new QuitCommand{} };
	ReturnToMenuCommand* returnCommand{ new ReturnToMenuCommand{*this} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };
	FireCommand* fireCommand{ new FireCommand{} };

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_BACK, returnCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_X, fireCommand);

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);

	AxisRange range{};
	range.max = std::numeric_limits<short int>::max();
	range.min = 15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveDown);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveRight);
	range.min = std::numeric_limits<short int>::min();
	range.max = -15000;
	range.wasReached = false;
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTY, range, gridMoveUp);
	inputComponent->AddAxisMapping(SDL_CONTROLLER_AXIS_LEFTX, range, gridMoveLeft);

	inputComponent->AddKeyboardMapping(SDLK_d, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_a, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_s, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_w, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);
	inputComponent->AddKeyboardMapping(SDLK_r, returnCommand);
	inputComponent->AddKeyboardMapping(SDLK_q, fireCommand);

	SpriteComponent* spriteComponent{ scene->CreateSpriteComponent(go, 4) };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(6, 1, 30.0f, 30.0f, 0.4f);
	spriteComponent->SetDimensions(30.0f, 30.0f);

	Player* playerComponent{ new Player{ go, pump } };
	playerComponent->AddObserver(livesObserver);
	playerComponent->AddObserver(scoreObserver);
	playerComponent->AddObserver(new GameOverObserver{ *this });
	playerComponent->SetInitPos(go->GetPosition().x, go->GetPosition().y);

	//TODO: REMOVE TEST CODE
	DieCommand* angryToggleCommand{ new DieCommand{playerComponent} };
	inputComponent->AddKeyboardMapping(SDLK_e, angryToggleCommand);
	inputComponent->AttachToGameObject(go);
	NPCFygar::Create(scene, { 300.f, 40.f });
	FCollisionManager.AddIgnore("Flame", "Fygar");

	StateComponent* stateComponent{ new StateComponent{ go } };
	IdleState* idleState{ new IdleState{} };
	idleState->SetAttachedSprite(spriteComponent);
	idleState->SetPlayer(playerComponent);
	stateComponent->SetCurrentState(idleState);

	ColliderComponent* colliderComponent{ new ColliderComponent{ go, "Player", 30.f, 30.f } };
	colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{ []() {} });
	pumpCollider->SetOnCollisionFunction(new FunctionHolder<void>{
	[playerComponent, pumpCollider, pumpSprite, pumpMover, stateComponent]()
{
	if (pumpCollider->GetCollisionHit()->GetGameObject()->CompareTag("Fygar"))
	{
		Fygar* hit{ pumpCollider->GetCollisionHit()->GetGameObject()->GetComponent<Fygar>() };
		if (hit->IsBloating()) return;
		hit->SetBloating(true);
		hit->SetHitBy(playerComponent);
		pumpSprite->StopAnimating();
		pumpMover->SetMoving(false);
		FInvoker.CancelOwnedInvokes(stateComponent->GetCurrentState());
		static_cast<FiringState*>(stateComponent->GetCurrentState())->SetHit(hit);
	}
	else
		playerComponent->SetFiring(false); } });
	FCollisionManager.AddIgnore("Pump", "Player");

	go->SetTag("Player");
	go->AddComponent(colliderComponent);
	go->AddComponent(spriteComponent);
	go->AddComponent(inputComponent);
	go->AddComponent(playerComponent);
	go->AddComponent(stateComponent);
	scene->AddGameObject(go);

	Rock::Create(scene, grid->GetNodeNearestTo(150.f, 150.f), FLocalizer.Get("texRock"));

	RenderComponent* renComp{};
	for (unsigned int i{ 0 }; i < grid->GetGridSize() - 21; ++i)
	{
		if (rand() % 4 != 0)
		{
			if (grid->GetGrid()[i + 21].IsBlocked()) continue;
			grid->GetGrid()[i + 21].SetBlocked(true);
			go = new GameObject{};
			renComp = scene->CreateRenderComponent(go, 2);
			renComp->SetPositionOffset(-15.f, -15.f);
			renComp->SetTexture(FResourceManager.LoadTexture(FLocalizer.Get("texDirt" + std::to_string(i * 4 / (grid->GetGridSize() - 21)))));

			colliderComponent = new ColliderComponent{ go, "Dirt", 7.5f, 7.5f, 15.f, 15.f };
			colliderComponent->SetOnCollisionFunction(new FunctionHolder<void>{ [go, grid, colliderComponent]() { if (!colliderComponent->GetCollisionHit()->GetGameObject()->CompareTag("Player")) return; go->SetActive(false); grid->GetNodeNearestTo(go->GetPosition().x, go->GetPosition().y)->SetBlocked(false); } });
			FCollisionManager.AddIgnore("Dirt", "Dirt");

			go->SetTag("Dirt");
			go->AddComponent(colliderComponent);
			go->AddComponent(renComp);
			go->SetPosition(grid->GetGrid()[i].GetPosition().x, grid->GetGrid()[i + 21].GetPosition().y);
			scene->AddGameObject(go);
		}
	}
	*/
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

void DigDug::Game::SetSwappingToNextLevel()
{
	FSceneManager.SwapScene(new flgin::FunctionHolder<void>{ std::bind(&Game::InitNextLevel, this) });
}
