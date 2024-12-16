#pragma once

#include <SDL/SDL_image.h>
#include <string>
#include <memory>
#include <queue>

#include "ObjectEssentials.h"

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name);
	virtual ~GameObject();

	bool MakeRect(SDL_Renderer* renderer, const SDL_Rect& rect,const SDL_Color& rectColor);
	virtual bool LoadImage(SDL_Renderer* renderer, const std::string& str);

	Fade fadeType() { return _fadeType; }
	void SetFadeType(Fade type) { _fadeType = type; }
	const int currentFade() { return _currentFade; }
	void SetCurrentFade(int amount) { _currentFade = amount; }
	void SetFadeAmount(int amount) { _fadeAmount = amount; }
	constexpr int fadeAmount() { return _fadeAmount; }
	void InitFade(Fade fadeType, int currentFade, int amount);
	void SetVisible(bool visible) { _visible = visible; }

	std::string objectName() { return _objectName; }
	std::string fileName() { return _fileName; }
	SDL_Color rectColor(){ return _rectColor; }
	SDL_Texture* texture() { return _texture; }
	const SDL_Rect imageRect() const { return _imageRect; }
	const SDL_FRect posRect() const { return _posRect; }
	const SDL_FRect hitbox() const { return _hitbox; }
	const bool visible() const { return _visible; }
	const bool isActive() const { return _active; }
	void Activate() { _active = true; }
	void Deactivate() { _active = false; }
	const double angle() { return _angle; }
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

	// rotate related function
	void Rotate();
	void SetRotateAmount(int degree) { _rotate_amount = degree; }

private:
	//move related function
	void MoveDefault(float deltaTime);
	void MoveExponential(float deltaTime);
protected:
	std::string _objectName{};
	std::string _fileName{};

	SDL_Color _rectColor{0,0,0};

	SDL_Texture* _texture{ nullptr };
	SDL_Rect _imageRect{};
	SDL_FRect _posRect{};
	SDL_FRect _hitbox{};
	double _angle{0.f};

	bool _active{ true };
	//texture fade
	int _fadeAmount{ 0 };
	int _currentFade{ 0 };
	Fade _fadeType{ Fade::NONE };
	bool _visible{ true };

	//move related
	bool _isTargetted{ false };
	std::queue<Vector2F> _targetCoords;
	std::queue<Vector2> _velocity;
	MoveType _moveType{ MoveType::END };

	//rotate related
	int _rotate_amount{ 0 };


};
