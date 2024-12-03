#pragma once

#include <vector>
#include <memory>
#include <string>
#include "GameObject.h"
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void AddObject(std::shared_ptr<GameObject> object);
	void DeleteObject(std::shared_ptr<GameObject>);
	void Flush();

	auto& operator[](int i) { return _objects[i]; }
	std::shared_ptr<GameObject> find(const std::string& textureName);
	bool name_contains(std::shared_ptr<GameObject> obj,const std::string& name);
	std::vector<std::shared_ptr<GameObject>>& objects()
	{
		return _objects;
	}
private:
	std::vector<std::shared_ptr<GameObject>> _objects;

};

