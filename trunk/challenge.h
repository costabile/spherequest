// SphereQuest - Software Engineering 3GC3 Game Project
// By: Ben Kybartas, Kaitlin Smith and Jason Costabile

// A Class which defines a challenge of the wisemen.
#ifndef CHALLENGE_H
#define CHALLENGE_H

class challenge{
public:
	challenge();
	challenge(char * question, char * a, char * b, char * c, int correctans); //constructor
	
	// Definition question = question to be asked
	//				a = answer a
	//				b = answer b
	//				c = answer c
	//				correctans = correct answer
	//						 0 = a
	//						 1 = b
	//						 2 = c
	~challenge(){}; //destructor
	bool checkAnswer(int choice);
	char * correctAnswer();
	char * getQuestion();
	char * getA();
	char * getB();
	char * getC();

};
#endif