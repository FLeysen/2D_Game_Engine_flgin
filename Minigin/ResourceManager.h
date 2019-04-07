#pragma once
#include "Singleton.h"

namespace flgin
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager() = default;

		void Init(std::string&& data);

		Texture2D* LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size);

	private:
		std::string m_DataPath;
	};
}
