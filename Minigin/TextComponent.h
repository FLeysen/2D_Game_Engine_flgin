#pragma once
#include "BaseComponent.h"
#include "Font.h"
class TextComponent final : public BaseComponent
{
public:
	TextComponent(dae::GameObject* const ownerObject, const std::string& fontPath, const unsigned int fontSize, const SDL_Color& color = { 255,255,255 }, const std::string& text = "");
	~TextComponent();
	void Update() override;
	void SetText(const std::string& text);

	TextComponent(const TextComponent&) = delete;
	TextComponent(TextComponent&&) = delete;
	TextComponent& operator=(const TextComponent&) = delete;
	TextComponent& operator=(TextComponent&&) = delete;

private:
	std::string m_Text;
	dae::Font* m_pFont;
	SDL_Color m_TextColor;
};