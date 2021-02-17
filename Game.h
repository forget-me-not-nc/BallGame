#pragma once

#define PI	3.14159265f

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <Windows.h>

using sf::RenderWindow;
using sf::VideoMode;
using sf::Event;
using sf::Color;
using sf::RectangleShape;
using sf::CircleShape;
using sf::Vector2f;
using sf::Vertex;
using sf::VertexArray;
using sf::PrimitiveType;
using sf::FloatRect;
using sf::Texture;

using std::cout;
using std::endl;
using std::cin;
using std::vector;

/*
	Class that interpreters game and its engine 
*/

static int pseudoLogTimer = 0;

class Game
{
private:

	//private Variables
	RenderWindow* renderWindow; //window ptr
	
	RectangleShape reflectiveBoard; //game board to reflect ball
	Vector2f boardPosition; //board pos

	CircleShape ball; //ball
	float offsetX = -1.2f; //ball X offset
	float offsetY = -1.2f; //ball Y offset
	Vector2f ballPosition; //ball pos

	VideoMode videoMode; //video mode
	Event event; //event var

	const float boardSpeed = 20.f;

	vector <VertexArray> boxes; //vector box elem
	Texture gangster;

	//private Functions
	bool boardIsHitted();
	bool verticalEdgeIsHitted(Vector2f higherPoint, Vector2f lowerPoint, bool fromLeftSide);
	bool horizontalEdgeIsHitted(Vector2f leftPoint, Vector2f rightPoint, bool fromAbove);
	void initVariables();
	void initLevel(); //game level design
	void drawBoxex(); //drawBoxex
	void initWindow();
	void validatePosition();

public:

	//Constructor & Destructor
	Game();
	virtual ~Game();

	//public Functions
	const bool isRunning() const;
	void eventsHandler();
	void update();
	void render();
};

