//Challenge Class!

#include "challenge.h"

int answer;

char * riddle;
char * ans1;
char * ans2;
char * ans3;

challenge::challenge()
{

}

challenge::challenge(char * question, char * a, char * b, char * c, int correctans)
{
	answer = correctans;
	riddle = question;
	ans1 = a;
	ans2 = b;
	ans3 = c;
}

bool challenge::checkAnswer(int choice)
{
	if (choice == answer)
	{
		return true;
	}
	else
	{
		return false;
	}
}

char * challenge::correctAnswer()
{
	if (answer == 0){
		return ans1;
	}
	else if (answer == 1){
		return ans2;
	}
	else{
		return ans3;
	}

}

char * challenge::getQuestion()
{
	return riddle;
}

char * challenge::getA()
{
	return ans1;
}

char * challenge::getB()
{
	return ans2;
} 

char * challenge::getC()
{
	return ans3;
}