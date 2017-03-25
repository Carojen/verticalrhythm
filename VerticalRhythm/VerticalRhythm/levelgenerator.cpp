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

std::vector<action> LevelGenerator::createActions(std::vector<double> beats)
{	
	std::vector<action> actions;
	//Kolla hur lång rytmen är
	double rhythmLength = 0;
	for (auto n : beats)
	{
		rhythmLength += n;
	}
	int randomValue = std::rand() % 2;
	double currentTime = 0;

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
		if (i < beats.size() - 1)
		{
			length = rand() % (int)(beats[i + 1] * 4) * 0.25;
		}
		else //Det sista beatet får samma längd som det första
		{
			length = beats[0];
		}
		
		a.stoptime = currentTime + length;
		actions.push_back(a);

		randomValue = rand() % 2;				
	}
	
	return actions;
}

std::vector<action> LevelGenerator::createActions(rhythm r)
{
	return createActions(GetBeats(r));
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
		std::cout << "Current time: " << currentTime << " length: " << a.stoptime - a.starttime << std::endl;
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
		}
		else if (a.word == brake)
		{
			brakeTime += a.stoptime - a.starttime;
			g.type = moving_block_single;
			g.position = avatar.position;
			g.size.x = 200;
			g.size.y = 1;
		}
		geometryElements.push_back(g);
	}
	return geometryElements;
}

std::vector<geometry> LevelGenerator::GetGeometry2(std::vector<action> actions)
{
	std::vector<action> moves;
	std::vector<action> brakes;
	double moveTime = 0;
	double brakeTime = 0;
	std::vector<geometry> geometryElements;
	/*
	for (action a : actions)
	{
		//Registrera spelarens position innan den går in i bansegmentet
		avatar.position = sf::Vector2f(moveTime * avatar.speed.x, (a.starttime - brakeTime) * avatar.speed.y);		
		geometry g;
		//Om det är en inbromsning, registrera det
		if (a.word == brake)
		{
			//Om det finns en påbörjad sväng som överlappar med inbromsningen, dela upp handlingarna i flera som inte överlappar			
			if (!moves.empty() && moves.back().stoptime > a.starttime)
			{
				brakes.push_back(a);
				brakeTime += a.stoptime - a.starttime;

				action move1, move2, move3 = moves.back();
				action brake1, brake2 = a;

				//Six cases: 
				// 1. brake and move are at the same time
				// 2. they begin at the same time, brake ends earlier
				// 3. they begin at the same time, brake ends later
				// 4. move has begun, both ends at the same time
				// 5. move has begun, brake ends earlier
				// 6. move has begun, brake ends later

				if (move1.starttime == brake1.starttime)
				{
					if (move1.stoptime > brake1.stoptime)
					{
						move1.stoptime, move2.starttime = brake1.stoptime;
						moves.back() = move1;
						moves.push_back(move2);

						g = geometryElements.back(); //Kombination av senaste move och brake
						g.size.x = (brake1.stoptime - brake1.starttime) * avatar.speed.x;
						g.size.y = (brake1.stoptime - brake1.starttime) * avatar.speed.y;
						g.type = move1.direction == left ? move_left_passage : move_right_passage;
						geometryElements.push_back(g);
						geometryElements.push_back(ActionToGeometry(move2));
					}
					else if(move1.stoptime < brake1.stoptime)
					{
						brake1.stoptime, brake2.starttime = move1.stoptime;
						brakes.back() = brake1;
						brakes.push_back(brake2);
						g = geometryElements.back(); //Kombination av senaste move och brake

					}
					else
					{
						move2.starttime, move3.starttime, brake2.starttime = -1;
					}
				}
				else
				{
					if (move1.stoptime == brake1.stoptime)
					{
						move1.stoptime, move2.starttime = brake1.starttime;
						moves.back() = move1;
						moves.push_back(move2);
						move3.starttime, brake2.starttime = -1;
					}
					else
					{
						
					}
				}

			}
			else
			{
				brakes.push_back(a);
				brakeTime += a.stoptime - a.starttime;
				g = ActionToGeometry(a);
				geometryElements.push_back(g);
			}			
		}
		else if (a.word == move)
		{
			//Om det finns en påbörjad inbromsning överlappar med svängen
			if (!brakes.empty() && brakes.back().stoptime >= a.starttime)
			{
				moves.push_back(a);
				moveTime += (a.stoptime - a.starttime) * a.direction == left ? -1 : 1;

				//revertera avatarens position lika mycket som inbromsningen överlappar gånger avatarens fallhastighet
				
				//Ändra elementet från moving platform till move_passage om de har samma längd
				if (brakes.back().starttime == a.starttime && brakes.back().stoptime == a.stoptime)
				{
					geometryElements.back().type = a.direction == left ? move_left_passage : move_right_passage;
					//Uppdatera elementets storlek till att motsvara en ledge
					geometryElements.back().size.x = (a.stoptime - a.starttime) * avatar.speed.x;
				}			
			}
			else
			{
				moves.push_back(a);
				moveTime += (a.stoptime - a.starttime) * a.direction == left ? -1 : 1;
				g = ActionToGeometry(a);
				geometryElements.push_back(g);
			}			
		}
	}	
	*/
	return geometryElements;
}

geometry LevelGenerator::ActionToGeometry(action a)
{
	geometry g;
	if (a.word == brake)
	{
		g.type = moving_block_single;
		g.position = avatar.position;
		g.size.x = 200;
		g.size.y = 1;
	}
	else
	{
		g.type = a.direction == left ? move_left_ledge : move_right_ledge;
		g.position = avatar.position;
		g.size.x = (a.stoptime - a.starttime) * avatar.speed.x;
		g.size.y = (a.stoptime - a.starttime) * avatar.speed.y;
	}
	return g;
}
/*
std::vector<geometry> LevelGenerator::GetGeometry2(std::vector<action> actions)
{
	std::vector<action> moves;
	std::vector<action> brakes;
	double moveTime = 0;
	double brakeTime = 0;
	std::vector<geometry> geometryElements;
	geometry g;
	for (action a : actions)
	{
		std::cout << std::endl << "T: " << a.starttime;
		avatar.position = sf::Vector2f(moveTime * avatar.speed.x, (a.starttime - brakeTime) * avatar.speed.y);
		g.position = avatar.position;
		std::cout << " P: " << g.position.x << ", " << g.position.y;
		g.size = sf::Vector2f(avatar.speed.x * (a.stoptime - a.starttime), avatar.speed.y * (a.stoptime - a.starttime));
		std::cout << " L: " << g.size.x << ", " << g.size.y;
				
		//Sortera handlingarna
		if (a.word == brake)
		{
			brakes.push_back(a);
			//Kolla samtida brake och move
			if (!moves.empty() && !geometryElements.empty() && moves.back().starttime == a.starttime)
			{
				//modifiera senaste handlingen till att bli en mer tvär sväng åt samma riktning
				geometryElements.back().size.y -= (a.stoptime - a.starttime) * avatar.speed.y;
			}
			else
			{
				g.type = keyword::moving_block_single;
				g.size.y = 0;
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
				//modifiera senaste handlingen till att bli en tvär sväng
				//Det är just nu ett moving_block_single, men ska bli en move_(left|right)_ledge
				// positionen kan behöva uppdateras
				geometryElements.back().type = a.direction == left ? move_left_ledge : move_right_ledge;
				if( brakes.back().stoptime == a.stoptime) brakes.back().stoptime = a.stoptime;
				geometryElements.back().size.y = ((a.stoptime - a.starttime)-(brakes.back().stoptime - brakes.back().starttime)) * avatar.speed.y; 				
			}
			//Kolla om move har en brake direkt före alternativt överlappar något
			//this only makes sense if braking means the avatar has a slower speed when it starts moving again
			// or it is a vertically moving block
			else if(!brakes.empty() && !geometryElements.empty() && brakes.back().stoptime >= a.starttime)
			{
				//modifiera senaste handlingen från att vara moving_block till att vara side_passage
				geometryElements.back().type = a.direction == left ? move_left_passage : move_right_passage;				
			}
			else
			{
				g.type = a.direction == left ? keyword::move_left_ledge : keyword::move_right_ledge;
				geometryElements.push_back(g);
			}
			moveTime += (a.stoptime - a.starttime) * a.direction == left ? -1 : 1;			
		}
	}
	return geometryElements;
}
*/

std::vector<sf::Shape*> LevelGenerator::GetShapes(std::vector<geometry> geometryElements, sf::Vector2f offset)
{
	std::vector<sf::Shape*> shapes;
	
	sf::Vector2f playerOffset = sf::Vector2f();
	std::cout << std::endl;

	sf::CircleShape* zero = new sf::CircleShape();
	zero->setOutlineThickness(2);
	zero->setOutlineColor(sf::Color::Cyan);
	zero->setFillColor(sf::Color::Transparent);
	zero->setRadius(5);
	zero->setPointCount(3);
	zero->setOrigin(zero->getLocalBounds().width / 2, zero->getLocalBounds().height / 2);
	zero->setPosition(0, 0);
	shapes.push_back(zero);
	
	for (auto e : geometryElements)
	{		
		sf::RectangleShape* r = new sf::RectangleShape();		

		sf::CircleShape* c = new sf::CircleShape();
		c->setOutlineThickness(2);
		c->setOutlineColor(sf::Color::White);
		c->setFillColor(sf::Color::Transparent);
		c->setRadius(5);
		c->setOrigin(c->getLocalBounds().width / 2, c->getLocalBounds().height / 2);				
		c->setPosition(e.position);	

		float platformThickness = 2;

		std::cout << "Position: " << e.position.x << ", " << e.position.y <<". Size: " << e.size.x << ", " << e.size.y << std::endl;
		switch (e.type)		{

			case move_left_ledge:				
				r->setOutlineThickness(-4);
				r->setOutlineColor(sf::Color::Red);
				r->setFillColor(sf::Color::Transparent);
				r->setSize(e.size);
				r->setOrigin(r->getLocalBounds().width, 0);
				r->setPosition(e.position);
				shapes.push_back(r);
				break;
			case move_right_ledge:	
				r->setOutlineThickness(-4);
				r->setOutlineColor(sf::Color::Magenta);
				r->setFillColor(sf::Color::Transparent);
				r->setSize(e.size);				
				r->setPosition(e.position);
				shapes.push_back(r);
				break;
			case moving_block_single:
				r->setOutlineThickness(-4);
				r->setOutlineColor(sf::Color::Blue);
				r->setFillColor(sf::Color::Transparent);
				r->setSize(e.size);
				r->setOrigin(r->getLocalBounds().width/2, 0);
				r->setPosition(e.position);
				shapes.push_back(r);
				break;			
			default:
				break;
		}
		shapes.push_back(c);
	}

	return shapes;
}