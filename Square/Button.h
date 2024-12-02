#pragma once

#include "GameObject.h"
#include "EventHandler.h"
#include <memory>
struct Event;
class Button : public GameObject
{
public:
	Button();
	Button(const std::string& str);
	virtual ~Button() override;
	// wrapping Function

	void BindEvent(Event* e);
	void ClickEvent(std::unique_ptr<EventHandler>&);
private:
	Event* _bind{ nullptr };
};

