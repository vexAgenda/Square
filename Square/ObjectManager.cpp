#include "ObjectManager.h"
#include <algorithm>

ObjectManager::ObjectManager() : _objects{}
{
}

ObjectManager::~ObjectManager()
{
	Flush();
}

void ObjectManager::AddObject(std::shared_ptr<GameObject> object)
{
	if (!object) return;
	_objects.push_back(object);
}

void ObjectManager::DeleteObject(std::shared_ptr<GameObject> object)
{
	auto it = std::find(_objects.begin(), _objects.end(), object);
	if (it != _objects.end())
	{
		_objects.erase(it);
	}

}

void ObjectManager::Flush()
{
	_objects.clear();
}

std::shared_ptr<GameObject> ObjectManager::find(const std::string& objectName)
{
	for (auto object : _objects)
	{
		if (object->objectName() == objectName)
		{
			return object;
		}
	}
	return nullptr;
}

bool ObjectManager::name_contains(std::shared_ptr<GameObject> object, const std::string& name)
{
	return (object->objectName().contains(name));
}

