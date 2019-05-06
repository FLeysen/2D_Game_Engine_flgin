#include "FlginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

flgin::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* flgin::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

flgin::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
