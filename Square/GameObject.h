#pragma once

#include <SDL/SDL_image.h>
#include <string>
#include <memory>

#include "ObjectEssentials.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name);
	virtual ~GameObject();

	virtual bool LoadImage(SDL_Renderer* renderer, const std::string& str);

	std::string name() { return _objectName; }
	std::string fileName() { return _fileName; }

	void SetFileName(const std::string& name)
	{
		_fileName = name;
	}
	void SetPos(const Vector2F& pos) 
	{
		_posRect.x = pos.x;
		_posRect.y = pos.y;
	};
	void SetHitbox(const SDL_FRect& rect)
	{
		_hitbox = rect;
	}

	SDL_FRect& posRect() {
		return const_cast<SDL_FRect&>(
			static_cast<const GameObject&>(*this).posRect()
			);
	}
	const SDL_FRect& posRect() const { return _posRect; }
	SDL_FRect& hitbox() {
		return const_cast<SDL_FRect&>(
			static_cast<const GameObject&>(*this).hitbox()
			);
	}
	const SDL_FRect& hitbox() const { return _hitbox; }

	double& angle() {
		return const_cast<double&>(
			static_cast<const GameObject&>(*this).angle());
	}
	const double& angle() const { return _angle; }

	template <typename T>
	std::weak_ptr<T> GetComponent()
	{
		for (auto component : _components)
		{
			if (auto castedComponent = std::dynamic_pointer_cast<T>(component))
				return castedComponent;
		}
		return std::weak_ptr<T>{};
	}
protected:
	std::vector<std::shared_ptr<Component>> _components;
	std::string _objectName{};
	std::string _fileName{};

	SDL_FRect _posRect{};
	SDL_FRect _hitbox{};
	double _angle{ 0.f };

};
