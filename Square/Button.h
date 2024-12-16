#pragma once

#include "GameObject.h"
#include "EventHandler.h"
#include <memory>
struct Event;
class Button : virtual public GameObject
{
public:
	Button();
	Button(const std::string& str);
	virtual ~Button() override;
	// wrapping Function

	void BindEvent(Event& e);
	void ClickEvent(std::unique_ptr<EventHandler>&);

	const bool isEnabled() const { return _enable; }
	void enable() { _enable = true; }
	void disable() { _enable = false; }
protected:
	std::unique_ptr<Event> _bind;
	bool _enable{ true };
};
