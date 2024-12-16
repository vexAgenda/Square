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
	TITLE_STAGE_SELECT_CALLED = 0b111,
	LOAD = 0b1000,
	LOAD_COMPLETE = 0b1001,
	GAME = 0b1010,
	GAME_END = 0b1011,
	END = 65536
};

struct Event
{
	EID eid = EID::START;
	std::string info{};
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

