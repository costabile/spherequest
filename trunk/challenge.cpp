//Challenge Class!

#include "challenge.h"

using namespace std;

int answer;

string riddle;
string ans1;
string ans2;
string ans3;

challenge::challenge(string question, string a, string b, string c, int correctans)
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

string challenge::correctAnswer()
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

string challenge::getQuestion()
{
	return riddle;
}

string challenge::getA()
{
	return ans1;
}

string challenge::getB()
{
	return ans2;
}

string challenge::getC()
{
	return ans3;
}