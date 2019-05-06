#include "FlginPCH.h"
#include "Font.h"

flgin::Font::Font(const std::string& fullPath, unsigned int size)
	: m_pFont{ nullptr }
{
	m_pFont = TTF_OpenFont(fullPath.c_str(), size);
	if (m_pFont == nullptr)
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, std::string("Failed to load font: ") + SDL_GetError(), this });
}

flgin::Font::~Font()
{
	if (m_pFont) TTF_CloseFont(m_pFont);
}

TTF_Font* flgin::Font::GetFont() const
{
	return m_pFont;
}