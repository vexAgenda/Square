#pragma once
#include <SDL/SDL.h>

#include <SDL/SDL_image.h>
#include <string>
#include <memory>
#include <queue>
#include <functional>

struct Vector2
{
	int x;
	int y;
};

struct Vector2F
{
	float x;
	float y;
};

enum class MoveType
{
	DEFAULT,
	SQUARE,
	END
};

enum class Fade
{
	NONE,
	FADE_IN,
	FADE_OUT
};
class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name);
	virtual ~GameObject();

	virtual bool LoadImage(SDL_Renderer* renderer, const std::string& str);

	Fade fadeType() { return _fadeType; }
	void SetFadeType(Fade type) { _fadeType = type; }
	int currentFade() { return _currentFade; }
	void SetCurrentFade(int amount) { _currentFade = amount; }
	void SetFadeAmount(int amount) { _fadeAmount = amount; }
	int fadeAmount() { return _fadeAmount; }
	void InitFade(Fade fadeType, int currentFade, int amount);

	std::string objectName() { return _objectName; }
	SDL_Texture* texture() { return _texture; }
	SDL_Rect imageRect() { return _imageRect; }
	SDL_FRect posRect() { return _posRect; }
	SDL_FRect hitbox() { return _hitbox; }
	void SetHitbox(const SDL_FRect& rect)
	{
		_hitbox = rect;
	}

	//Move related function
	void InitMove(const Vector2F& initPos, const Vector2F& velocity,const MoveType& mType);
	void SetPos(const Vector2F& pos);
	void SetVelocity(const Vector2& vel,bool isStatic = true);
	void SetMoveType(const MoveType& mType);
	void Move(float deltaTime);
	void MoveTargetted(float deltaTime);
	bool isTargetEmpty() { return _targetCoords.empty(); }
	void PushTarget(const Vector2F& pos);
	bool is_hover(const Vector2& mouse);

private:
	//move related function
	void MoveDefault(float deltaTime);
	void MoveExponential(float deltaTime);
protected:
	std::string _objectName{};
	std::string _fileName{};

	SDL_Texture* _texture{ nullptr };
	SDL_Rect _imageRect{};
	SDL_FRect _posRect{};
	SDL_FRect _hitbox{};
	//texture fade
	int _fadeAmount{ 0 };
	int _currentFade{ 0 };
	Fade _fadeType{ Fade::NONE };

	//move related
	bool _isTargetted{ false };
	std::queue<Vector2F> _targetCoords;
	std::queue<Vector2> _velocity;
	MoveType _moveType{ MoveType::END };


};
