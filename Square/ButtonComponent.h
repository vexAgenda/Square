#pragma once

#include "Component.h"

struct Event;
class ButtonComponent : public Component
{
public:
	ButtonComponent(std::shared_ptr<GameObject> const&, int);
	virtual ~ButtonComponent() override;
	// wrapping Function

	void BindEvent(Event& e);
	void ClickEvent(std::unique_ptr<class EventHandler>&);

	const bool isEnabled() const { return _enable; }
	void enable() { _enable = true; }
	void disable() { _enable = false; }
protected:
	std::unique_ptr<Event> _bind;
	bool _enable{ true };
};

