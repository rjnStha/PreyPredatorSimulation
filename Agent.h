#pragma once

enum AgentType { COYOTE, ROADRUNNER};

struct Coordinates
{
	int currentRow = -1, currentCol = -1;
};

class Agent
{
public:
	//Constructor and Destructor 
	Agent() {}
	Agent(Coordinates coord) : coordinates(coord), breedCount(0), moved(false){}
	virtual ~Agent() { }

	// Returns the ASCII value of the agent
	virtual const char representASCII() = 0;

	// Returns the type of agent
	virtual const AgentType getType() = 0;

	//getters//

	//get the coordinates of the agent
	Coordinates getCoordinates() { return coordinates; }

	//returns the breedCount
	int getBreedCount() { return breedCount; }

	//checks if the agent moved or not
	bool hasMoved() { return moved; }

	//setters//

	//set the coordinates of the agent
	void setCoordinates(Coordinates coord) { coordinates = coord; }

	//sets the breed count
	void setBreedCount(int newCount) { breedCount = newCount; }

	//sets the moved variable
	void setMove(bool hasMoved) { moved = hasMoved; }
	
protected:
	//Coordniates of the agent enclosing both current and next position
	Coordinates coordinates;

	//Number of consecutive steps to breeding
	int breedCount = 0;
	
	//tracks if the agent moved for given time frame
	bool moved = false;
};