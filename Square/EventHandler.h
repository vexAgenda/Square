#pragma once
#include <queue>
#include <memory>

enum class EID
{
	EID_START,
	EID_SPLASH = 0b1,
	EID_TITLE = 0b10,
	EID_END = 0b1000000
};
class EventHandler
{
public:
	EventHandler();
	~EventHandler();
private:
	
};

