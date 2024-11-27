#pragma once
#include <SDL/SDL.h>

#include <SDL/SDL_image.h>
#include <string>

struct Pos
{
	int x;
	int y;
};

enum class MoveType
{
	DEFAULT,
	EXPONENT,
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
	void InitMove(const Pos& initPos,const Pos& velocity, const MoveType& moveType,Pos* target = nullptr);
	void Move(float deltaTick);

	void SetPos(int x, int y)
	{
		_posRect.x = x;
		_posRect.y = y;
	}

	void SetMoveVelocity(const Pos& pos)
	{
		if (_targetPos  && _posRect.x + _velocity.x >= _targetPos->x
			&& _posRect.y + _velocity.y >= _targetPos->y)
			_velocity = { 0,0 };
		else
			_velocity = pos;

	}
	void SetMoveType(const MoveType& moveType)
	{
		_moveType = moveType;
	}
	void SetMoveTarget(const Pos& target);
	bool MoveTargetDone() { SDL_Log("%s : %d, %d ",_objectName.c_str(),_posRect.x,_posRect.y); return _targetPos->x == _posRect.x && _targetPos->y == _posRect.y; }

	void ClearMoveTarget()
	{
		delete _targetPos;
		_targetPos = nullptr;
	}

	SDL_Texture* texture() { return _texture;  }
	SDL_Rect imageRect() { return _imageRect; }
	SDL_Rect posRect() { return _posRect; }
	std::string objectName() { return _objectName; }

	bool isMouseCollide(const Pos& mouse);

private:
	void MoveDefault(float deltaTick);
	void MoveExponential(float deltaTick);

	SDL_Texture* _texture{ nullptr };
	SDL_Rect _imageRect{};
	SDL_Rect _posRect{};
	//texture fade
	int _fadeAmount{ 0 };
	int _currentFade{ 0 };
	Fade _fadeType{ Fade::NONE };
	// move related
	Pos _velocity{ 0,0 };
	Pos* _targetPos{ nullptr };
	MoveType _moveType{ MoveType::DEFAULT };
	std::string _objectName{};

};
