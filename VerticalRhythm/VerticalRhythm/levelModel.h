#ifndef LEVELMODEL_H
#define LEVELMODEL_H
#include <SFML\System\Vector2.hpp>


//RHYTHMS: Density, RhythmType, Length
enum Density
{
	low,	// = 0,	3,	4
	medium,	// = 1,	5,	8
	high	// = 2,	10,	16
};

enum RhythmType
{
	regular,
	swing,
	random
};

//ACTIONS: Verb, Direction, Starttime, Stoptime
enum Verb
{
	move,
	brake,
	wait
};

enum Direction
{
	left,
	right
};

enum keyword
{
	move_left_ledge,
	move_right_ledge,
	moving_block_single,
	platform,
	move_left_hinder,
	move_right_hinder,
	other
};

//A rhythm is the pre-stage to a level-segment
struct rhythm
{
	int length = 20; //{5,10,15,20}
	Density density = Density::medium;
	RhythmType type = RhythmType::regular;
	bool isReflectable = false;
	bool isRepeatable = false;
};

struct action
{
	Verb word;
	Direction direction;
	double starttime;
	double stoptime;
};

struct geometry
{
	keyword type;
	sf::Vector2f position;
	sf::Vector2f size = sf::Vector2f();
};

struct rhythmgroup
{
	//std::list<state> states;
	//std::queue<brake> brakes;
};

struct player
{
	sf::Vector2f speed = sf::Vector2f(30, 30);
	sf::Vector2f scale = sf::Vector2f(5, 5);
	sf::Vector2f position = sf::Vector2f();
};



#endif
