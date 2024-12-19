#pragma once
#include <vector>
#include <any>
#include <memory>
class Component
{
public:
	Component(std::shared_ptr<class GameObject>const&,int);
	virtual ~Component();

	virtual bool init(const std::vector<std::any>&);
	virtual void update(float deltaTime);
protected:
	std::shared_ptr<GameObject> _owner;
	int _order;

};

