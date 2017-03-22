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
		currentTime += beat; // starttiden är efter det första beatet/vänteperioden. Se anteckning 20/3.
		action a = action();
		a.word = (Verb)randomValue;
		a.starttime = currentTime; 

		double length = 0;
		if (a.word == Verb::move)
		{
			//längden på det sista beatet går inte att veta i förväg - det bestäms av när viloplattformen är
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
		else //längden på inbromsningen
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
	GetGeometry(actions);
	return actions;
}

std::vector<geometry> LevelGenerator::GetGeometry(std::vector<action> actions)
{
	std::vector<action> moves;
	std::vector<action> brakes;
	double moveTime = 0;
	double brakeTime = 0;
	std::vector<geometry> geometryElements;
	geometry g;
	for (action a : actions)
	{
		avatar.position = sf::Vector2f(moveTime * avatar.speed.x, (a.starttime - brakeTime) * avatar.speed.y);
		g.position = avatar.position;

		std::cout << avatar.position.x << ", " << avatar.position.y << std::endl;
		//Sortera handlingarna
		if (a.word == brake)
		{
			brakes.push_back(a);
			//Kolla samtida brake och move
			if (!moves.empty() && !geometryElements.empty() && moves.back().starttime == a.starttime)
			{
				//modifiera senaste handlingen till att bli en mer tvär sväng åt samma riktning
				geometryElements.back().type = keyword::other;
			}
			else
			{
				g.type = keyword::moving_block_single;
				geometryElements.push_back(g);
			}
			brakeTime += a.stoptime - a.starttime;
		}
		else
		{
			moves.push_back(a);
			//Kolla samtida brake och move
			if (!brakes.empty() && !geometryElements.empty() && brakes.back().starttime == a.starttime)
			{
				//modifiera senaste handlingen till att bli en mer tvär sväng åt samma riktning
				geometryElements.back().type = keyword::other;
			}
			//Kolla om move har en brake direkt före alternativt överlappar något
			else if(!brakes.empty() && !geometryElements.empty() && brakes.back().stoptime >= a.starttime)
			{
				//modifiera senaste handlingen från att vara moving_block till att vara side_passage
				geometryElements.back().type = keyword::side_passage;
			}
			else
			{
				g.type = a.direction == left ? keyword::move_left_ledge : keyword::move_right_ledge;
				geometryElements.push_back(g);
			}
			moveTime += (a.stoptime - a.starttime) * a.direction == left ? -1 : 1;			
		}		
	}
	//Create a list with left | right | brake as key words
	//Create possible geometry for those actions
	//Add these possibilities as entries under the keywords
	//
	//Create geometry according to type	
	for (auto e : geometryElements)
	{
		std::cout << "Position: " << e.position.x << ", " << e.position.y << ". Type: ";
		switch (e.type)
		{
		case move_left_ledge:
			std::cout << "move_left_ledge";
			break;
		case move_right_ledge:
			std::cout << "move_right_ledge";
			break;
		case moving_block_single:
			std::cout << "moving_block_single";
			break;
		case side_passage:
			std::cout << "side_passage";
			break;
		case other:
			std::cout << "other";
			break;
		default:
			break;
		}
		std::cout << std::endl;
	}
	return geometryElements;
}

