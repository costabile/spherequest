// The Challenge List class
// Currently, will be a hardcoded list of challenges

#include "challengelist.h"

challenge challenges[10];

int challengenum;

challengelist::challengelist(){
	// Riddles are from this website: http://inspirationalriddles.blogspot.com/
	challengenum = 0;

	challenges[0] = challenge("What has both light and darkness?", "The mind", "The heart", "A room", 0);
	challenges[1] = challenge("What is the one thing that man will always surrender to?", "Bears", "His appetite", "Fear", 1);
	challenges[2] = challenge("What is the fastest way to show happiness and sadness?", "A smile and a tear", "A giggle and a frown", "A grin and a cry", 0);
	challenges[3] = challenge("Who is small and can give much more than she gave as a maiden?", "A rock", "A fruit", "A seed", 2);
	challenges[4] = challenge("This could be many things but is only one.  It leaves right away if you wait for it to return.  What is it?", "A train", "Love", "The present", 2);
	challenges[5] = challenge("I will tell you what nobody ever did and nobody ever can.  What am I?", "Silence", "Memories", "Dreams", 0);
	challenges[6] = challenge("I know you better than anyone else, and if your smile is true.  What am I?", "Your toothbrush", "The mirror", "Your mind", 1);
	challenges[7] = challenge("We have been together since the day you were born and you don't have a place without me. What am I?", "Your life", "Your spirit", "Your name", 2);
	challenges[8] = challenge("I accept you during the day but I don't tolerate you at night. What is it?", "Insomnia", "Hunger", "Fear", 0);
	challenges[9] = challenge("My hands are small but contain the unsurpassable eternity. What am I?", "A child", "A clock", "A flower", 1);

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

	challengenum--;

}

char * challengelist::getCurrentChallenge(){

	return challenges[challengenum].getQuestion();

}

char * challengelist::getFirstChoice(){

	return challenges[challengenum].getA();

}

char * challengelist::getSecondChoice(){

	return challenges[challengenum].getB();

}

char * challengelist::getThirdChoice(){

	return challenges[challengenum].getC();

}

bool challengelist::answerChallenge(int choice){
	
	bool correct = challenges[challengenum].checkAnswer(choice);
	challengenum++;
	return correct;

}