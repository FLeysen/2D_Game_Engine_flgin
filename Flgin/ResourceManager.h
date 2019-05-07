#pragma once
#include "Singleton.h"
#define FResourceManager flgin::ResourceManager::GetInstance()

namespace flgin
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager() = default;

		void Init(const std::string& data);

		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);

	private:
		std::string m_DataPath;
	};
}
