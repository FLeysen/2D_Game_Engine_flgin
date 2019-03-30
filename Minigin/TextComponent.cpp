#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "RenderComponent.h"

TextComponent::TextComponent(dae::GameObject* const ownerObject, const std::string& fontPath, const unsigned int fontSize, const SDL_Color& color, const std::string& text)
	: BaseComponent(ownerObject)
	, m_pFont{ new dae::Font{fontPath, fontSize} }
	, m_TextColor{ color }
	, m_Text{ text }
{}

TextComponent::~TextComponent()
{
	Logger::GetInstance().SafeDelete(m_pFont);
}

void TextComponent::Update()
{

	RenderComponent* const renderComponent{ m_pOwnerObject->GetComponent<RenderComponent>() };
	if (renderComponent == nullptr)
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::WARNING, "TextCompoment could not find attached RenderComponent.", this });
		return;
	}

	SDL_Surface* const surface{ TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_TextColor) };
	if (surface == nullptr)
	{
		Logger::GetInstance().Log(StatusCode{StatusCode::Status::FAIL, std::string("Render text failed: ") + SDL_GetError(), this});
		return;
	}
	SDL_Texture* const texture{ SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surface) };
	if (texture == nullptr)
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, std::string("Create text texture from surface failed: ") + SDL_GetError(), this });
		return;
	}
	SDL_FreeSurface(surface);

	renderComponent->SetTexture(new dae::Texture2D{ texture });
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
}