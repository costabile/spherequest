// The Challenge List class
// Currently, will be a hardcoded list of challenges

#include "challengelist.h"

using namespace std;

challenge challenges[2];
int challengenum;

challengelist::challengelist(){
	challengenum = 0;
	challenges[0] = challenge("What is 1 + 1?", "2", "3", "4", 0);
	challenges[1] = challenge("What is 3 * 3?", "6", "9", "12", 1);
}

challenge challengelist::getChallenge(int number){

	return challenges[number];

}

challenge challengelist::getNextChallenge(){
	
	return challenges[challengenum];

}

void challengelist::incrementChallengeNum(){
	
	challengenum++;

}

void challengelist::decrementChallengeNum(){

	challengenum++;

}

string challengelist::getCurrentChallenge(){

	return challenges[challengenum].getQuestion();

}

string challengelist::getFirstChoice(){

	return challenges[challengenum].getA();

}

string challengelist::getSecondChoice(){

	return challenges[challengenum].getB();

}

string challengelist::getThirdChoice(){

	return challenges[challengenum].getC();

}

bool challengelist::answerChallenge(int choice){
	
	bool correct = challenges[challengenum].checkAnswer(choice);
	challengenum++;
	return correct;

}