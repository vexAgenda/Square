#pragma once
#include <SDL/SDL.h>

#include <SDL/SDL_image.h>
#include <string>

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

	bool LoadImage(SDL_Renderer* renderer,const std::string& str);

	Fade fadeType() { return _fadeType; }
	void SetFadeType(Fade type) { _fadeType = type; }
	int currentFade() { return _currentFade; }
	void SetCurrentFade(int amount) { _currentFade = amount; }
	void SetFadeAmount(int amount) { _fadeAmount = amount; }
	int fadeAmount() { return _fadeAmount; }
	void InitFade(Fade fadeType, int currentFade, int amount);

	//move related functions
	void InitMove(const Vector2F& initPos,const Vector2F& velocity, const MoveType& moveType,Vector2F* target = nullptr);
	void Move(float deltaTick);

	void SetPos(int x, int y)
	{
		_posRect.x = x;
		_posRect.y = y;
		_hitbox.x = x;
		_hitbox.y = y;
	}

	void SetMoveVelocity(const Vector2& pos)
	{
		_velocity = pos;
		SDL_Log("%s's MoveVelocity : (%d,%d)", _objectName.c_str(), _velocity.x, _velocity.y);

	}
	void SetMoveType(const MoveType& moveType)
	{
		_moveType = moveType;
	}
	void SetMoveTarget(const Vector2F& target);
	bool MoveTargetDone() { SDL_Log("%s : %f, %f ",_objectName.c_str(),_posRect.x,_posRect.y); return _targetPos->x == _posRect.x && _targetPos->y == _posRect.y; }

	void ClearMoveTarget()
	{
		delete _targetPos;
		_targetPos = nullptr;
	}

	SDL_Texture* texture() { return _texture;  }
	SDL_Rect imageRect() { return _imageRect; }
	SDL_FRect posRect() { return _posRect; }
	SDL_FRect hitbox() { return _hitbox; }
	void SetHitbox(const SDL_FRect& frect)
	{
		_hitbox = frect;
	}
	std::string objectName() { return _objectName; }

	bool isMouseCollide(const Vector2& mouse);

protected:
	void MoveDefault(float deltaTick);
	void MoveExponential(float deltaTick);

	SDL_Texture* _texture{ nullptr };
	SDL_Rect _imageRect{};
	SDL_FRect _posRect{};
	SDL_FRect _hitbox{};
	//texture fade
	int _fadeAmount{ 0 };
	int _currentFade{ 0 };
	Fade _fadeType{ Fade::NONE };
	// move related
	Vector2 _velocity{ 0,0 };
	Vector2F* _targetPos{ nullptr };
	MoveType _moveType{ MoveType::DEFAULT };
	std::string _objectName{};

};
