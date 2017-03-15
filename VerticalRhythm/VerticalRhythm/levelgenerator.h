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

enum verb
{
	move,
	brake
};

struct action
{
	verb word;
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

class LevelGenerator
{
public:
	static LevelGenerator& instance();
	LevelGenerator();
	~LevelGenerator();
	rhythm GetRhythm();
	rhythmgroup GetRhythmGroup();
	std::vector<double> GetBeats(rhythm r);
	void GetActions(std::vector<double> beats);
	
};

#endif
