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

	return beats;
}

std::vector<action> LevelGenerator::GetActions(std::vector<double> beats)
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
	int latestMoveIndex = -1;
	bool simultanousAction = false;

	for (int i = 0; i < beats.size(); i++)
	{		
		double beat = beats[i];
		currentTime += beat; // starttiden �r efter det f�rsta beatet/v�nteperioden. Se anteckning 20/3.
		action a = action();
		a.word = (Verb)randomValue;
		a.starttime = currentTime; 

		double length = 0;
		if (a.word == Verb::move)
		{
			//l�ngden p� det sista beatet g�r inte att veta i f�rv�g - det best�ms av n�r viloplattformen �r
			length = (rand() % (int)((sum + 1 - currentTime) * 4) + 1) * 0.25;
			if (latestMoveIndex != -1)
			{
				action& b = actions.at(latestMoveIndex);
				if (b.stoptime >= currentTime)
				{
					b.stoptime = currentTime;
					a.direction = (Direction)((b.direction + 1) % 2);
				}				
			}
			else
			{
				a.direction = (Direction)(rand() % 2);
			}
			latestMoveIndex = actions.size();
		}
		else //l�ngden p� inbromsningen
		{
			length = (rand() % (int)(beat * 4) + 1) * 0.25;
		}
		
		a.stoptime = currentTime + length;
		actions.push_back(a);

		std::cout << beat << " ";
		if (std::rand() % 3 == 0 && simultanousAction == false) // one in three chance of having simultanous actions
		{
			//reset starting point to latest used point and set action to the opposite
			currentTime -= beat;
			randomValue = (randomValue + 1) % 2;
			simultanousAction = true;
			std::cout << "|";
			i--;
		}
		else
		{
			simultanousAction = false;
			randomValue = std::rand() % 2;
		}			
	}
	std::cout << "]" << std::endl;
	for (auto a : actions)
	{
		switch (a.word)
		{
		case move:
			std::cout << (a.direction == Direction::left ? "left " : "right ")  << a.starttime << " " << a.stoptime 
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
	return actions;
}

void LevelGenerator::GetGeometry(std::vector<action> actions)
{
	//Create a list with left | right | brake as key words
	//Create possible geometry for those actions
	//Add these possibilities as entries under the keywords
	//
	//Create geometry according to type	
}

