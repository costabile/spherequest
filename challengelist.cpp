// The Challenge List class
// Currently, will be a hardcoded list of challenges

#include "challengelist.h"
#define NUMBER_OF_CHALLENGES 10

int correctAns[NUMBER_OF_CHALLENGES];
char* questions[NUMBER_OF_CHALLENGES];
char* ans1[NUMBER_OF_CHALLENGES];
char* ans2[NUMBER_OF_CHALLENGES];
char* ans3[NUMBER_OF_CHALLENGES];

int challengenum;

challengelist::challengelist(){
	// Riddles are from this website: http://inspirationalriddles.blogspot.com/
	challengenum = 0;

	questions[0] = "What has both light and darkness?";
	ans1[0] = "The mind";
	ans2[0] = "The heart";
	ans3[0] = "A room";
	correctAns[0] = 0;
	questions[1] = "What is the one thing that man will always surrender to?";
	ans1[1] = "Bears";
	ans2[1] = "His appetite";
	ans3[1] = "Fear";
	correctAns[1] = 1;
	questions[2] = "What is the fastest way to show happiness and sadness?";
	ans1[2] = "A smile and a tear";
	ans2[2] = "A giggle and a frown";
	ans3[2] = "A grin and a cry";
	correctAns[2] = 0;
	questions[3] = "Who is small and can give much more than she gave as a maiden?";
	ans1[3] = "A rock";
	ans2[3] = "A fruit";
	ans3[3] = "A seed";
	correctAns[3] = 2;
	questions[4] = "This could be many things but is only one.  It leaves right away if you wait for it to return.  What is it?";
	ans1[4] = "A train";
	ans2[4] = "Love";
	ans3[4] = "The present";
	correctAns[4] = 2;
	questions[5] = "I will tell you what nobody ever did and nobody ever can.  What am I?";
	ans1[5] = "Silence";
	ans2[5] = "Memories";
	ans3[5] = "Dreams";
	correctAns[5] = 0;
	questions[6] = "I know you better than anyone else, and if your smile is true.  What am I?";
	ans1[6] = "Your toothbrush";
	ans2[6] = "The mirror";
	ans3[6] = "Your mind";
	correctAns[6] = 1;
	questions[7] = "We have been together since the day you were born and you don't have a place without me. What am I?";
	ans1[7] = "Your life";
	ans2[7] = "Your spirit";
	ans3[7] = "Your name";
	correctAns[7] = 2;
	questions[8] = "I accept you during the day but I don't tolerate you at night. What is it?";
	ans1[8] = "Insomnia";
	ans2[8] = "Hunger";
	ans3[8] = "Fear";
	correctAns[8] = 0;
	questions[9] = "My hands are small but contain the unsurpassable eternity. What am I?";
	ans1[9] = "A child";
	ans2[9] = "A clock";
	ans3[9] = "A flower";
	correctAns[9] = 1;

}

void challengelist::setChallengeNum(int newNum) {
	challengenum = newNum;
}

int challengelist::getChallengeNum(){
	return challengenum;
}

void challengelist::incrementChallengeNum(){
	
	challengenum++;

}

void challengelist::decrementChallengeNum(){

	challengenum--;

}

char * challengelist::getCurrentChallenge(){

	return questions[challengenum];

}

char * challengelist::getFirstChoice(){

	return ans1[challengenum];

}

char * challengelist::getSecondChoice(){

	return ans2[challengenum];

}

char * challengelist::getThirdChoice(){

	return ans3[challengenum];

}

bool challengelist::answerChallenge(int choice){
	
	bool correct = (choice == correctAns[challengenum]);
	challengenum++;
	return correct;

}