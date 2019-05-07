#pragma once
#include <string>
#include <unordered_map>
#include "Singleton.h"
#define FTextLocalizer flgin::TextLocalizer::GetInstance()

namespace flgin
{
	class TextLocalizer final : public Singleton<TextLocalizer>
	{
	public:
		void Load(std::string&& filename, std::string&& locale);
		const std::string& Get(std::string&& key);
		
		TextLocalizer() = default;
		~TextLocalizer() = default;
		TextLocalizer(TextLocalizer&&) = delete;
		TextLocalizer(const TextLocalizer&) = delete;
		TextLocalizer& operator=(TextLocalizer&&) = delete;
		TextLocalizer& operator=(const TextLocalizer&) = delete;

	private:
		std::unordered_map<std::string, std::string> m_Localizations;
	};
}
