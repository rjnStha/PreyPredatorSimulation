#pragma once
#include <vector>
#include "Agent.h"
#include "Roadrunner.h"
#include "Coyote.h"

struct Coordinates
{
	int row;
	int col;
};

class Grid
{

public:
	//Constructor
	//User inputs the dimension of the grid, populatest the grid
	//and displays it	
	Grid();
		
private:
	///////////////////////////
	//	MEMBER VARIABLES	
	///////////////////////////

	// Initializing 2D vector of Agent pointer
	std::vector<std::vector<Agent*> > grid;
	
	//size of the Grid Column
	int gridCol;
	
	//size of the Grid Row
	int gridRow;

	//initial ratio of roadrunner to cycote
	const double RATIO_RR_C= 0.75;

	//consecutive time steps for roadrunner to breed
	const int BREED_ROADRUNNER = 3;

	//consecutive time steps for cyote to breed
	const int BREED_COYOTE = 8;

	//consecutive time steps for cyote to die without eating
	const int DEATH_COYOTE = 4;

	///////////////////////////
	//	MEMBER FUNCTIONS
	///////////////////////////

	//creates the grid of user given size 
	//and populates it with roadrunners and cyotes
	void newGrid();

	//creates given number of new agent of given type 
	void breedAgent(AgentType type, Coordinates coord);

	//displays the grid environment in ASCII
	//and prints the number of cyotes and roadrunners
	void const displayGrid();

	//randomly generate the coordinates within the grid
	Coordinates const randomCoordinates();
	
	//start the simulation
	void startSimulation();

	//stores the behaviour of agent for execution
	void storeBehaveAgent(Agent* currentRoadRunner);

	//stores the behaviour of agent for execution
	//void executeBehaveAgent(Agent* currentRoadRunner);

};