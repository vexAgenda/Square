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
	_findMap[object->objectName()] = object;
}

void ObjectManager::DeleteObject(std::shared_ptr<GameObject> object)
{
	auto it = std::find(_objects.begin(), _objects.end(), object);
	if (it != _objects.end())
	{
		_findMap.erase(object->objectName());
		_objects.erase(it);
	}
}

void ObjectManager::Flush()
{
	_objects.clear();
}

void ObjectManager::SetVisibleAll(bool visible)
{
	for (auto object : _objects)
		object->SetVisible(visible);
}

void ObjectManager::ActivateAll()
{
	for (auto object : _objects)
		object->Activate();
}

void ObjectManager::DeactivateAll()
{
	for (auto object : _objects)
		object->Deactivate();
}

std::shared_ptr<GameObject> ObjectManager::find(const std::string& objectName)
{
	return _findMap[objectName];
}

bool ObjectManager::name_contains(std::shared_ptr<GameObject> object, const std::string& name)
{
	return (object->objectName().contains(name));
}