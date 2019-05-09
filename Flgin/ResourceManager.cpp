#include "FlginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <algorithm>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

flgin::ResourceManager::~ResourceManager()
{
	for (std::pair<const std::string, Texture2D*>& texPair : m_pTextures)
		FLogger.SafeDelete(texPair.second);

	for (std::pair<const std::string, Font*>& fontPair : m_pFonts)
		FLogger.SafeDelete(fontPair.second);
}

void flgin::ResourceManager::Init(const std::string& dataPath)
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
	if (m_pTextures.find(file) == m_pTextures.cend())
	{
		std::string fullPath{ m_DataPath + file };
		SDL_Texture* texture{ IMG_LoadTexture(FRenderer.GetSDLRenderer(), fullPath.c_str()) };
		if (texture == nullptr)
		{
			FLogger.Log(StatusCode{ StatusCode::Status::FAIL, std::string("Failed to load texture: ") + SDL_GetError() });
			return nullptr;
		}
		m_pTextures[file] = new Texture2D{ texture };
	}

	return m_pTextures[file];
}

flgin::Font* flgin::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	auto it{ std::find_if(m_pFonts.cbegin(), m_pFonts.cend(),
		[file, size](const std::pair<const std::string, Font*>& el) { return (el.first == file) && (el.second->GetSize() == size);  }) };

	if (it == m_pFonts.cend())
	{
		std::string fullPath{ m_DataPath + file };
		it = m_pFonts.emplace(file, new Font{ fullPath, size });
	}

	return it->second;
}
