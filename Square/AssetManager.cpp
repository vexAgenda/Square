#include "AssetManager.h"

#include "SDL/SDL_image.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{
	for (const auto& map : _textures)
		SDL_DestroyTexture(map.second);
	for (const auto& map : _fonts)
		TTF_CloseFont(reinterpret_cast<TTF_Font*>(map.second));
}

T AssetManager::GetAsset(const std::string& fileName)
{

	if constexpr (std::is_same_v<T, SDL_Texture*>)
	{
		auto it = _textures.find(fileName);
		if (it != _textures.end())
			return it->second;
		else
		{
			if ()
			{

			}
			else
				return nullptr;
		}
	}
}

bool AssetManager::Fetch(const std::string* fileName)
{

	return true;
}
