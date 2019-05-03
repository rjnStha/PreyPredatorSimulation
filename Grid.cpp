#include "Grid.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <thread>

//Constructor
//User inputs the dimension of the grid, populatest the grid
//and displays it
// coordinates --> (row,col)
Grid::Grid()
{
	//prompt and user input for the size of the grid environment
	std::cout << "Welcome to Cyote and Roadrunner Simulation" << std::endl;
	std::cout << "Enter the column size of the grid: ";
	std::cin >> gridCol;
	std::cout << "Enter the row size of the grid: ";
	std::cin >> gridRow;

	//create a new grid
	newGrid();
	//start the simulation
	startSimulation();
}

//creates the grid of user given size 
//and populates it with roadrunners and cyotes
void Grid::newGrid()
{
	//Creating the vector of vector initialized to NULL
	for (int i = 0; i < gridRow; i++) {
		std::vector<Agent*> temp;
		for (int j = 0; j < gridCol; j++) {
			temp.push_back(NULL);
		}
		grid.push_back(temp);
	}

		
	//seed the random generator for random distribution of agents
	srand((unsigned int) time(NULL));

	//user input roadrunner and coyote number
	int initNumRoadRunner;
	int initNumCoyote;
	std::cout << "Enter the number of roadrunners: ";
	std::cin >> initNumRoadRunner;
	std::cout << "Enter the number of coyotes: ";
	std::cin >> initNumCoyote;

	//Place Roadrunners in random positions
	while (initNumRoadRunner != 0) {
		Coordinates coord = randomCoordinates();

		//Create roadrunner in empty spot
		if (grid[coord.currentRow][coord.currentCol] == NULL) {
			initNumRoadRunner--;
			breedAgent(ROADRUNNER, coord);
		}
	}

	//Place Coyotes in random positions
	while (initNumCoyote != 0) 	{
		Coordinates coord = randomCoordinates();

		//Create coyote in empty spot
		if (grid[coord.currentRow][coord.currentCol] == NULL) {
			initNumCoyote--;
			breedAgent(COYOTE, coord);
		}
	}

}

//creates given number of new agent of given type
//if count is 1, breeds per project specification
void Grid::breedAgent(AgentType type, Coordinates coord) 
{	
	if (type == ROADRUNNER) { grid[coord.currentRow][coord.currentCol] = new Roadrunner(coord); }
	else if (type == COYOTE) { grid[coord.currentRow][coord.currentCol] = new Coyote(coord);	}
}

//returns random coordinates
Coordinates const Grid::randomCoordinates()  
{
	//range from 0 to total positions in grid (column * row)
	int uniDirectCoord = rand()% (gridCol*gridRow);
	//temp coordinate
	Coordinates temp;
	//rowPosition = total positions % numColumns
	temp.currentRow = uniDirectCoord / gridCol;
	//rowPosition = total positions % numColumns
	temp.currentCol = uniDirectCoord - temp.currentRow * gridCol;
	return temp;
}

//displays the grid environment in ASCII
//and prints the number of cyotes and roadrunners
void const Grid::displayGrid()
{
	int numRoadRunnner = 0;
	int numCoyote = 0;

	std::cout << "\n";
	for (int i = 0; i < gridRow; i++) {
		std::cout << i<< "	";
		for (int j = 0; j < gridCol; j++) {
			if (grid[i][j] == NULL) {
				std::cout << "-"<<" ";
			}
			else {
				
				if (grid[i][j]->getType() == COYOTE) {
					//increase the num
					numCoyote++;
				}
				else if (grid[i][j]->getType() == ROADRUNNER) {
					//increase the num
					numRoadRunnner++;
				}
				std::cout << grid[i][j]->representASCII() << " ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	std::cout << "	ROADRUNNER: " << numRoadRunnner << " COYOTE: " << numCoyote << "\n\n";
}

//starts the simulation
void Grid::startSimulation() {
	while (true) {
		//display the grid
		displayGrid();
		system("PAUSE");

		std::vector<Coordinates> nonEmptyPos;

		//loop through the grid and find the agent occupied cells
		for (int row = 0; row < gridRow; row++) {
			for (int col = 0; col < gridCol; col++) {
				//add the non empty grid cells to a vector
				if (grid[row][col] != NULL) {
					//set the move flag to false
					grid[row][col]->setMove(false);
					//add the position to the vector 
					nonEmptyPos.push_back({ row,col }); 
				}
			}
		}
		//way to clear the screen
		std::cout << std::string(50, '\n');
		//required to display grid before and after execution of each agent
		displayGrid();

		//randomly shuffle the vector storing the non-empty positions
		std::random_shuffle(nonEmptyPos.begin(), nonEmptyPos.end());
		//execute the move for each agent in the list
		for (Coordinates c : nonEmptyPos) { executeMove(grid[c.currentRow][c.currentCol]);}


	}
}

//executes the behaviour of the agent
void Grid::executeMove(Agent* currentAgent)
{
	//check if the agent has moved -> true then return
	if (currentAgent->hasMoved()) { return; }

	//get the coordinates of the agent
	Coordinates coord = currentAgent->getCoordinates();
	//get the breed count 
	int breedCount = currentAgent->getBreedCount();

	CellFinder cellFind(coord, grid);

	if (currentAgent->getType() == ROADRUNNER) {
		//Find the optimum position for current agent to move
		Coordinates newPos = cellFind.optimumRoadRunPosition();
	
		//Execute the coyotes's behaviour
		//Set the new position to roadRunner
		grid[newPos.currentRow][newPos.currentCol] = currentAgent;
		//Set the current position to empty
		grid[coord.currentRow][coord.currentCol] = NULL;
		//update all the member varivales 
		currentAgent->setCoordinates(newPos);
		//set the move flag to true
		currentAgent->setMove(true);
				
		//Algorithm Alert
		//a.2. check for the 3rd cosecutive safe move by the roadrunner 
		//	a.2.1. yes -> check for an an empty position to breed a new roadrunner
		//	a.2.1 no -> ++breedCount 
		if (breedCount == 3) {
			std::vector<Coordinates> list = cellFind.BFS(newPos,false,false);
			//OBSERVATION: ALways has an empty adjacent cell since the agent moves first then breeds
			// leaving an empty cell to breed
			if(!list.empty()) {
				//breed a new Roadrunner
				breedAgent(ROADRUNNER, list[0]);
				//resetting the breed count
				currentAgent->setBreedCount(0);
			}
		}
		else currentAgent->setBreedCount(++breedCount);

		//print the change in each agent's data
		/*breedCount = currentAgent->getBreedCount();
		std::cout << "\n\n";
		std::cout << "ROADRUNNER AT:" << coord.currentRow << coord.currentCol << "\n";
		std::cout << "Optimum cell:" << newPos.currentRow << newPos.currentCol << "\n";
		std::cout << "BreedCount:" << breedCount << "\n";*/

	}
	else if (currentAgent->getType() == COYOTE) {
		
		//Find the optimum position for current agent to move
		Coordinates newPos = cellFind.optimumCoyotePosition();
		
		//Check if the roadrunner was captured in new position
		//Reset the death count 
		//else increase the death count
		if (grid[newPos.currentRow][newPos.currentCol] != NULL) { static_cast<Coyote*>(currentAgent)->setDeathCount(0);}

		//Execute the Roadrunner's behaviour
		//Set the new position to roadRunner
		grid[newPos.currentRow][newPos.currentCol] = currentAgent;
		//Set the current position to empty
		grid[coord.currentRow][coord.currentCol] = NULL;
		//update all the member varivales 
		currentAgent->setCoordinates(newPos);
		//set the move flag to true
		currentAgent->setMove(true);
			
		//Algorithm Alert
		//b.2.check for 8th consecutive time step
			//b.2.1.yes->check for free adj cell
			//b.2.1.1. if found breed and reset the time step
			//b.2.2.no -> ++the time step
		coord = currentAgent->getCoordinates();
		if (breedCount == 8) {
			std::vector<Coordinates> list = cellFind.BFS(coord, false, false);
			//Check if there atleast one empty cell adj to the coyote
			if (!list.empty()) { 
				breedAgent(COYOTE,list[0]);
				currentAgent->setBreedCount(0);
			}
		
		} else {currentAgent->setBreedCount(++breedCount);}
		
		//base class pointer accessing the derived class member function with static casting
		int coyoteDeathCount = static_cast<Coyote*>(currentAgent)->getDeathCount();
		
		//Algorithm Alert
		//b.3.check for 4th consecutive time step without capturing roadrunner
		//	b.3.1.yes->die
		//	b.3.2.no -> ++the consecutive time step
		if (static_cast<Coyote*>(currentAgent)->getDeathCount() == 4) { grid[newPos.currentRow][newPos.currentCol] = NULL;}
		else{ static_cast<Coyote*>(currentAgent)->setDeathCount(++coyoteDeathCount);}
		
		//print the change in each agent's data
		/*breedCount = currentAgent->getBreedCount();
		std::cout << "\n\n";
		std::cout << "COYOTE AT:" << coord.currentRow << coord.currentCol << "\n";
		std::cout << "optimum: " << newPos.currentRow << newPos.currentCol << "\n";
		std::cout << "DeathCount:" << coyoteDeathCount << "\n";
		std::cout << "BreedCount:" << breedCount << "\n";*/

	}

	//print the agent posiiton change in each agent
	//displayGrid();
}
