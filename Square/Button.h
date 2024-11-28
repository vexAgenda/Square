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
	void  is_clicked(std::unique_ptr<class EventHandler>,
		const Vector2& mouse);

	void BindEvent(Event& e);
private:
	Event* _bind{ nullptr };
};

