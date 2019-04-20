#pragma once
#include "Agent.h"

// Roadrunners class is derived from base class Agent.
class Roadrunner : public Agent
{
public:
	//Constructor
	//Calling the Agent's constructor 
	Roadrunner(int x, int y) : Agent(x, y) {}

	// Returns the ASCII value of the agent
	const char representASCII() { return 'R'; }

	//Returns the type of agent, ROADRUNNER
	const AgentType getType() { return ROADRUNNER; }

};