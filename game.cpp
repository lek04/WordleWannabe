#pragma warning(disable : 6262)
#define _CRT_SECURE_NO_WARNINGS
#include "utilities.h"
#include <iostream>
#include <cstring>
#include<cctype>
using namespace std;

const int MAXWORDS = 9000;

const char WORDFILENAME[] = "C:/CS31/Project5/wordlist.txt";
// Windows users:  Use / instead of \ for pathname separators.


bool isInDictionary(const char wlist[][MAXWORDLEN + 1], const char probe[]) {
	for (int k = 0; k != MAXWORDS; k++) {
		if (strcmp(probe, wlist[k]) == 0) {
			return true;
		}
	}
	return false;
}

int validword(const char probeword[]) { //checks if probe word is a 4-6 lowercase word
	if (strlen(probeword) < 4 || strlen(probeword) > 6) {
		return -1;
	}
	else {
		for (int k = 0; probeword[k] != '\0'; k++) {
			if (isupper(probeword[k])) {
				return -1;
			}if (!isalpha(probeword[k])) {
				return -1;
			}
		}
		return 0;
	}
}

int golds(const char targetWord[], char guess[]) {
	int gold = 0;
	for (int k = 0; guess[k] != '\0'; k++) {
		if ((guess[k] == targetWord[k]) && isalpha(guess[k])) { // gold if probe word has a same letter at same position
			gold++;
		}
	}
	return gold;
}

int silvers(const char targetWord[], char guess[]) {
	int silver = 0;
	char copy[MAXWORDLEN + 1];
	strcpy(copy, targetWord);//allows the copy to revert back to original targetword after being replaced by dashes when looped
	for (int s = 0; guess[s] != '\0'; s++) {//one letter of probe word will be compared to all letters of target word
		for (int j = 0; targetWord[j] != '\0'; j++) {
			if (guess[s] == copy[j] && isalpha(guess[s])) {
				silver++;
				copy[j] = '-'; //places letters of copy to prevent the same letter of target word being counted twice
				break;
			}
		}
	}

	return silver;
}


int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum) {
	char probe[100];
	char targetword[MAXWORDLEN + 1];
	int score = 0; //the number of tries
	int end = 0;

	if (nWords < 0 || wordnum >= nWords) {
		return -1; //prevents negative array sizes and out of bounds target word
	}

	while (end != 100) {
		cout << "Probe word: ";
		cin.getline(probe, 100);
		if (validword(probe) == 0) { //if probe word is 4-6 lowercase letters
			if (isInDictionary(words, probe)) { //if probe word is within wordlist
				score++; //records every valid try
				strcpy(targetword, words[wordnum]);
				int numgolds = golds(targetword, probe);
				int numsilvers = silvers(targetword, probe);
				if (numgolds == strlen(words[wordnum])) { //ends game if target word is found
					end = 100;
				}
				else {
					cout << "Golds: " << numgolds << ", Silvers: " << numsilvers - numgolds << endl;
				}
			}
			else {
				cout << "I don't know that word." << endl;
			}
		}
		else if (validword(probe) != 0) {
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
		}
	}

	return score; //successful run
}

int main()
{

	char wlist[MAXWORDS][MAXWORDLEN + 1];
	int n = getWords(wlist, 9000, WORDFILENAME);
	int gamereturn; //tries for individual rounds
	double totaltries; //accumulates tries from all rounds
	totaltries = 0;
	double average;
	int maxtries;
	int mintries;
	int numofrounds;
	numofrounds = 0;

	cout.setf(ios::fixed);
	cout.precision(2);

	cout << "How many rounds do you want to play? ";
	cin >> numofrounds;
	cin.ignore(10000, '\n');
	cout << endl;

	if (numofrounds < 0) {
		cout << "The number of rounds must be positive.";
		return 0;
	}

	if (n < 1) {
		cout << "No words were loaded, so I can't play the game." << endl;
		return 0;
	}

	for (int round = 1; round <= numofrounds; round++) {
		int randnum = randInt(0, n - 1); //randomizes word
		cout << "Round " << round << endl;
		cout << "The hidden word is " << strlen(wlist[randnum]) << " letters long." << endl;
		gamereturn = playOneRound(wlist, n, randnum); //game return is equal to the number of attempts for a round

		totaltries += gamereturn; //adds each round's tries
		average = (totaltries / round);

		if (round != 1) {
			if (gamereturn > maxtries) { //sets maxtries to the new highest tries 
				maxtries = gamereturn;
			}
			if (gamereturn < mintries) { //sets min tries to the new lowest tries
				mintries = gamereturn;
			}
		}
		else {
			maxtries = gamereturn; //starts off with the first round's stats
			mintries = gamereturn; //starts off with the first round's stats
		}


		if (gamereturn == 1) {
			cout << "You got it in 1 try." << endl;
			cout << "Average: " << average << ", minimum: " << mintries << ", maximum: " << maxtries << endl;
		}
		else if (gamereturn < 0) {
			return 0;
		}
		else {
			cout << "You got it in " << gamereturn << " tries." << endl;
			cout << "Average: " << average << ", minimum: " << mintries << ", maximum: " << maxtries << endl;
		}
		cout << endl;
	}
}