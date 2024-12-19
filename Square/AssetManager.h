#pragma once
#include <unordered_map>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct TTF_Font;

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	template<typename T>
	T GetAsset(const std::string& fileName);
private:
	bool Fetch(const std::string* fileName);
private:
	std::unordered_map<std::string, SDL_Texture*> _textures;
	std::unordered_map<std::string, TTF_Font*> _fonts;
};

