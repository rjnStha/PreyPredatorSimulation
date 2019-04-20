#pragma once

enum AgentType { COYOTE, ROADRUNNER};

class Agent
{
public:
	//Constructor and Destructor 
	Agent() {}
	Agent(int x, int y) : currentRow(x), currentCol(y) {}
	virtual ~Agent() { }

	// Returns the ASCII value of the agent
	virtual const char representASCII() = 0;

	// Returns the type of agent
	virtual const AgentType getType() = 0;

	//getters//

	//get the current position
	int getCurrentRow() { return currentRow; }
	int getCurrentCol() { return currentCol; }

	//get the next position
	int getNextRow() { return nextRow; }
	int getNextCol() { return nextCol; }

	//returns the breedCount
	int getBreedCount() { return breedCount; }

	//setters//

	//set the current position
	void setCurrentRow(int row) { currentRow = row; }
	void setCurrentCol(int col) { currentCol = col; }

	//set the next position
	void setNextRow(int row) { nextRow = row; }
	void setNextCol(int col) { nextCol = col; }

	//sets the breed count
	void setBreedCount(int newCount) { breedCount = newCount; }
	
protected:
	//current position of the agent
	int currentRow = 0;
	int currentCol = 0;

	//next position of the agent
	int nextRow = 0;
	int nextCol = 0;

	//Number of consecutive steps to breeding
	int breedCount = 0;
		
};