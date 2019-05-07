#include "FlginPCH.h"
#include "Flgin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "Logger.h"
#include "Time.h"
#include "Invoker.h"
#include "InputComponent.h"
#include "PlayerCommands.h"
#include "MovementGrid.h"
#include "GridRenderer.h"
#include "GridMovementComponent.h"
#include "MovementCommands.h"
#include "SpriteComponent.h"
#include "PathfinderComponent.h"
#include "TextLocalizer.h"

void flgin::Flgin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, std::string("SDL_Init Error: ") + SDL_GetError(), this });
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	SDL_GameController* controller{ };

	for (int i{}, amt{ SDL_NumJoysticks() }; i < amt; ++i)
	{
		if (SDL_IsGameController(i))
			controller = SDL_GameControllerOpen(i);
	}

	m_pWindow = SDL_CreateWindow(
		"Flgin",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, std::string("SDL_CreateWindow Error: ") + SDL_GetError(), this });
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	FRenderer.Init(m_pWindow);
}

void flgin::Flgin::LoadGame() const
{
	Scene& scene = FSceneManager.CreateScene("Demo");

	GameObject* go{ new flgin::GameObject{} };
	RenderComponent* renderComponent{ new RenderComponent{ go, scene, 0 } };
	renderComponent->SetTexture(FResourceManager.LoadTexture(FTextLocalizer.Get("texBg")));
	go->AddComponent(renderComponent);
	scene.AddGameObject(go);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go, scene, 0 };
	renderComponent->SetTexture(FResourceManager.LoadTexture(FTextLocalizer.Get("texLogo")));
	go->AddComponent(renderComponent);
	go->SetPosition(216, 180);
	scene.AddGameObject(go);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go, scene, 3 };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FTextLocalizer.Get("fontDefault"), 36, {255, 0,0 }, FTextLocalizer.Get("stringAssignment")});
	go->SetPosition(80.f, 20.f);
	scene.AddGameObject(go);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go, scene, 4 };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, FTextLocalizer.Get("fontDefault"), 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene.AddGameObject(go);

	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	MovementGrid* grid{ new MovementGrid{ go, 16, 21, 30.0f } };
	go->AddComponent(grid);
	go->AddComponent(new GridRenderer{ go, scene, grid });
	scene.AddGameObject(go);

	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	InputComponent* inputComponent{ new InputComponent{ go } };
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
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);

	SpriteComponent* spriteComponent{ new SpriteComponent{ go, scene, 2 } };
	spriteComponent->SetTexture(FResourceManager.LoadTexture(FTextLocalizer.Get("texPlayer")));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(4, 1, 30.0f, 30.0f, 1.0f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	go->AddComponent(spriteComponent);
	go->AddComponent(gridMover);
	go->AddComponent(inputComponent);
	scene.AddGameObject(go);

	go = new GameObject{};
	inputComponent = new InputComponent{ go };
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_B, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 1 });
	go->AddComponent(inputComponent);
	scene.AddGameObject(go);

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

void flgin::Flgin::Cleanup()
{
	FRenderer.Destroy();
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void flgin::Flgin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	FResourceManager.Init(FTextLocalizer.Get("dataPath"));

	LoadGame();
	{
		float lag{ 1.0f };
		auto lastTime{ std::chrono::high_resolution_clock::now() };
		Renderer& renderer{ FRenderer };
		SceneManager& sceneManager{ FSceneManager };
		InputManager& input{ FInputManager };
		Invoker& invoker{ FInvoker };
		Time& time{ FTime };
		bool doContinue{ true };
		float frameTime{ m_MsPerFrame / 1000.0f };
		time.SetFixedTime(frameTime);
		time.SetTimeScale(1.0f);

		while (doContinue)
		{
			auto currTime{ std::chrono::high_resolution_clock::now() };
			float deltaTime{ std::chrono::duration<float>(currTime - lastTime).count() };
			lastTime = currTime;
			lag += deltaTime;
			time.SetDeltaTime(deltaTime);

			doContinue = input.ProcessInput();

			invoker.Update();
			sceneManager.Update();

			while (lag >= frameTime)
			{
				//FixedUpdate functions are called here
				lag -= frameTime;
			}

			renderer.Render();
		}
	}

	Cleanup();
}
