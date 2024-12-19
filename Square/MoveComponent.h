#pragma once

#include <queue>

#include "Component.h"
#include "ObjectEssentials.h"

class MoveComponent : public Component
{
public:
	MoveComponent(std::shared_ptr<GameObject> const&, int);
	virtual ~MoveComponent() override;

	// const Vector2F& initPos, const Vector2F& velocity, const MoveType& mType
	virtual bool init(const std::vector<std::any>&) override;
	void SetVelocity(const Vector2F& vel, bool hasOnlyOne = true);
	//void accelerate(const Vector2F& vel);
	//void deaccelerate(const Vector2F& vel);
	void SetRotateAmount(float degree) { _rotateAmount = degree; }
	void stop() { _canMove = false; }
	void go() { _canMove = true; }

	bool hasNoTarget() { return _targetCoords.empty(); }

	virtual void update(float deltaTime) override;
private:
	void Move(float deltaTime);
	void MoveToTarget(float deltaTime);
	bool isTargetEmpty() { return _targetCoords.empty(); }
	void Rotate(float deltaTime);
	void PushTarget(const Vector2F& pos);
private:
	bool _isTargetted{ false };
	bool _canMove{true};
	std::queue<Vector2F> _targetCoords;
	std::queue<Vector2F> _velocity;

	float _rotateAmount{ 0 };
};

