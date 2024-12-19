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

template<typename T>
T AssetManager::GetAsset(SDL_Renderer* renderer,const std::string& fileName)
{

	if constexpr (std::is_same_v<T, SDL_Texture*>)
	{
		auto it = _textures.find(fileName);
		if (it != _textures.end())
			return it->second;
		else
			return Fetch<T>(renderer, fileName);
	}
	else if constexpr (std::is_same_v<T, TTF_Font*>)
	{
		auto it = _fonts.find(fileName);
		if (it != _fonts.end())
			return it->second();
		else
			return Fetch<T>(renderer, fileName);
	}
}

template <typename T>
T AssetManager::Fetch(SDL_Renderer* renderer,const std::string& fileName)
{
	if constexpr (std::is_same_v<T,SDL_Texture*>)
	{
		SDL_Surface* _surface = IMG_Load(fileName.c_str());
		if (!_surface)
		{
			SDL_Log("%s does not exist!", fileName.c_str());
			return nullptr;
		}
		SDL_Log("Loaded Image from %s", fileName.c_str());
		_textures[fileName] = SDL_CreateTextureFromSurface(renderer, _surface);
		if (_textures[fileName] == nullptr)
			SDL_Log("Failed to Create Texture : %s", SDL_GetError());
		SDL_FreeSurface(_surface);
		return _textures[fileName];
	}
	else if constexpr(std::is_same_v<T,TTF_Font*>)
	{
		_fonts[fileName] = TTF_OpenFont(fileName.c_str(), 0);
		if (!_fonts[fileName])
			 SDL_Log("Failed to Create Texture : %s", SDL_GetError());
		return _fonts[fileName];
	}
	return nullptr;
}
