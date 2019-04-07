#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

void flgin::ResourceManager::Init(std::string&& dataPath)
{
	m_DataPath = std::move(dataPath);

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

flgin::Texture2D* flgin::ResourceManager::LoadTexture(const std::string& file)
{
	std::string fullPath = m_DataPath + file;
	SDL_Texture *texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, std::string("Failed to load texture: ") + SDL_GetError() });
		return nullptr;
	}
	return new Texture2D{ texture };
}

std::shared_ptr<flgin::Font> flgin::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	return std::make_shared<Font>(m_DataPath + file, size);
}
