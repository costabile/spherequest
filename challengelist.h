// SphereQuest - Software Engineering 3GC3 Game Project
// By: Ben Kybartas, Kaitlin Smith and Jason Costabile

// A Class which defines a list of wisemen Challenges

#ifndef CHALLENGELIST_H
#define CHALLENGELIST_H

#include "challenge.h"

class challengelist{

public:
	challengelist();
	~challengelist(){};

	/*
	The Below Options should generally not be used since they directly
	affect the challenge list although the challenge list is designed to
	work on its own
	*/
	challenge getChallenge(int number); // Get a specific challenge
	challenge getNextChallenge(); //Get the actual challenge object
	void incrementChallengeNum(); //manually go to the next challenge
	void decrementChallengeNum(); //manually go to the previous challenge

	/*
	These operators should be used since they are abstract and require no
	manual changes to be made to the challenge list
	*/
	char * getCurrentChallenge(); // Get a string of the current challenge
	char * getFirstChoice(); // Get the first choice (a)
	char * getSecondChoice(); // Get the second choice (b)
	char * getThirdChoice(); // Get the third choice (c)
	bool answerChallenge(int choice); //Input an answer (0 = a, 1 = b, 2 = c) and the function returns true if correct
									  //and false if incorrect. Afterwards, the challenge list moves to the next challenge
};

#endif