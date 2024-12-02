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

