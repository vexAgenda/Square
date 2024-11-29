#pragma once

#include "GameObject.h"
#include <memory>
struct Event;
class Button : public GameObject
{
public:
	Button();
	Button(const std::string& str);
	virtual ~Button() override;
	// wrapping Function
	bool is_hover(const Vector2& mouse);

	void BindEvent(Event& e);
private:
	Event* _bind{ nullptr };
};

