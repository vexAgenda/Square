#include "ObjectManager.h"
#include <algorithm>

ObjectManager::ObjectManager() : _objects{}
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::AddObject(std::shared_ptr<GameObject> object)
{
	_objects.push_back(object);
}

void ObjectManager::DeleteObject(std::shared_ptr<GameObject> object)
{
	_objects.erase(std::remove(_objects.begin(),_objects.end()
	,object),_objects.end());
}

std::shared_ptr<GameObject> ObjectManager::find(const std::string& textureName)
{
	for (auto object : _objects)
	{
		if (object->textureName() == textureName)
		{
			return object;
		}
	}
	return nullptr;
}
