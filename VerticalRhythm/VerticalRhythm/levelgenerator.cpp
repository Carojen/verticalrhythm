#include "levelgenerator.h"
#include "ObjectManager.h"
#include <iostream>
#include "gameobject.h"



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
		currentTime += beat; // starttiden är efter det första beatet/vänteperioden. Se anteckning 20/3.
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
		//Avataren ska hinna falla förbi hindret, så det måste finnas utrymme efter
		if (i < beats.size() - 1)
		{
			length = rand() % (int)(beats[i + 1] * 3) * 0.25;
		}
		else //Det sista beatet får samma längd som det första
		{
			length = beats[0];
		}
		
		a.stoptime = currentTime + length;
		actions.push_back(a);

		randomValue = rand() % 2;				
	}

	action last;
	last.word = wait;
	last.starttime = currentTime + beats[0];
	last.stoptime = last.starttime + beats[0];
	actions.push_back(last);
	
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
			g.size.x = 200;
			g.size.y = 1;
		}
		else if(a.word == wait)
		{
			g.type = platform;
			g.position = avatar.position;
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

//Ska enbart ha hand om utritningen av bansegmenten och inte påverka resultatet
//scale, platformThickness och passWidth bör därmed sättas tidigare
std::vector<sf::Shape*> LevelGenerator::GetShapes(std::vector<geometry> geometryElements, sf::Vector2f offset)
{
	std::vector<sf::Shape*> shapes;
	std::vector<sf::Vector2f> rightSide;
	std::vector<sf::Vector2f> leftSide;

	float min_point = offset.x;
	float max_point = offset.x;

	int scale = avatar.scale.y;	
	int passWidth = 4 * scale;
	float platformThickness = 1;
	sf::Vector2f playerOffset = sf::Vector2f(scale, scale *2);

	/*
	sf::CircleShape* zero = new sf::CircleShape();
	zero->setOutlineThickness(-2);
	zero->setOutlineColor(sf::Color::Cyan);
	zero->setFillColor(sf::Color::Transparent);
	zero->setRadius(scale);
	zero->setPointCount(3);
	zero->setOrigin(zero->getLocalBounds().width / 2, zero->getLocalBounds().height / 2);
	zero->setPosition(sf::Vector2f() + offset);
	shapes.push_back(zero);	
	rightSide.push_back(zero->getPosition() + sf::Vector2f(passWidth, 0));	
	leftSide.push_back(zero->getPosition() - sf::Vector2f(passWidth, 0));	*/
	
	for (auto e : geometryElements)
	{		
		sf::ConvexShape* r = new sf::ConvexShape();		

		sf::CircleShape* c = new sf::CircleShape();
		c->setOutlineThickness(-2);
		c->setOutlineColor(sf::Color::White);
		c->setFillColor(sf::Color::Transparent);
		c->setRadius(scale/2);
		c->setOrigin(c->getLocalBounds().width / 2, c->getLocalBounds().height / 2);				
		c->setPosition(e.position + offset);			
		shapes.push_back(c);

		switch (e.type)		
		{
			case move_left_ledge:				
				r->setOutlineThickness(-2);
				r->setOutlineColor(sf::Color::Green);
				r->setFillColor(sf::Color::Green);
				r->setPointCount(4);
					r->setPoint(0, sf::Vector2f(0, 0));
					r->setPoint(1, sf::Vector2f(-e.size.x, e.size.y));
					r->setPoint(2, sf::Vector2f(-e.size.x + platformThickness, e.size.y + platformThickness));
					r->setPoint(3, sf::Vector2f(platformThickness, platformThickness));
				r->setPosition(e.position + offset + playerOffset);
				shapes.push_back(r);

				rightSide.push_back(r->getPosition() + r->getPoint(0) + sf::Vector2f(platformThickness*2, 0));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(platformThickness*2, 0));
				leftSide.push_back(r->getPosition() + r->getPoint(1) - sf::Vector2f(passWidth, e.size.y < scale ? e.size.y/2 : passWidth));
				leftSide.push_back(r->getPosition() + r->getPoint(1) - sf::Vector2f(passWidth, 0));
				break;
			case move_right_ledge:	
				r->setOutlineThickness(-2);
				r->setOutlineColor(sf::Color::Green);
				r->setFillColor(sf::Color::Green);
				r->setPointCount(4);
					r->setPoint(0, sf::Vector2f(0, 0));
					r->setPoint(1, sf::Vector2f(e.size.x, e.size.y));
					r->setPoint(2, sf::Vector2f(e.size.x - platformThickness, e.size.y + platformThickness));
					r->setPoint(3, sf::Vector2f(-platformThickness, platformThickness));					
				r->setPosition(e.position + offset + sf::Vector2f(-playerOffset.x, playerOffset.y));
				shapes.push_back(r);

				leftSide.push_back(r->getPosition() + r->getPoint(0) - sf::Vector2f(platformThickness*2, 0));
				leftSide.push_back(r->getPosition() + r->getPoint(1) - sf::Vector2f(platformThickness*2, 0));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(passWidth, -(e.size.y < scale ? e.size.y / 2 : passWidth)));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(passWidth, 0));
				break;

			case move_left_hinder:
				r->setOutlineThickness(-2);
				r->setOutlineColor(sf::Color::Red);
				r->setFillColor(sf::Color::Red);
				r->setPointCount(4);
				r->setPoint(0, sf::Vector2f(0, 0));
				r->setPoint(1, sf::Vector2f(-e.size.x, e.size.y));
				r->setPoint(2, sf::Vector2f(-e.size.x + scale, e.size.y + scale));
				r->setPoint(3, sf::Vector2f(scale, scale));
				r->setPosition(e.position + offset + playerOffset);
				shapes.push_back(r);

				rightSide.push_back(r->getPosition() + r->getPoint(0) + sf::Vector2f(scale, 0));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(scale, 0));
				leftSide.push_back(r->getPosition() + r->getPoint(1) - sf::Vector2f(passWidth, (e.size.y < scale ? e.size.y / 2 : passWidth)));
				leftSide.push_back(r->getPosition() + r->getPoint(1) - sf::Vector2f(passWidth, 0));
				break;
			case move_right_hinder:
				r->setOutlineThickness(-2);
				r->setOutlineColor(sf::Color::Red);
				r->setFillColor(sf::Color::Red);
				r->setPointCount(4);
				r->setPoint(0, sf::Vector2f(0, 0));
				r->setPoint(1, sf::Vector2f(e.size.x, e.size.y));
				r->setPoint(2, sf::Vector2f(e.size.x - scale, e.size.y + scale));
				r->setPoint(3, sf::Vector2f(-scale, scale));
				r->setPosition(e.position + offset + sf::Vector2f(-playerOffset.x, playerOffset.y));
				shapes.push_back(r);

				leftSide.push_back(r->getPosition() + r->getPoint(0) - sf::Vector2f(scale, 0));
				leftSide.push_back(r->getPosition() + r->getPoint(1) - sf::Vector2f(scale, 0));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(passWidth, -(e.size.y < scale ? e.size.y / 2 : passWidth)));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(passWidth, 0));
				break;
			case moving_block_single:
				r->setOutlineThickness(-4);
				r->setOutlineColor(sf::Color::Blue);
				r->setFillColor(sf::Color::Transparent);
				r->setPointCount(4);
					r->setPoint(0, sf::Vector2f(0, 0));
					r->setPoint(1, sf::Vector2f(scale, scale));
					r->setPoint(2, sf::Vector2f(0, scale*2));
					r->setPoint(3, sf::Vector2f(-scale, scale));
				//r->setOrigin(r->getLocalBounds().width/2, 0);
				r->setPosition(e.position + offset + sf::Vector2f(0, playerOffset.y));
				shapes.push_back(r);

				leftSide.push_back(r->getPosition() + r->getPoint(3) - sf::Vector2f(passWidth, 0));
				rightSide.push_back(r->getPosition() + r->getPoint(1) + sf::Vector2f(passWidth, 0));
				break;		
			case platform:
				break;
			default:
				break;
		}		
	}
	/*
	sf::CircleShape* end = new sf::CircleShape();
	end->setOutlineThickness(-2);
	end->setOutlineColor(sf::Color::Magenta);
	end->setFillColor(sf::Color::Transparent);
	end->setRadius(scale);
	end->setPointCount(3);
	end->setOrigin(end->getLocalBounds().width / 2, end->getLocalBounds().height / 2);
	end->setPosition(sf::Vector2f(0, passWidth) + shapes.back()->getPosition() + shapes.back()->getPoint(2));
	shapes.push_back(end);
	leftSide.push_back(end->getPosition() + sf::Vector2f(-passWidth, passWidth));
	rightSide.push_back(end->getPosition() + sf::Vector2f(passWidth, passWidth));
	*/
	for (auto lp : leftSide)
	{
		if (lp.x < min_point)
		{
			min_point = lp.x;
		}
	}
	for (auto rp : rightSide)
	{
		if (rp.x > max_point)
		{
			max_point = rp.x;
		}
	}		

	sf::Color backgroundColor = sf::Color(255, 128, 0);
	for (int i = 1; i < rightSide.size() - 1; i++)
	{
		sf::ConvexShape* boundariesRight = new sf::ConvexShape();
		boundariesRight->setPointCount(4);
		boundariesRight->setOutlineThickness(0);
		boundariesRight->setOutlineColor(sf::Color::Red);
		boundariesRight->setFillColor(backgroundColor);

		boundariesRight->setPoint(0, rightSide[i - 1]);
		boundariesRight->setPoint(1, rightSide[i]);
		boundariesRight->setPoint(2, sf::Vector2f(max_point + passWidth * 3, rightSide[i].y));
		boundariesRight->setPoint(3, sf::Vector2f(max_point + passWidth * 3, rightSide[i - 1].y));
		shapes.push_back(boundariesRight);		
	}

	for (int i = 1; i < leftSide.size() - 1; i++)
	{
		sf::ConvexShape* boundariesLeft = new sf::ConvexShape();
		boundariesLeft->setPointCount(4);
		boundariesLeft->setOutlineThickness(0);
		boundariesLeft->setOutlineColor(sf::Color::Red);
		boundariesLeft->setFillColor(backgroundColor);

		boundariesLeft->setPoint(0, leftSide[i - 1]);
		boundariesLeft->setPoint(1, leftSide[i]);
		boundariesLeft->setPoint(2, sf::Vector2f(min_point - passWidth * 3, leftSide[i].y));
		boundariesLeft->setPoint(3, sf::Vector2f(min_point - passWidth * 3, leftSide[i - 1].y));
		shapes.push_back(boundariesLeft);
	}
	
	return shapes;
}