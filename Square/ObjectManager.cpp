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