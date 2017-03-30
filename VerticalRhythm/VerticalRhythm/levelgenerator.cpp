#include "levelgenerator.h"
#include "ObjectManager.h"
#include <iostream>
#include "gameobject.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics\CircleShape.hpp>



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
				beats.push_back(r.length / (nrOfBeats * 3) / 2.0);
			}
			else
			{
				beats.push_back(r.length / (nrOfBeats * 1) / 2.0);
			}
		}
	}

	if (r.type == RhythmType::swing && nrOfBeats % 2 != 0)
	{
		beats.push_back(r.length / (nrOfBeats * 1) / 2.0);
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
		if (beat == 0)
		{
			beat = 0.25;
		}
		sum2 += beat;
	}	

	return beats;
}

std::vector<action> LevelGenerator::createActions(std::vector<double> beats, float offset)
{	
	std::vector<action> actions;
	//Kolla hur lång rytmen är
	double rhythmLength = 0;
	for (auto n : beats)
	{
		rhythmLength += n;
	}
	int randomValue = std::rand() % 2;
	double currentTime = offset;

	for (int i = 0; i < beats.size(); i++)
	{		
		double beat = beats[i];
		
		currentTime += beat; //starttiden är efter det första beatet/vänteperioden. Se anteckning 20/3.
		action a = action();
		a.word = (Verb)randomValue;
		a.starttime = currentTime; 
		double length = 0;

		if (a.word == Verb::move)
		{						
			//Se till att förflyttningar inte går kant-i-kant
			if (!actions.empty() && actions.back().word == move && actions.back().stoptime == currentTime)
			{
				//Nästa ges motsatt riktning
				//Ett alternativ hade varit att alternera med inbromsningar				
				a.direction = (Direction)((actions.back().direction + 1) % 2);							
			}			
			else
			{
				a.direction = (Direction)(rand() % 2);
			}			
		}
		
		//Längden på en handling begränsas av längden på taktslaget
		//Avataren ska hinna falla förbi hindret eller platformen, så det måste finnas utrymme efter.
		if (i < beats.size() - 1)
		{
			if (beats[i + 1] * 3 >= 1 && a.word == brake)
			{
				length = (rand() % (int)(beats[i + 1] * 3)) * 0.25;
			}
			else if (a.word == move)
			{
				length = (rand() % (int)(beats[i + 1] * 4)) * 0.25;
			}
			else
			{
				length = 0.25;
			}			
		}
		else //Det sista beatet får en fast längd
		{
			a.word = wait;
			length = 0.5;
			a.direction = (Direction)(rand() % 2);
		}
		
		a.stoptime = currentTime + length;
		actions.push_back(a);

		randomValue = rand() % 2;				
	}	
	
	return actions;
}

std::vector<action> LevelGenerator::createActions(rhythm r, float offset)
{
	return createActions(GetBeats(r), offset);
}

std::vector<geometry> LevelGenerator::createGeometry(rhythm r)
{
	return GetGeometry(createActions(GetBeats(r)));
}

std::vector<geometry> LevelGenerator::GetGeometry(std::vector<action> actions)
{
	std::vector<geometry> geometryElements;
	double moveTime = 0;
	double brakeTime = 0;
	double currentTime = 0;

	for (action a : actions)
	{
		currentTime = a.starttime;
		avatar.position.x = moveTime * avatar.speed.x;
		avatar.position.y = (currentTime - brakeTime) * avatar.speed.y;

		geometry g;
		if (a.word == move)
		{
			moveTime += (a.stoptime - a.starttime) * (a.direction == left ? -1 : 1);
			g.type = a.direction == left ? move_left_ledge : move_right_ledge;
			g.position = avatar.position;
			g.size.x = (a.stoptime - a.starttime) * avatar.speed.x;
			g.size.y = (a.stoptime - a.starttime) * avatar.speed.y;

			if (a.stoptime - a.starttime < 0.75)
			{
				g.type = g.type == move_left_ledge ? move_left_hinder : move_right_hinder;
			}
		}
		else if (a.word == brake)
		{
			brakeTime += a.stoptime - a.starttime;
			g.type = moving_block_single;
			g.position = avatar.position;
			g.size.x = -1;
			g.size.y = a.stoptime - a.starttime;
		}
		else if(a.word == wait)
		{
			moveTime += (a.stoptime - a.starttime) * (a.direction == left ? -1 : 1);
			g.type = a.direction == left ? platform_right : platform_left;
			g.position = avatar.position;
			g.size.x = (a.stoptime - a.starttime) * avatar.speed.x;
			g.size.y = (a.stoptime - a.starttime) * avatar.speed.y;
		}
		geometryElements.push_back(g);
	}
	return geometryElements;
}

std::vector<GameObject*> LevelGenerator::GetLevelObjects(std::vector<action> actions, sf::Vector2f offset)
{
	return GetLevelObjects(GetGeometry(actions), offset);
}

std::vector<GameObject*> LevelGenerator::GetLevelObjects(std::vector<geometry> geometryElements, sf::Vector2f offset)
{
	std::vector<GameObject*> gameObjects;

	for (auto e : geometryElements)
	{
		gameObjects.push_back(new GameObject(e.type, e.position + offset, e.size, std::max(avatar.scale.x, avatar.scale.y)));
	}
	return gameObjects;
}
