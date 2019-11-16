#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cstring>
#include "utilities.cpp"
using namespace std;

const int MAXWORDS = 10000;
const int MAXWORDLENGTH = 6; //Using constant variables for max amount of words and length

void playGame(const char words[][7], int nWords, int nRounds); //Iterates playOneRound for n rounds
int playOneRound(const char words[][7], int nWords, int wordnum); //Required function
bool correctSyntax(char trialWord[100]); //Helper function for playOneRound
bool wordExists(const char words[][7], char trialWord[100], int nWords); //Helper function for playOneRound

int main()
{
    const char WORDFILENAME[] = "words.txt"; //Import the file
    char words[MAXWORDS][MAXWORDLENGTH+1]; //Create an array to hold those words
    int nWords = getWords(words, MAXWORDS, WORDFILENAME); //Count the number of words
    if (nWords < 1)
    {
        cout << "No words were loaded, so I can't play the game.\n";
        return 0;
    }
    int nRounds;
    cout << "How many rounds do you want to play?: ";
    cin >> nRounds; //Take user input for rounds to play, this will be used to iterate playOneRound
    if (nRounds <= 0)
    {
        cout << "The number of rounds must be positive.\n";
        return 0; //Terminate program if rounds is not positive, as specified in the specs
    }
    else
    {
        cin.ignore(100000, '\n'); //Very important! Code somehow won't iterate after round one correctly without this
        playGame(words, nWords, nRounds); //Begin iteration of playOneRound
    }
}

void playGame(const char words[][7], int nWords, int nRounds) //Simply cout-ing, so no return value required
{
    int totalAttempts = 0;
    int maxNum = 0;
    int minNum = 10000;
    int attemptNum = 0; //Initialize some variables here
    for (int k = 1; k <= nRounds; k++) //Iteration begins here
    {
        int wordnum = randInt(0, nWords-1);
        cout << "\nRound " << k << "\n";
        cout << "The mystery word is " << strlen(words[wordnum]) << " letters long.\n";
        attemptNum = playOneRound(words, nWords, wordnum); //Usage of playOneRound to return score value
        if (attemptNum == 1)
        {
            cout << "You got it in 1 try.\n"; //Win condition message for 1 attempt
        }
        else if (attemptNum == -1)
        {
            break; //Error message as specified in specs
        }
        else
        {
            cout << "You got it in " << attemptNum << " tries.\n"; //Win condition message for over 1 attempt
        }
        totalAttempts += attemptNum; //After each iteration, add to a total
        cout.setf(ios::fixed);
        cout.setf(ios::showpoint);
        cout.precision(2); //When calculating the average, show two decimal points
        double average = static_cast<double>(totalAttempts)/k; //Convert an int to a double to show average properly
        if (attemptNum > maxNum)
        {
            maxNum = attemptNum; //Find max attempts tried
        }
        if (attemptNum < minNum)
        {
            minNum = attemptNum; //Find min attempts used
        }
        cout << "Average: " << average << ", minimum: " << minNum << ", maximum: " << maxNum << endl;
    }
}

int playOneRound(const char words[][7], int nWords, int wordnum)
{
    if (nWords <= 0)
    {
        return -1;
    }
    if (wordnum < 0 || wordnum >= nWords)
    {
        return -1;
    } //Two error messages required by the spec
    int flowers = 0;
    int bees = 0;
    int attempts = 0; //Initialize some variables
    char mysteryWord[MAXWORDLENGTH+1]; //Create a c-string array to hold our mystery word
    char trialWord[100]; //User input array
    strcpy(mysteryWord, words[wordnum]); //Copy the word from the word list and place it into our array
    while (flowers != strlen(mysteryWord)) //Loop as long as the user hasn't found the word
    {
        flowers = 0;
        bees = 0; //Each iteration needs to reset the flower and bee count
        cout << "Trial word: ";
        cin.getline(trialWord, 100); //Take input
        if (!correctSyntax(trialWord))
        {
            cout << "Your trial word must be a word of 4 to 6 lower case letters." << "\n";
        }
        else if (!wordExists(words, trialWord, nWords))
        {
            cout << "I don't know that word." << "\n";
        }
        else
        {
            char mysteryWordCopy[MAXWORDLENGTH+1];
            strcpy(mysteryWordCopy, mysteryWord); //Create a modifiable copy of our mystery word
            for (int k = 0; k < strlen(mysteryWordCopy); k++)
            {
                for (int j = 0; j < strlen(trialWord); j++)
                {
                    if (trialWord[j] == mysteryWordCopy[k]) //Match found between both arrays
                    {
                        if (j == k)
                        {
                            flowers++;
                            trialWord[j] = '*';
                            mysteryWordCopy[k] = '*'; //This is my way of pairing flowers so bees won't check it
                        }
                        else
                        {
                            bees++;
                            trialWord[j] = '%'; //Changing the character so that it won't accidentally get read again
                        }
                    }
                }
            }
            strcpy(mysteryWordCopy, mysteryWord); //Reset the copy to original mystery word to get rid of any modifications for next iterations
            attempts++;
            if (flowers == strlen(mysteryWord))
            {
                break; //Once the user found the word, break out from while loop
            }
            cout << "Flowers: " << flowers << ", bees: " << bees << endl; //Tell user information about their guess, as required by the specs
        }
    }
    return attempts; //Return attempts the user needed to guess the word
}

bool correctSyntax(char trialWord[100])
{
    if (strlen(trialWord) < 4 || strlen(trialWord) > 6)
    {
        return false; //Not syntactically correct if the user word is out of the bounds of our mystery word (4 - 6 "interesting" characters, not including null character)
    }
    for (int k = 0; k < strlen(trialWord); k++)
    {
        if (!isalpha(trialWord[k]) || !islower(trialWord[k]))
        {
            return false; //Must be an alphabetical letter AND lowercase to be sytactically correct
        }
    }
    return true;
}

bool wordExists(const char words[][7], char trialWord[100], int nWords)
{
    for (int k = 0; k < nWords; k++)
    {
        if (strcmp(words[k], trialWord) == 0)
        {
            return true; //Check mystery word against every word in the list, if there is a match then it exists
        }
    }
    return false; //No matches found, cout the error message in playOneRound
}
