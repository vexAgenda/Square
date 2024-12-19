#pragma once

#include "Component.h"

#include "SDL/SDL.h"
enum class Fade
{
	NONE,
	IN,
	OUT
};

class SpriteComponent : public Component
{
public:
	SpriteComponent(std::shared_ptr<GameObject>const&,int);
	virtual ~SpriteComponent() override;

	//Fade fadeType, int currentFade, int amount
	virtual bool init(const std::vector<std::any>&)override;
	virtual void draw(SDL_Renderer* renderer);

	void SetTexture(SDL_Texture* texture);
	void SetBackroundRect(const Vector2F& widthHeight);
	void SetBackground(SDL_Renderer* renderer,SDL_Color rectColor);
	//Fade
	Fade fadeType() { return _fadeType; }
	void SetFadeType(Fade type) { _fadeType = type; }
	const int currentFade() { return _currentFade; }
	void SetCurrentFade(int amount) { _currentFade = amount; }
	void SetFadeAmount(int amount) { _fadeAmount = amount; }
	constexpr int fadeAmount() { return _fadeAmount; }

	void SetVisible(bool visible) { _visible = visible; }
	const bool visible() const { return _visible; }
private:
	void drawTextureFade();
protected:
	SDL_Color _rectColor{0,0,0,0};
	SDL_Texture* _texture{ nullptr };
	SDL_Texture* _background{ nullptr };
	//texture fade
	int _fadeAmount{ 0 };
	int _currentFade{ 0 };
	Fade _fadeType{ Fade::NONE };
	bool _visible{ true };
};

