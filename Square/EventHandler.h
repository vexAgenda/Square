#pragma once
#include <queue>
#include <memory>

enum class EID
{
	START,
	SPLASH = 0b1,
	NEUTRAL = 0b10,
	TITLE = 0b100,
	TITLE_ENTER = 0b101,
	TITLE_END = 0b110,
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

