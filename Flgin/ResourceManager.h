#pragma once
#include "Singleton.h"
#include <unordered_map>
#define FResourceManager flgin::ResourceManager::GetInstance()

namespace flgin
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager() = default;
		~ResourceManager();

		void Init(const std::string& data);

		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);

	private:
		std::string m_DataPath;
		std::unordered_map<std::string, Texture2D*> m_pTextures;
		std::unordered_multimap<std::string, Font*> m_pFonts;
	};
}
