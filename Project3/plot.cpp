#include "grid.h"
#include <iostream>
#include <string>
#include <cassert>
#include <cctype>
#include <vector>
using namespace std;

const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;
//Global constants required

bool isValidStartingPoint(int r, int c);
bool isWithinBounds(int r, int c, int distance, int dir);
bool isForeGround(int fgbg);
bool isBackGround(int fgbg);
bool isHorizontal(int dir);
bool isVertical(int dir);
bool isCharacterToReplace(char ch);
void drawVerticalLine(int r, int c, int distance, char ch, int fgbg);
void drawHorizontalLine(int r, int c, int distance, char ch, int fgbg);
//The helper functions above assist the execution of bool plotLine(...)

bool isH(char commandStringArray);
bool isV(char commandStringArray);
bool isC(char commandStringArray);
bool isB(char commandStringArray);
bool isF(char commandStringArray);
bool isCommand(char commandStringArray);
bool syntaxOopsie(vector<string> parsedCommands, int& badPos);
bool logicOopsie(vector<string> parsedCommands, int& badPos);
vector<string> parseCommandString (string commandString); //The only way I could parse is to use an array/vector
//The helper functions above assist int performCommands

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
//Two main functions required to implement as per the specs

int main()
{
    setSize(20, 30);
     clearGrid();
       char currentChar = '*';
       int currentMode = FG;
       for (;;)
       {
           cout << "Enter a command string: ";
           string cmd;
           getline(cin, cmd);
           if (cmd == "")
               break;
       int position;
       int status = performCommands(cmd, currentChar, currentMode, position);
       switch (status)
       {
         case 0:
           draw();
           break;
         case 1:
               cout << "Syntax error at position " << position+1 << endl;
           break;
         case 2:
               cout << "Cannot perform command at position " << position+1 << endl;
           break;
         default:
             // It should be impossible to get here.
               cerr << "performCommands returned " << status << "!" << endl;
       }
    }
}

bool isValidStartingPoint(int r, int c)
{
    if (r > 0 && r < getRows()) //Makes sure the starting point for rows is within 1 & 20
    {
        if (c > 0 && c < getCols()) //Makes sure the starting point for columns is within 1 & 30
        {
            return true;
        }
    }
    return false; //Otherwise, not in bounds
}

bool isWithinBounds(int r, int c, int distance, int dir)
{
    if (dir == HORIZ)
    {
        if ((c+distance) >= 0 && (c+distance <= getCols()))
        {
            return true; //Checks if our line is within the grid horizontally
        }
    }
    else if (dir == VERT)
    {
        if ((r+distance) >= 0 && (r+distance) <= getRows())
        {
            return true; //Checks if our line is within the grid vertically
        }
    }
    return false; //Either (somehow) not HORIZ or VERT, or not in bounds
}

bool isForeGround(int fgbg)
{
    if (fgbg == FG) //Checks to see if it is foreground
    {
        return true;
    }
    return false; //Not foreground
}

bool isBackGround(int fgbg)
{
    if (fgbg == BG) //Checks to see if it is background
    {
        return true;
    }
    return false; //Not background
}

bool isHorizontal(int dir)
{
    if (dir == HORIZ)
    {
        return true; //If the user input command to draw horiz, this is true
    }
    return false; //Otherwise, it is false
}

bool isVertical(int dir)
{
    if (dir == VERT)
    {
        return true; //If the user input command to draw vert, this is true
    }
    return false; //Otherwise, it is false
}

bool isCharacterToReplace(char ch)
{
    if (isprint(ch))
    {
        return true; //If we can print it, then true
    }
    return false; //Otherwise, we cannot print it
}

void drawVerticalLine(int r, int c, int distance, char ch, int fgbg)
{
    if (isForeGround(fgbg)) //First, let's check if the current mode is foreground
    {
         if (distance > 0) //If distance is positive...
           {
               for (int k = 0; k <= abs(distance); k++)
                   //Interesting point I'd like to make here: I originally made this line without the
                   //'=' sign after the '<'. It worked the way I THOUGHT it should. However, when I
                   //ran the test case with h12V3H-1B@v-3, it didn't look EXACTLY like in the spec.
                   //Once adding the = sign, the code ran "as it should." I still don't understand why
                   //I should've included the = sign since when inputting the size of the line, it should
                   //only print out that many characters (h12 prints only 12 *'s). Weird... oh well!
                   //Smallberg does what Smallberg do! #praisesmallberg
               {
                   setChar((r+k), c, ch); //...then print out a vertical line downwards
               }
           }
           else if (distance == 0) //If distance is 0...
           {
               setChar(r, c, ch); //...then just print out the character at (r,c)
           }
           else if (distance < 0) //If the distance is negative...
           {
               for (int k = 0; k <= abs(distance); k++)
               {
                   setChar((r-k), c, ch); //...then print a vertical line upwards
               }
           }
    }
    else if (isBackGround(fgbg)) //If it wasn't in the foreground, then it should be in background mode
    {
        if (distance > 0) //If distance is positive...
           {
               for (int k = 0; k <= abs(distance); k++)
               {
                   if (getChar((r+k), c) == ' ') //...AND if the current position is blank...
                   {
                       setChar((r+k), c, ch); //...then draw the character
                   }
                   else
                   {
                       continue; //Otherwise, skip the current position and move on
                   }
               }
           }
           else if (distance == 0) //If distance is 0...
           {
               if (getChar(r, c) == ' ') //...AND if the current position is blank...
               {
                   setChar(r, c, ch); //...then place a char down
               }
               else //Otherwise, don't do anything
               {
               }
           }
           else if (distance < 0) //If distance is negative...
           {
               for (int k = 0; k <= abs(distance); k++)
               {
                   if (getChar((r-k), c) == ' ') //...AND the current position is blank...
                   {
                       setChar((r-k), c, ch); //...then draw the character
                   }
                   else
                   {
                       continue; //Otherwise, skip the current position and move on
                   }
               }
           }
    }
}

void drawHorizontalLine(int r, int c, int distance, char ch, int fgbg)
{
    //The lines of code below serve the exact same purposes as drawVerticalLine, except it's for horizontal lines
    if (isForeGround(fgbg))
    {
         if (distance > 0)
           {
               for (int k = 0; k <= abs(distance); k++)
               {
                   setChar(r, (c+k), ch);
               }
           }
           else if (distance == 0)
           {
               setChar(r, c, ch);
           }
           else if (distance < 0)
           {
               for (int k = 0; k <= abs(distance); k++)
               {
                   setChar(r, (c-k), ch);
               }
           }
    }
    else if (isBackGround(fgbg))
    {
        if (distance > 0)
        {
            for (int k = 0; k <= abs(distance); k++)
            {
                if (getChar(r, (c+k)) == ' ')
                {
                    setChar(r, (c+k), ch);
                }
                else
                {
                    continue;
                }
            }
        }
        else if (distance == 0)
        {
            if (getChar(r, c) == ' ')
            {
                setChar(r, c, ch);
            }
        }
        else if (distance < 0)
        {
            for (int k = 0; k <= abs(distance); k++)
            {
                if (getChar(r, (c-k)) == ' ')
                {
                    setChar(r, (c-k), ch);
                }
                else
                {
                    continue;
                }
            }
        }
    }
}

bool plotLine(int r, int c, int distance, int dir, char ch, int fgbg)
{
    if (isValidStartingPoint(r, c)) //Let's first check if it is a valid start position
    {
        if (isWithinBounds(r, c, distance, dir)) //Then, check if it's within the grid
        {
            if (isCharacterToReplace(ch)) //Next, make sure we can print the character
            {
                if (isForeGround(fgbg) || isBackGround(fgbg)) //Check if fgbg is a valid input
                {
                    if (isHorizontal(dir)) //If we are drawing a horizontal line...
                    {
                        drawHorizontalLine(r, c, distance, ch, fgbg); //...then draw it...
                        return true; //...and show that it was possible
                    }
                    else if (isVertical(dir)) //If we are drawing a vertical line...
                    {
                        drawVerticalLine(r, c, distance, ch, fgbg);//...then draw it...
                        return true; //...and show that it succeeded in doing so
                    }
                }
                else
                    return false; //Invalid mode input
            }
            else
                return false; //The character can't be printed
        }
        else
            return false; //The line was not in bounds
    }
    return false; //We can't start here
}

bool isH(char commandStringArray) //Checks if the character is an H
{
    switch (commandStringArray)
    {
        case 'h':
        case 'H':
            return true;
            break;
            
        default:
            return false;
            break;
    }
}
bool isV(char commandStringArray) //Checks if the character is a V
{
    switch (commandStringArray)
    {
        case 'v':
        case 'V':
            return true;
            break;
            
        default:
            return false;
            break;
    }
}
bool isC(char commandStringArray) //Checks if the character is a C
{
    switch (commandStringArray)
    {
        case 'c':
        case 'C':
            return true;
            break;
            
        default:
            return false;
            break;
    }
}
bool isB(char commandStringArray) //Checks if the character is a B
{
    switch (commandStringArray)
    {
        case 'b':
        case 'B':
            return true;
            break;
            
        default:
            return false;
            break;
    }
}
bool isF(char commandStringArray) //Checks if the character is an F
{
    switch (commandStringArray)
    {
        case 'f':
        case 'F':
            return true;
            break;
            
        default:
            return false;
            break;
    }
}
bool isCommand(char commandStringArray) //Checks the string if the character is a command
{
    switch (commandStringArray)
    {
        case 'f':
        case 'F':
        case 'h':
        case 'H':
        case 'v':
        case 'V':
        case 'b':
        case 'B':
        case 'c':
        case 'C':
            return true;
            break;
        default:
            return false;
            break;
    }
}

vector<string> parseCommandString (string commandStringArray) //This will cut our string into executable snipets
{
    string commandHolder = ""; //The temporary holder must be empty each time it is called
    vector<string> fullyParsedCommand; //This is where we store our snipets
    for (int i = 0; i < commandStringArray.size(); i++) {
        if (isF(commandStringArray[i]) || isB(commandStringArray[i])) //If it's a mode change...
        {
            commandHolder = "";
            commandHolder += commandStringArray[i];
            i++;
            commandHolder += commandStringArray[i]; //...include the letter and character after (we'll check syntax later)
            fullyParsedCommand.push_back(commandHolder); //Insert the snipet into what will be read
            commandHolder = ""; //Clear the holder each time after performing this action
        }
        else if (i == (commandStringArray.size()-1) || isCommand(commandStringArray[i+1]))
        { //Once we reach the end of the string or executable command snipet...
            commandHolder += commandStringArray[i];
            fullyParsedCommand.push_back(commandHolder);
            commandHolder = ""; //...put whatever we are currently at into the parsed command and prepare to read another one
        }
        else if (isCommand(commandStringArray[i]))
        {
            commandHolder = "";
            commandHolder += commandStringArray[i]; //Begin a new parsed command, the else at the bottom will take care of numbers and negatives
        }
        else //Probably a number or negative sign
        {
            commandHolder += commandStringArray[i]; //Add to the holder
        }
    }
    return fullyParsedCommand; //Don't forget to return the parsed command
}

bool syntaxOopsie(vector<string> parsedCommands, int& badPos) //Check syntax errors
{
    int currentIndex = 0; //Where we are in the command, starting at the first letter
    for (int i = 0; i < parsedCommands.size(); i++)
    {
        string commandAnalyzer = parsedCommands[i]; //This will help us analyze each character in the parsed command
        if (!isCommand(commandAnalyzer[0])) //If it doesn't start with a command letter...
        {
            badPos = currentIndex;
            return true; //State that there is an error at position i
        }
        else if (isH(commandAnalyzer[0]) || isV(commandAnalyzer[0])) //If it starts with H or V...
        { //Check the size of the parsed command, and check it against errors. Otherwise, no error
            if (commandAnalyzer.size() == 1)
            {
                currentIndex += 1;
                badPos = currentIndex;
                return true;
            }
            else if (commandAnalyzer.size() == 2)
            {
                if (!isdigit(commandAnalyzer[1]))
                {
                    currentIndex += 1;
                    badPos = currentIndex;
                    return true;
                }
            }
            else if (commandAnalyzer.size() == 3)
            {
                if (!isdigit(commandAnalyzer[1]) && commandAnalyzer[1] != '-')
                {
                    currentIndex += 1;
                    badPos = currentIndex;
                    return true;
                }
                else if (!isdigit(commandAnalyzer[2]))
                {
                    currentIndex += 2;
                    badPos = currentIndex;
                    return true;
                }
            }
            else if (commandAnalyzer.size() == 4)
            {
                if (commandAnalyzer[1] != '-')
                {
                    currentIndex += 1;
                    badPos = currentIndex;
                    return true;
                }
                else if (!isdigit(commandAnalyzer[2]))
                {
                    currentIndex += 2;
                    badPos = currentIndex;
                    return true;
                }
                else if (!isdigit(commandAnalyzer[3]))
                {
                    currentIndex += 3;
                    badPos = currentIndex;
                    return true;
                }
            }
            else if (commandAnalyzer.size() > 4)
            {
                if (commandAnalyzer[1] != '-')
                {
                    currentIndex += 1;
                    badPos = currentIndex;
                    return true;
                }
                else if (!isdigit(commandAnalyzer[2]))
                {
                    currentIndex += 2;
                    badPos = currentIndex;
                    return true;
                }
                else if (!isdigit(commandAnalyzer[3]))
                {
                    currentIndex += 3;
                    badPos = currentIndex;
                    return true;
                }
                else
                {
                    currentIndex += 4;
                    badPos = currentIndex;
                    return true;
                }
            }
        }
        else if (isC(commandAnalyzer[0])) //If the command is a C...
        {
            if (commandAnalyzer.size() > 1) //...and if there is anything after that...
            {
                currentIndex += 1;
                badPos = currentIndex;
                return true; //...there is clearly an error
            }
        }
        else if (isF(commandAnalyzer[0]) || isB(commandAnalyzer[0])) //If the command is an F or B...
        {
            if (!isCharacterToReplace(commandAnalyzer[1])) //...and if the next value isn't a character we can replace...
            {
                currentIndex += 1;
                badPos = currentIndex;
                return true; //...clearly an error
            }
        }
        currentIndex += commandAnalyzer.size();
    }
    return false;
}

bool logicOopsie(vector<string> parsedCommands, int& badPos) //This function will see if the lines go out of bounds and report errors if it does happen
{
    int currentIndex = 0;
    int r = 1;
    int c = 1;
    int distance = 0;
    int dir = 0;
    
    for (int i = 0; i < parsedCommands.size(); i++)
    {
        string commandAnalyzer = parsedCommands[i];
        distance = 0;
        if (isF(commandAnalyzer[0]))
        {
            currentIndex += 2; //Move over two spaces since F commands take up two positions total
        }
        else if (isB(commandAnalyzer[0]))
        {
            currentIndex += 2; //Move over two spaces since B commands take up two positions total
        }
        else if (isC(commandAnalyzer[0]))
        {
            currentIndex += 1; //Move over one space since C commands take up one position
        }
        else if (isH(commandAnalyzer[0]))
        {
            dir = HORIZ;
            if (commandAnalyzer.size() == 1)
            {
                distance = 0;
            }
            else if (commandAnalyzer.size() == 2)
            {
                distance = commandAnalyzer[1] - 48; //ASCII WILL MESS THIS UP, - 48 IS REQUIRED
            }
            else if (commandAnalyzer.size() == 3)
            {
                if (commandAnalyzer[1] == '-')
                {
                    distance = (-1)*(commandAnalyzer[2] - 48);
                }
                else
                {
                    distance = (10*(commandAnalyzer[1] - 48)) + (commandAnalyzer[2] - 48);
                }
            }
            else if (commandAnalyzer.size() == 4)
            {
                distance = (-1)*((10*(commandAnalyzer[2] - 48)) + (commandAnalyzer[3] - 48));
            }
            if (abs(c + distance) > abs(getCols()))
            {
                badPos = currentIndex;
                return true;
            }
            currentIndex += commandAnalyzer.size();
        }
        else if (isV(commandAnalyzer[0]))
        {
            dir = VERT;
            if (commandAnalyzer.size() == 1)
            {
                distance = 0;
            }
            else if (commandAnalyzer.size() == 2)
            {
                distance = commandAnalyzer[1] - 48; //ASCII WILL MESS THIS UP, - 48 IS REQUIRED
            }
            else if (commandAnalyzer.size() == 3)
            {
                if (commandAnalyzer[1] == '-')
                {
                    distance = -1*(commandAnalyzer[2] - 48);
                }
                else
                {
                    distance = 10*(commandAnalyzer[1] - 48) + (commandAnalyzer[2] - 48);
                }
            }
            else if (commandAnalyzer.size() == 4)
            {
                distance = -1*(10*(commandAnalyzer[2] - 48) + (commandAnalyzer[3] - 48));
            }
            if (abs(r + distance) > abs(getRows()))
            {
                badPos = currentIndex;
                return true;
            }
            currentIndex += commandAnalyzer.size();
        }
        if (dir == HORIZ)
        {
            c += distance;
        }
        else if (dir == VERT)
        {
            r += distance;
        }
    }
    return false;
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) //The Big Kahuna. The Motherload. The Boss. The Head Honcho behind this program. It will call the function to parse commands, check syntax and logic, and call plotline when everything is all set. The cumulation of Project 3
{
    int r = 1;
    int c = 1;
    int distance = 0;
    int dir = 0;
    vector<string> commandStringParsed = parseCommandString(commandString);
    if (syntaxOopsie(commandStringParsed, badPos))
    {
        return 1;
    }
    else if (logicOopsie(commandStringParsed, badPos))
    {
        return 2;
    }
    else
    {
        for (int i = 0; i < commandStringParsed.size(); i++)
        {
            distance = 0;
            string actions = commandStringParsed[i];
            if (isF(actions[0]))
            {
                mode = FG;
                if (isCharacterToReplace(actions[1]))
                {
                    plotChar = actions[1];
                }
            }
            else if (isB(actions[0]))
            {
                mode = BG;
                if (isCharacterToReplace(actions[1]))
                {
                    plotChar = actions[1];
                }
            }
            else if (isC(actions[0]))
            {
                clearGrid();
                r = 1;
                c = 1;
                mode = FG;
                plotChar = '*';
            }
            else if (isH(actions[0]))
            {
                dir = HORIZ;
                if (actions.size() == 1)
                {
                    distance = 0;
                }
                else if (actions.size() == 2)
                {
                    distance = actions[1] - 48; //ASCII WILL MESS THIS UP, - 48 IS REQUIRED
                }
                else if (actions.size() == 3)
                {
                    if (actions[1] == '-')
                    {
                        distance = (-1)*(actions[2] - 48);
                    }
                    else
                    {
                        distance = 10*(actions[1] - 48) + (actions[2] - 48);
                    }
                }
                else if (actions.size() == 4)
                {
                    distance = (-1)*(10*(actions[2] - 48) + (actions[3] - 48));
                }
                plotLine(r, c, distance, dir, plotChar, mode);
            }
            else if (isV(actions[0]))
            {
                dir = VERT;
                if (actions.size() == 1)
                {
                    distance = 0;
                }
                else if (actions.size() == 2)
                {
                    distance = actions[1] - 48; //ASCII WILL MESS THIS UP, - 48 IS REQUIRED
                }
                else if (actions.size() == 3)
                {
                    if (actions[1] == '-')
                    {
                        distance = (-1)*(actions[2] - 48);
                    }
                    else
                    {
                        distance = 10*(actions[1] - 48) + (actions[2] - 48);
                    }
                }
                else if (actions.size() == 4)
                {
                    distance = (-1)*(10*(actions[2] - 48) + (actions[3] - 48));
                }
                plotLine(r, c, distance, dir, plotChar, mode);
            }
            if (dir == HORIZ)
            {
                c += distance;
            }
            else if (dir == VERT)
            {
                r += distance;
            }
        
        }
        return 0;
    }
}

