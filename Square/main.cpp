#include <SDL/SDL.h>
#include "Game.h"

int main(int argc, char** argv)
{
	Game game;
	bool success = game.init();

	if (success)
	{
		game.run();
	}
	game.quit();
	return 0;
}
