#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void AddObject(std::shared_ptr<GameObject>);
	void DeleteObject(std::shared_ptr<GameObject>);

	auto& operator[](int i) { return _objects[i]; }
	const std::vector<std::shared_ptr<GameObject>> objects() const
	{
		return _objects;
	}
private:
	std::vector<std::shared_ptr<GameObject>> _objects;

};

