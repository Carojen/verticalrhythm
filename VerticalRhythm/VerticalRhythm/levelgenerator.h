#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H
#include <string>
#include <vector>
#include <list>
#include <queue>

enum Density
{
	low,
	medium,
	high
};

enum MovementState
{
	moving,
	waiting
};

enum Interval
{
	falling,
	breaking
};

struct state
{
	int length;
	MovementState move;
};

struct stop
{
	int length;
	Interval interval;
};

struct rhythm
{
	int length = 10; //{0,5,10,15,20}
	Density density = Density::low;
	std::vector<int> beatPattern = { 0,0,0,0,1 }; //regular {0,0,0,0,1}, swing {0,0,0,1,1}
	bool isReflectable = false;
	bool isRepeatable = false;
};

struct rhytmgroup
{
	std::list<state> states;
	std::queue<stop> stops;
};

class LevelGenerator
{
	LevelGenerator();
	~LevelGenerator();
	rhythm GetRhythm();
	rhytmgroup GetRhythmGroup();
	
};

#endif
