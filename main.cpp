#include "Game.h"

#include <ctime>

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));

	cout << "--------------Log--------------\n";

	//init Game class
	Game game;

	//game loop
	while (game.isRunning())
	{
		game.update();

		game.render();
	}

	//end program
	return 0;
}