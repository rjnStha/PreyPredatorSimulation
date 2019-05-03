#pragma once
#include "Agent.h"

// Coyotees class is derived from base class Agent.
class Coyote : public Agent
{
public:
	//Constructor
	//Calling the Agent's constructor 
	Coyote(Coordinates coord) : Agent(coord), deathCount(0) {}
	
	// Returns the ASCII value of the agent
	const char representASCII() { return 'C'; }

	//Returns the type of agent
	const AgentType getType() { return COYOTE; }
	
	//returns the deathCount
	int getDeathCount() { return deathCount; }

	//sets the death count
	void setDeathCount(int newCount) { deathCount = newCount; }
	
private:

	//Number of consevutive steps without eating to death
	int deathCount;
};