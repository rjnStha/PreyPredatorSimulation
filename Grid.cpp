#include "Grid.h"
#include <iostream>
#include <stdlib.h>
#include <time.h> 

using namespace std;

//Constructor
//User inputs the dimension of the grid, populatest the grid
//and displays it
// coordinates --> (row,col)
Grid::Grid()
{
	//prompt and user input for the size of the grid environment
	cout << "Welcome to Cyote and Roadrunner Simulation" << endl;
	cout << "Enter the column size of the grid: ";
	cin >> gridCol;
	cout << "Enter the row size of the grid: ";
	cin >> gridRow;

	////create a new grid
	newGrid();
	//display the initial grid
	displayGrid();
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

	//initial number of roadrunner = average of num of col and row
	int initNumRoadRunner = (gridCol+gridRow)/2;

	//if less than ratio then just create one coyote
	//initial number of coyote = num of roadrunner / ratio
	int initNumCoyote;
	if (initNumRoadRunner < RATIO_RR_C) initNumCoyote = 1;
	else initNumCoyote = initNumRoadRunner* RATIO_RR_C;

	//Create the roadrunners 
	while (initNumRoadRunner != 0) {
		Coordinates coord = randomCoordinates();

		//Create roadrunner in empty spot
		if (grid[coord.row][coord.col] == NULL) {
			initNumRoadRunner--;
			breedAgent(ROADRUNNER, coord);
		}
	}
	//Create the Cyotes
	while (initNumCoyote != 0) 	{
		Coordinates coord = randomCoordinates();

		//Create coyote in empty spot
		if (grid[coord.row][coord.col] == NULL) {
			initNumCoyote--;
			breedAgent(COYOTE, coord);
		}
	}

}

//creates given number of new agent of given type
//if count is 1, breeds per project specification
void Grid::breedAgent(AgentType type, Coordinates coord) 
{	
	if (type == ROADRUNNER) { grid[coord.row][coord.col] = new Roadrunner(coord.row,coord.col); }
	else if (type == COYOTE) { grid[coord.row][coord.col] = new Coyote(coord.row, coord.col);	}
}

//returns random coordinates
Coordinates const Grid::randomCoordinates()  
{
	//range from 0 to total positions in grid (column * row)
	int uniDirectCoord = rand()% (gridCol*gridRow);
	//temp coordinate
	Coordinates temp;
	//rowPosition = total positions % numColumns
	temp.row = uniDirectCoord / gridCol;
	//rowPosition = total positions % numColumns
	temp.col = uniDirectCoord - temp.row * gridCol;
	return temp;
}

//displays the grid environment in ASCII
//and prints the number of cyotes and roadrunners
void const Grid::displayGrid()
{
	int numRoadRunnner = 0;
	int numCoyote = 0;

	cout << "\n";

	for (int i = 0; i < gridRow; i++) {
		cout << "	";
		for (int j = 0; j < gridCol; j++) {
			if (grid[i][j] == NULL) {
				cout << "-"<<" ";
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
				cout << grid[i][j]->representASCII()<<" ";

			}
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "	ROADRUNNER: " << numRoadRunnner << " COYOTE: " << numCoyote << "\n\n";
}

//starts the simulation
void Grid::startSimulation() {

	while (true) {
		
		char input;
		cout << "Enter q to exit:";
		cin >> input;
		
		if (input == 'q') break;

		//loop through the grid and check for the
		for (int row = 0; row < gridRow; row++) {
			for (int col = 0; col < gridCol; col++) {
				//Ignore the empty grid cells and execute the behaviour
				if (grid[row][col] != NULL) { storeBehaveAgent(grid[row][col]); }
			}
		}

		//display the grid
		displayGrid();
	}
}

//execute the behaviour of roadrunner
void Grid::storeBehaveAgent(Agent* currentAgent) {
	
	//get the current position
	int agentRow = currentAgent->getCurrentRow();
	int agentCol = currentAgent->getCurrentCol();
	//get the breed count 
	int breedCount = currentAgent->getBreedCount();
	
	if (currentAgent->getType() == ROADRUNNER) {
		
		//Execute the coyotes's behaviour
		/*
		a. if roadrunner,
			a.1.check all the four adjacent positions in the grid
				a.1.1 if no cyote in adjacent cells move random 1 cell
				a.1.2 else move optimum 1 to 2 cells minimizing the num of coyotes adj in next step
			a.2.check for 3rd consecutive time step
				a.2.1.yes->check for free adj cell
					a.2.1.1. if found breedand reset the time step
				a.2.2.no -> ++the time step
		*/

	}
	else if (currentAgent->getType() == COYOTE) {

		//base class pointer accessing the derived class member function with static casting
		int coyoteDeathCount = static_cast<Coyote*>(currentAgent)->getDeathCount();

		//Execute the Roadrunner's behaviour
		/*
		b. else if coyote,
			b.1.check if there is roadrunner in the adjacent cell
				b.1.1.yes->move to the cell
				b.1.2.no->move to the random cell by 1
			b.2.check for 8th consecutive time step
				b.2.1.yes->check for free adj cell
					b.2.1.1. if found breedand reset the time step
				b.2.2.no -> ++the time step
			b.3.check for 4th consecutive time step
				b.3.1.yes->die
				b.3.2.no -> ++the consecutive time step
		*/
	}

}

//
////Checks if the given coordinates are in 
//bool Organism::in_range(int xx, int yy)
//{
//	return (xx >= 0) && (xx < WORLDSIZE) && (yy >= 0) && (yy < WORLDSIZE);
//}