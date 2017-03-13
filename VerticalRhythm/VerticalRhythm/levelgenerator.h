#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H
#include <string>
#include <vector>
#include <list>
#include <queue>

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

//	beat = regular *= 5, swing *= 8
//	5 or 8
//	density: low = roundup(medium/2), high = medium * 2
//	nrOfbeats *= density
//	2.5,	5,	10	or	4,	8,	16
//		std::ceil - 2.5 -> 3
//	nrOfbeats *= length/20
//	array[nrOfbeats]
//	if regular fill array w/ length/nrOfbeats
//	else length/nrOfbeats * 2, alternate * 4/5 and * 1/5

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
	int length = 20; //{0,5,10,15,20}
	Density density = Density::medium;
	RhythmType type = RhythmType::regular;
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
