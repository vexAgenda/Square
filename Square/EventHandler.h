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

	EDITOR = 0b10000,
	EDITOR_MODE_TRANSIT = 0b10001,
	EDITOR_BUTTON_WALL = 0b11000,
	EDITOR_BUTTON_HOSTILE = 0b11001,
	EDITOR_BUTTON_SP = 0b11010,
	EDITOR_BUTTON_EP = 0b11011,
	EDITOR_BUTTON_PATH = 0b11100,
	EDITOR_BUTTON_FORWARD = 0b11101,
	EDITOR_BUTTON_BACK = 0b11110,

	GO_TO_TITLE = 0b100000,
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
