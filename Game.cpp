#include "Game.h"

//
//private Functions
//

void Game::initVariables()
{
	//init videoMode
	this->videoMode.height = 800;
	this->videoMode.width = 1000;

	//init window ptr
	this->renderWindow = nullptr;

	//init board position
	this->boardPosition.x = static_cast<float>(videoMode.width / 2 - 50);
	this->boardPosition.y = static_cast<float>(videoMode.height - 80);

	//init board 
	this->reflectiveBoard.setSize(Vector2f(200.f, 10.f));
	this->reflectiveBoard.setFillColor(Color::White);
	this->reflectiveBoard.setPosition(boardPosition);

	//init ball position
	this->ballPosition.x = static_cast<float>(videoMode.width / 2 - 5);
	this->ballPosition.y = static_cast<float>(videoMode.height - 105);

	//init ball
	this->ball.setRadius(3);
	this->reflectiveBoard.setFillColor(Color::White);
	this->reflectiveBoard.setPosition(ballPosition);
	this->ball.setPosition(this->ballPosition);

}

void Game::initLevel()
{
	VertexArray quad(sf::Quads, 4);

	const int boxSideSize = 16;

	for (unsigned int column = 5; column < (this->renderWindow->getSize().x - 5); column += boxSideSize + 6)
	{
		for (unsigned int row = 5; row < (this->renderWindow->getSize().y - (this->renderWindow->getSize().y / 2)); row += boxSideSize + 6)
		{
			if (static_cast<bool>(rand() % 2))
			{
				quad[0].position = Vector2f(static_cast<float>(column), static_cast<float>(row));
				quad[1].position = Vector2f(static_cast<float>(column + boxSideSize), static_cast<float>(row));
				quad[2].position = Vector2f(static_cast<float>(column + boxSideSize), static_cast<float>(row + boxSideSize));
				quad[3].position = Vector2f(static_cast<float>(column), static_cast<float>(row + boxSideSize));

				this->boxes.push_back(quad);
			}
		}
	}
}

void Game::drawBoxex()
{
	for (auto iter = this->boxes.begin(); iter != this->boxes.end(); iter++)
	{
		this->renderWindow->draw(*iter);
	}
}

void Game::initWindow()
{
	this->renderWindow = new RenderWindow(this->videoMode, "Ball", sf::Style::Titlebar | sf::Style::Close);

	this->renderWindow->setFramerateLimit(144);
}

bool Game::boardIsHitted()
{
	if (this->ball.getPosition().y > (this->reflectiveBoard.getPosition().y + this->reflectiveBoard.getSize().y)) return false;
	else return ((this->ball.getPosition().y + 2 * this->ball.getRadius()) >= this->boardPosition.y
		&& this->ball.getPosition().x >= this->boardPosition.x
		&& this->ball.getPosition().x <= (this->boardPosition.x + this->reflectiveBoard.getSize().x));
}

bool Game::verticalEdgeIsHitted(Vector2f higherPoint, Vector2f lowerPoint, bool fromLeftSide)
{
	if (higherPoint.x != lowerPoint.x) return false;
	else if (this->ball.getPosition().y < higherPoint.y || this->ball.getPosition().y > lowerPoint.y) return false;
	else
	{
		if (fromLeftSide) return ((this->ball.getPosition().x + 2 * this->ball.getRadius()) >= higherPoint.x);
		else return (this->ball.getPosition().x <= higherPoint.x);
	}
}

bool Game::horizontalEdgeIsHitted(Vector2f leftPoint, Vector2f rightPoint, bool fromAbove)
{
	if (leftPoint.y != rightPoint.y) return false;
	else if (this->ball.getPosition().x < leftPoint.x || this->ball.getPosition().x > rightPoint.x) return false;
	else
	{
		if (fromAbove) return (this->ball.getPosition().y + 2 * this->ball.getRadius() >= leftPoint.y);
		else return (this->ball.getPosition().y <= leftPoint.y);
	}
}

void Game::validatePosition()
{
	//get ball position
	this->ballPosition = this->ball.getPosition();

	//validate ball position
	if (this->ballPosition.y > this->renderWindow->getSize().y)
	{
		//game over statement
		this->renderWindow->close();

		cout << "Game over...\n";
	}
	//borders reflection statements
	else if (this->verticalEdgeIsHitted(Vector2f(0, 0), Vector2f(0, static_cast<float>(this->renderWindow->getSize().y)), false) ||
		this->verticalEdgeIsHitted(Vector2f(static_cast<float>(this->renderWindow->getSize().x), 0), Vector2f(static_cast<float>(this->renderWindow->getSize().x), static_cast<float>(this->renderWindow->getSize().y)), true))
	{
		this->offsetX = -this->offsetX;
	}
	else if (this->horizontalEdgeIsHitted(Vector2f(0, 0), Vector2f(static_cast<float>(this->renderWindow->getSize().x), 0), false) || this->boardIsHitted())
	{
		this->offsetY = -this->offsetY;
	}
	//boxex reflection statements
	else
	{
		double distance = 10;

		Vector2f ballCenter = Vector2f(static_cast<float>(this->ball.getPosition().x + this->ball.getRadius()), static_cast<float>(this->ball.getPosition().y + this->ball.getRadius()));
		FloatRect rect;

		for (int iter = 0; iter < this->boxes.size(); iter++)
		{
			rect = this->boxes[iter].getBounds();

			if (sqrt(pow(ballCenter.x - rect.left, 2) + pow(ballCenter.y - rect.top, 2)) < distance ||
				sqrt(pow(ballCenter.x - (rect.left + rect.width), 2) + pow(ballCenter.y - rect.top, 2)) < distance ||
				sqrt(pow(ballCenter.x - rect.left, 2) + pow(ballCenter.y - (rect.top + rect.height), 2)) < distance ||
				sqrt(pow(ballCenter.x - (rect.left + rect.width), 2) + pow(ballCenter.y - (rect.top + rect.height), 2)) < distance)
			{
				if (this->verticalEdgeIsHitted(Vector2f(rect.left, rect.top), Vector2f(rect.left, rect.top + rect.height), false) ||
					this->verticalEdgeIsHitted(Vector2f(rect.left + rect.width, rect.top), Vector2f(rect.left + rect.width, rect.top + rect.height), true))
				{
					this->offsetX = -this->offsetX;

					this->boxes.erase(this->boxes.begin() + iter, this->boxes.begin() + iter + 1);
				}
				else if (this->horizontalEdgeIsHitted(Vector2f(rect.left, rect.top), Vector2f(rect.left + rect.width, rect.top), false) ||
					this->horizontalEdgeIsHitted(Vector2f(rect.left, rect.top + rect.height), Vector2f(rect.left + rect.width, rect.top + rect.height), true))
				{
					this->offsetY = -this->offsetY;

					this->boxes.erase(this->boxes.begin() + iter, this->boxes.begin() + iter + 1);
				}
			}
		}
	}

	//move ball by offsets
	this->ball.move(this->offsetX, this->offsetY);

	if (pseudoLogTimer == 0) cout << "Current ball pos--> x: " << this->ball.getPosition().x << " y: " << this->ball.getPosition().y << endl;
}

const bool Game::isRunning() const
{
	return this->renderWindow->isOpen();
}

//
//public Functions
//

void Game::eventsHandler()
{
	while (this->renderWindow->pollEvent(this->event))
	{
		switch (this->event.type)
		{
			//clode event
		case sf::Event::Closed:
		{
			cout << "Closing window...\n";

			this->renderWindow->close();

			break;
		}

		//keyboard events case
		case sf::Event::KeyPressed:
		{
			if (pseudoLogTimer == 0) cout << "Key " << this->event.key.code << " pressed\n";

			switch (this->event.key.code)
			{
			case 0x47:
			{
				//key <--(left arrow)

				//move to left
				if (this->boardPosition.x > 0)
					this->boardPosition.x = this->boardPosition.x - boardSpeed;

				break;
			}

			case 0x48:
			{
				//key -->(right arrow)

				//move to right 
				if ((this->boardPosition.x + this->reflectiveBoard.getSize().x) < this->renderWindow->getSize().x)
					this->boardPosition.x = this->boardPosition.x + boardSpeed;

				break;
			}

			default: break;
			}

			cout << "boardPosition.x: " << this->boardPosition.x << endl;

			break;
		}

		default: break;
		}
	}
}

void Game::update()
{
	this->eventsHandler();
}

void Game::render()
{
	//clear window to redraw
	this->renderWindow->clear(Color::Black);

	//draw boxex
	this->drawBoxex();

	//renew ball coordinates
	this->validatePosition();
	this->renderWindow->draw(this->ball);

	//renew board coordinates
	this->reflectiveBoard.setPosition(this->boardPosition);
	this->renderWindow->draw(this->reflectiveBoard);

	//display
	this->renderWindow->display();

	pseudoLogTimer++;
	pseudoLogTimer = pseudoLogTimer % 15;

	if (this->boxes.size() == 0)
	{
		this->renderWindow->close();

		cout << "Game over..\n";
	}
}

//
//Constructor & Destructor
//

Game::Game()
{
	initVariables();
	initWindow();
	initLevel();
}

Game::~Game()
{
	delete this->renderWindow;
}