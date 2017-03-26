#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Shape.hpp>

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
/*
enum MovementState
{
	moving,
	waiting
};


enum Interval
{
	falling,
	braking
};

struct state
{
	int length;
	MovementState move;
};

struct brake
{
	int length;
	Interval interval;
};*/

enum Verb
{
	move,
	brake
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

enum Direction
{
	left,
	right
};

struct geometry
{
	keyword type;
	sf::Vector2f position;
	sf::Vector2f size = sf::Vector2f();
};


struct action
{
	Verb word;
	Direction direction;
	double starttime;
	double stoptime;
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

struct rhythmgroup
{
	//std::list<state> states;
	//std::queue<brake> brakes;
};

struct player
{
	sf::Vector2f speed = sf::Vector2f(30, 30);
	sf::Vector2f size = sf::Vector2f(5, 5);
	double brakeTime = 5.0;
	double gravity = 9.82;
	sf::Vector2f position = sf::Vector2f();
};
class LevelGenerator
{
public:
	static LevelGenerator& instance();
	LevelGenerator();
	~LevelGenerator();
	rhythm GetRhythm();
	rhythmgroup GetRhythmGroup();
	std::vector<double> GetBeats(rhythm r);
	std::vector<action> createActions(std::vector<double> beats);
	std::vector<action> createActions(rhythm r);
	std::vector<geometry> GetGeometry(std::vector<action> actions);
	std::vector<geometry> createGeometry(rhythm r);
	std::vector<sf::Shape*> GetShapes(std::vector<geometry> geometryElements, sf::Vector2f offset = sf::Vector2f());
	player avatar;
	
	//Think this through how to do it
	//std::map<std::vector<std::vector<keyword>>, Verb> keywords;
};

#endif
