#pragma once
#include <iostream>
#include <vector>
#include "Agent.h"

class CellFinder
{
public:
	CellFinder(Coordinates coord, const std::vector<std::vector<Agent*> > grid);
	
	///////////////////////////
	//	MEMBER FUNCTIONS
	///////////////////////////
	
	//Uses BFS to transverse upto 2 cells from the given and returns
	//	a. the list of empty cells when coyoteCheck == False
	//	b. else returns the list of coyote cells
	std::vector<Coordinates> BFS(Coordinates cood, bool agentCheck, bool levelTwo);

	//returns the optimum coordinates for roadrunner in given position
	Coordinates optimumRoadRunPosition();

	//returns the optimum coordinates for coyote in given position
	Coordinates optimumCoyotePosition();

private:
	///////////////////////////
	//	MEMBER VARIABLES	
	///////////////////////////

	Coordinates currentcoord;
	std::vector<std::vector<Agent*> > grid;
	int gridRow, gridCol;


	///////////////////////////
	//	MEMBER FUNCTIONS
	///////////////////////////

	//Checks for the adjacent positions for given coordinates
	bool levelCheck(Coordinates origin, Coordinates current);

	//Checks if the given coordinates are in within the board
	bool rangeCheck(int row, int col);

	//returns the element at given coordinates
	//returns NULL or pointer to the agent
	AgentType agentAt(int row, int col);

};

