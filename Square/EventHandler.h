#pragma once
#include <queue>
#include <memory>

enum class EID
{
	START,
	SPLASH = 0b1,
	TITLE = 0b10,
	TITLE_ENTER = 0b11,
	NEUTRAL = 0b100,
	END = 65536
};

struct Event
{
	EID eid = EID::START;
};
class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void PushEvent(std::shared_ptr<Event> event);
	std::shared_ptr<Event> PopEvent();
private:
	std::queue<std::shared_ptr<Event>> _events;
};

