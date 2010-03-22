// SphereQuest - Software Engineering 3GC3 Game Project
// By: Ben Kybartas, Kaitlin Smith and Jason Costabile

// A Class which defines a challenge of the wisemen.
#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>

class challenge{
public:
	challenge();
	challenge(std::string question, std::string a, std::string b, std:: string c, int correctans); //constructor
	
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
	std::string correctAnswer();
	std::string getQuestion();
	std::string getA();
	std::string getB();
	std::string getC();

};
#endif