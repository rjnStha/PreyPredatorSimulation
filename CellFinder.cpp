#include "CellFinder.h"
#include <iostream>
#include <queue> 

CellFinder::CellFinder(Coordinates coord, const std::vector<std::vector<Agent*> > grid) {
	currentcoord = coord;
	this->grid = grid;
	gridRow = grid.size();
	gridCol = grid[0].size();
}

//Uses BFS to transverse 2 cells(if levelTwo == true) else 1 cell from the given and returns
//	a. the list of empty cells when coyoteCheck == False
//	b. else returns the list of coyote cells
std::vector<Coordinates> CellFinder::BFS(Coordinates coord, bool agentCheck, bool levelTwo)
{
	std::vector<Coordinates> optimumCells;

	// Mark all the vertices as not visited 
	//vector of vector to replicate the grid
	std::vector<std::vector<bool>> visited;
	//set all the grid position into false
	for (int i = 0; i < gridRow; i++) {
		std::vector<bool> v;
		for (int j = 0; j < gridCol; j++) {
			v.push_back(false);
		}
		visited.push_back(v);
	}

	// Create a queue for BFS 
	std::queue <Coordinates> queueList;

	// Mark the current node as visited and enqueue it 
	visited[coord.currentRow][coord.currentCol] = true;
	queueList.push(coord);

	//loop until the queue is empty
	while (!queueList.empty())
	{
		// Dequeue a vertex from queue and check for safe place
		Coordinates coordCurrent = queueList.front();

		queueList.pop();

		//grid[coord.currentRow][coord.currentCol];
		
		//Possible positions from the given coordinates
		std::vector<Coordinates> listCoordinates;
		//Up
		listCoordinates.push_back({ coordCurrent.currentRow - 1, coordCurrent.currentCol });
		//Right
		listCoordinates.push_back({ coordCurrent.currentRow, coordCurrent.currentCol + 1 });
		//Down
		listCoordinates.push_back({ coordCurrent.currentRow + 1, coordCurrent.currentCol });
		//Left
		listCoordinates.push_back({ coordCurrent.currentRow , coordCurrent.currentCol - 1 });


		// Get all adjacent vertices of the dequeued Coordinates coord.
		for (Coordinates newcoord : listCoordinates) {
			
			int row = newcoord.currentRow;
			int col = newcoord.currentCol;
		
			//check if the adjacent position is within the range and is not visited
			if (rangeCheck(row, col) && !visited[row][col]) {
				// mark it visited and enqueue it 
				visited[row][col] = true;
				//only push the adjacent coordinates to the queue, left right up and down from the orignal coordinates
				if (levelTwo && levelCheck(coord, newcoord)) { queueList.push(newcoord); }
				
				//Check if the cell is NULL (safe cell for coyote)
				if (!agentCheck && grid[row][col] == NULL) {
					//Empty cell found!!!
					//Add the coordinates to the vector
					optimumCells.push_back(newcoord);
				}
				else if (agentCheck && grid[row][col] != NULL &&
					grid[row][col]->getType() != grid[currentcoord.currentRow][currentcoord.currentCol]->getType()) {
					//Coyote or RoadRunner cell found!!!
					//Add the coordinates to the vector
					optimumCells.push_back(newcoord);
				}
			}
		}
	}
	return optimumCells;
}

//returns the optimum coordinates for roadrunner in given position
Coordinates CellFinder::optimumRoadRunPosition() {
	
	//Algorithm Alert
	//a. if roadrunner,
		//a.1.check all the four adjacent positions in the grid
		//a.1.1 if no cyote in adjacent cells move random 1 cell
		//a.1.2 else move optimum 1 to 2 cells minimizing the num of coyotes adj in next step
		
	//get the list of empty cell within 1 cell distance
	std::vector<Coordinates> adjEmptyCells = BFS(currentcoord, false, false);
	//Check if there is atleast one empty cell 
	//Check if the list of coyote cell within 1 cell distance is empty
	if (!adjEmptyCells.empty() && BFS(currentcoord, true, false).empty()) {
		int randomNum;
		while (true) {

			//if the list is empty break
			if (adjEmptyCells.empty()) { break; }

			//find random coordinates to move the roadrunner to 
			randomNum = rand() % adjEmptyCells.size();
			int newRow = adjEmptyCells[randomNum].currentRow;
			int newCol = adjEmptyCells[randomNum].currentCol;

			//Check if there is coyote in the adjacent cell
			//Yes-> return the coordinate
			//No -> remove the coordinate form the list
			if (BFS(adjEmptyCells[randomNum], true, false).empty()) { return adjEmptyCells[randomNum]; }
			else { adjEmptyCells.erase(adjEmptyCells.begin()); }

		}
		
	}
	
	//get the list of empty cell within 2 cell distance
	std::vector<Coordinates> levelTwoEmptyCells = BFS(currentcoord, false, true);
	//CHECK FOR EMPTY LIST --> return the current position
	if (levelTwoEmptyCells.empty()) { return currentcoord; }
	//store optimum as the first element as the list 
	Coordinates optimumPos = levelTwoEmptyCells[0];
	
	//optimum number of safe adjacent cells for given safe cell
	int bestEmptyAdjCount = INT_MIN;
	int bestCoyoteAdjCount = INT_MAX;
	
	
	for (Coordinates cell : levelTwoEmptyCells) {

		//std::cout << "Empty coordinate: " << cell.currentRow << cell.currentCol << std::endl;
		
		
		//get the list of empty cell within 2 cell distance for given levelTwoEmptyCell
		int tempEmptyAdjCount = BFS(cell, false, true).size();
		//get the list of empty cell within 2 cell distance for given levelTwoEmptyCell
		int tempCoyoteAdjCount = BFS(cell, true, true).size();
		
		/*	
		std::vector<Coordinates> list = BFS(cell, false,true);
		std::cout << "	EmptyCells:  count: " << list.size() << std::endl;
		for (Coordinates emptyCell : list) {
			std::cout << " " << emptyCell.currentRow << emptyCell.currentCol<<" ";
		}
		list = BFS(cell, true, true);
		std::cout << "\n	CoyoteCells: count: " << list.size() << std::endl;
		for (Coordinates coyoteCell : list) {
			std::cout << " "<<coyoteCell.currentRow << coyoteCell.currentCol<<" ";
		}

		std::cout << " Count ( E - C ): " << tempEmptyAdjCount << tempCoyoteAdjCount << std::endl;
		*/

		//Check to filter the position with least number of adjacent coyote cells
		if( tempCoyoteAdjCount <= bestCoyoteAdjCount){
			//check equal coyote count
			if (tempCoyoteAdjCount == bestCoyoteAdjCount) { 
				//now check for equal empty count
				//if both coyote and empty count pairs are equal to best count --> no change
				//if empty count is less than best count -->no change
				if (tempEmptyAdjCount <= bestEmptyAdjCount ) { continue; }
			}
			//update the best counts and the optimum position
			optimumPos = cell;
			bestEmptyAdjCount = tempEmptyAdjCount;
			bestCoyoteAdjCount = tempCoyoteAdjCount;
		}

		//std::cout << "Optimum Count ( E - C ): " << bestEmptyAdjCount << " " << bestCoyoteAdjCount << std::endl;
		//std::cout<< "Optimum coordinates: " << optimumPos.currentRow << optimumPos.currentCol << "\n ----------- \n ";
	}
	//std::cout << "\n\n";
	
	return optimumPos;
	
}

//returns the optimum coordinates for coyote in given position
Coordinates CellFinder::optimumCoyotePosition() {
	
	//Algorithm Alert
	//b.1.check if there is roadrunner in the adjacent cell
		//b.1.1.yes->move to the cell
		//b.1.2.no->move to the random cell by 1

	//aget the list of roadrunner cell within 1 cell distance
	std::vector<Coordinates> adjCells = BFS(currentcoord, true, false);

	//Check if there is atleast one roadrunner in the adjacent position
	if (!adjCells.empty()) { return adjCells[0]; }

	//get the list of empty cell within 1 cell distance
	adjCells = BFS(currentcoord, false, false);

	//Check if there is atleast one empty adjacent position
	if (!adjCells.empty()) {
		//find random coordinates to move the roadrunner to 
		int randomNum = rand() % adjCells.size();
		return adjCells[randomNum];
	}

	return currentcoord;
}

//Checks if the given coordinates are in within the board
bool CellFinder::rangeCheck(int row, int col) { return (row >= 0) && (row < gridRow) && (col >= 0) && (col < gridCol);}

//returns the element at given coordinates
//returns NULL or pointer to the agent
AgentType CellFinder::agentAt(int row, int col) { return grid[row][col]->getType(); }

//Checks for the adjacent positions for given coordinates
bool CellFinder::levelCheck(Coordinates origin, Coordinates current) {
	//Check if the current coordinates is the adjacent coordinate of the origin 
	if ((current.currentRow == origin.currentRow) && (current.currentCol == origin.currentCol - 1 || current.currentCol == origin.currentCol + 1)) {
		//if row equal and one step to left or right
		return true;
	}
	else if ((current.currentCol == origin.currentCol) && (current.currentRow == origin.currentRow - 1 || current.currentRow == origin.currentRow + 1)) {
		//if column equal and one step up or down
		return true;
	}
	return false;
}
