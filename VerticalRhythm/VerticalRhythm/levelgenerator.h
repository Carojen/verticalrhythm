#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <SFML\System\Vector2.hpp>

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
	side_passage
};

enum Direction
{
	left,
	right
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
	sf::Vector2f speed = sf::Vector2f(10, 10);
	sf::Vector2f size = sf::Vector2f(64, 64);
	double brakeTime = 5.0;
	double gravity = 9.82;
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
	std::vector<action> GetActions(std::vector<double> beats);
	void GetGeometry(std::vector<action> actions);
	player avatar;
	
	//Think this through how to do it
	//std::map<std::vector<std::vector<keyword>>, Verb> keywords;
};

#endif
