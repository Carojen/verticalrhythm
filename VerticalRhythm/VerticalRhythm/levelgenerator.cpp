#include "levelgenerator.h"
#include "ObjectManager.h"
#include <iostream>


LevelGenerator::LevelGenerator()
{
	srand(time(NULL));
}

LevelGenerator::~LevelGenerator()
{
}

LevelGenerator& LevelGenerator::instance()
{
	static LevelGenerator lg;
	return lg;
}

rhythm LevelGenerator::GetRhythm()
{
	return rhythm();
}

rhythmgroup LevelGenerator::GetRhythmGroup()
{
	return rhythmgroup();
}

std::vector<double> LevelGenerator::GetBeats(rhythm r)
{
	int nrOfBeats = r.type == RhythmType::regular ? 5 : 8;
	switch (r.density)
	{
	case low:
		nrOfBeats = std::round((double)nrOfBeats / 2.0);
		break;
	case medium:
		break;
	case high:
		nrOfBeats *= 2;
	default:
		break;
	}

	nrOfBeats *= (double)r.length / 20.0;
	if (nrOfBeats == 0)
	{
		nrOfBeats = 1;
	}
	//std::cout << nrOfBeats << ": ";
	
	std::vector<double> beats;
	for (int i = 0; i < nrOfBeats; i++)
	{
		if (r.type == RhythmType::regular)
		{
			beats.push_back(r.length / nrOfBeats);
		}
		else
		{
			if (i % 2 == 0)
			{
				beats.push_back(r.length / nrOfBeats * 4 / 2.0);
			}
			else
			{
				beats.push_back(r.length / nrOfBeats * 1 / 2.0);
			}
		}
	}

	if (r.type == RhythmType::swing && nrOfBeats % 2 != 0)
	{
		beats.push_back(r.length / nrOfBeats * 1 / 2.0);
	}

	float sum = 0;
	for (auto& n : beats)
	{
		sum += n;
	}
	float sum2 = 0;
	for (auto& beat : beats)
	{
		beat *= r.length / sum;
		sum2 += beat;
	}
	//std::cout << "(" << r.length << ")" << sum2 << " - ";	

	return beats;
}

void LevelGenerator::GetActions(std::vector<double> beats)
{
	
	std::vector<action> actions;
	double sum = 0;
	for (auto n : beats)
	{
		sum += n;
	}
	int randomValue = std::rand() % 2;

	std::cout << "[ ";
	double currentTime = 0;
	double endMove = 0;
	

	for (auto beat : beats)
	{
		action a = action();
		a.word = (verb)randomValue;
		a.starttime = currentTime;

		double length = 0;
		if (a.word == verb::move)
		{
			length = (rand() % (int)((sum - currentTime) * 4) + 1) * 0.25;
		}
		else
		{
			length = (rand() % (int)(beat * 4) + 1) * 0.25;
		}
		
		a.stoptime = currentTime + length;
		actions.push_back(a);

		std::cout << beat << " ";
		randomValue = std::rand() % 2;
		currentTime += beat;
	}
	std::cout << "]" << std::endl;
	for (auto a : actions)
	{
		switch (a.word)
		{
		case move:
			std::cout << "move: " << a.starttime << " " << a.stoptime 
				<< " (" << a.stoptime - a.starttime << ")";
			break;
		case brake:
			std::cout << "brake: " << a.starttime << " " << a.stoptime
				<< " (" << a.stoptime - a.starttime << ")";
			break;
		default:
			break;
		}
		std::cout << std::endl;
	}
}