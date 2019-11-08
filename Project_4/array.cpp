#include <iostream>
#include <string>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);
//10 functions to implement

int main()
{
        string h[7] = { "mick", "marie", "fiona", "rudy", "", "gordon", "lindsey" };
        assert(lookup(h, 7, "gordon") == 5);
        assert(lookup(h, 7, "fiona") == 2);
        assert(lookup(h, 2, "fiona") == -1);
        assert(positionOfMax(h, 7) == 3);

        string g[4] = { "mick", "marie", "lindsey", "nancy" };
        assert(differ(h, 4, g, 4) == 2);
        assert(appendToAll(g, 4, "?") == 4 && g[0] == "mick?" && g[3] == "nancy?");
        assert(rotateLeft(g, 4, 1) == 1 && g[1] == "lindsey?" && g[3] == "marie?");

        string e[4] = { "fiona", "rudy", "", "gordon" };
        assert(subsequence(h, 7, e, 4) == 2);

        string d[5] = { "marie", "marie", "marie", "rudy", "rudy" };
        assert(countRuns(d, 5) == 2);
    
        string f[3] = { "lindsey", "fiona", "mike" };
        assert(lookupAny(h, 7, f, 3) == 2);
        assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "lindsey");
    
        assert(separate(h, 7, "lindsey") == 3);
    
        cout << "All tests succeeded" << endl;
}


int appendToAll(string a[], int n, string value)
{
    if (n >= 0) //(n is allowed to be 0) If n is greater than or equal to 0...
    {
        int k;
        for (k = 0; k < n; k++)
        {
            a[k] += value; //Add the desired string value to the existing string at each point in the array
        }
        return k; //This is the code that shows that the action was completed successfully
    }
    return -1; //If n is negative, report an error
}

int lookup(const string a[], int n, string target)
{
    if (n >=0)
    {
        if (n == 0)
        {
            return -1; //n cannot be 0 here, otherwise there is nothing we can use to look up with
        }
        else
        {
            int k;
            for (k = 0; k < n; k++)
            {
                if (a[k] == target) //If we find what we are looking for EXACTLY letter for letter...
                {
                    return k; //...return the position it was located at
                }
                else
                {
                    continue; //Otherwise, continue through the array
                }
            }
        }
    }
    return -1; //If n is negative, report an error
}

int positionOfMax(const string a[], int n)
{
    if (n >= 0)
    {
        if (n == 0)
        {
            return -1; //For this function, n can't be 0. Otherwise, no array to look up with
        }
        else
        {
            int positionWithGreatestStringValue = 0;
            string stringComparison = a[positionWithGreatestStringValue]; //Initializes the string with a[0]
            for (int k = 0; k < n; k++)
            {
                if (a[k] > stringComparison) //If the value of the point in the array is greater than the current greatest value...
                {
                    stringComparison = a[k];
                    positionWithGreatestStringValue = k; //...set that point as the greatest value and use it to compare to other string values. Note that it will not change if there is another position with an equal value down the line (>, not >=)
                }
            }
            return positionWithGreatestStringValue; //Return the value
        }
    }
    return -1; //If n is negative, report an error
}

int rotateLeft(string a[], int n, int pos)
{
    if (n >= 0)
    {
        if (n == 0)
        {
            return -1; //n cannot be 0 as we need an array to check
        }
        else if (pos < 0 || pos >= n)
        {
            return -1; //The position needs to be within the range of the array from 0 to n-1
        }
        else
        {
            string stringToBeMoved = a[pos]; //The string that will move to the end
            int originalPosition = pos; //The original position of the string to be moved
            int temp = 0; //A temporary position holder
            a[pos] = ""; //The string is now erased at this postion!
            while (pos < (n-1))
            {
                temp = pos; //The holder will take the position value now
                pos ++; //Move up one position
                a[temp] = a[pos]; //The previous string is now the string in front of it. The strings have shifted here
            }
            a[n-1] = stringToBeMoved; //We have moved the string to the end!
            return originalPosition; //Show that we have done so successfully
        }
    }
    return -1; //n can't be negative
}

int countRuns(const string a[], int n)
{
    if (n >= 0) //0 is allowed here, it'll just print out 0 since no repeats exist
    {
        int uniques = 0;
        int k = 0;
        while(k < n)
        {
            string stringCompare = a[k]; //Use the first string as a source to compare to
            k++; //Move up one position
            uniques++; //It exists, so it's been repeated once
            while (a[k] == stringCompare)
            {
                k++; //What happens here is that if the value at position k is equal to what we are comparing it to, then move up; it's part of the sequence
            }
        }
        return uniques; //Return the total unique values
    }
    return -1; //n can't be negative
}

int flip(string a[], int n)
{
    if (n >= 0) //0 inclusive, just returns 0
    {
        string stringHolder; //Holds the string values
        int middle = n/2; //Identify the midpoint
        for (int k = 0; k < middle; k++)
        {
            stringHolder = a[k]; //Have the holder take the value at a[k]
            a[k] = a[n - (k + 1)]; //a[k]'s new value is the last value of the range we want to flip
            a[n - (k + 1)] = stringHolder; //That last value is now the origiinal value of a[k]
        }
        return n; //If it works, return this value
    }
    return -1; //n can't be negative
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    if(n1 < 0 || n2 < 0)
    {
          return -1; //Neither values of the size of the arrays can be negative
    } //0 can be included, it'll return 0 however
      
    int sizeOfSmallerArray;
    if(n1 < n2) //Determine which is smaller by comparing sizes
    {
        sizeOfSmallerArray = n1;
    }
    else
    {
        sizeOfSmallerArray = n2;
    }
      
    int pos = 0;
    while(pos < sizeOfSmallerArray)
    {
        if(a1[pos] == a2[pos])
        {
            pos++; //If the values are the same, then move on; we need to find differences
        }
        else
        {
            return pos; //If they are different, return the position where they are different
        }
    }
    return pos; //Otherwise, return the position value where it stopped
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if(n2 == 0)
    {
        return 0; //If the second array size is 0, then return 0; why bother comparing them?
    }
    else if(n1 <= 0 || n2 < 0)
    {
        return -1; //n1 can't be 0, no array to compare to; n2 can't be negative
    }
    if(n2 > n1)
    {
        return -1; //We are comparing n2 to n1, so n2 needs to be smaller
    }
    
    int array1Pos = 0;
    int array2Pos = 0;
    
    int array2Repeats;
    int startingPosition;
    
    while(array1Pos < n1)
    {
        array2Repeats = 0;
        startingPosition = array1Pos; //Initialize the starting position at the start of the array
        
        while(a2[array2Pos] == a1[array1Pos])
        {
            array1Pos++;
            array2Pos++;
            array2Repeats++; //If the strings match up, up the counter for repeats, move the positions of the arrays up
            if(array2Repeats == n2)
                return startingPosition; //If everything matches up, then return the value where the matches began
        }
        array1Pos++; //Otherwise, move the starting point up one and loop back
    }
    return -1; //If no matches, return a failure
}



//Add comments to explain (and understand) code. Change some variable names while you're at it



int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    if(n1 == 0 && n2 == 0)
    {
        return 0; //In the case that both array sizes are 0, then return 0
    }
    else if(n1 <= 0 || n2 <= 0)
    {
        return -1; //However, if one size is specified and the other is 0 or negative, then we have a problem; nothing to look up with or nothing to search
    }
    
    int array1Pos = 0;
    while (array1Pos < n1) //This loop iterates through all of array 1 so that array 2 can check for matches one string at a time
    {
        int array2Pos = 0;
        while (array2Pos < n2) //This loop iterates through the array that looks up strings in array 1
        {
            if(a2[array2Pos] == a1[array1Pos])
            {
                return array1Pos; //When there is a match, return the position where it was made
            }
            else
            {
                array2Pos++; //Otherwise, see if there are any other matches
            }
        }
        array1Pos++; //If there are no matches for the array position, move up one
    }
    return -1; //If nothing matches, return the failure value
}

int separate(string a[], int n, string separator)
{
    if(n < 0)
    {
        return -1; //n cannot be negative
    }
    
    string tempArray[100] = {}; //Let's create an array with a large size, and set all values to blank
    int firstHalfOfArray = 0; //The first half of the array will start from the left
    int secondHalfOfArray = n - 1; //The second half will start from the right
    
    for(int k = 0; k < n; k++)
    {
        if(a[k] < separator)
        {
            tempArray[firstHalfOfArray] = a[k];
            firstHalfOfArray++; //If the original array's value is less than the separator's value, then put it before the separator (to the left) and move up one to the right
        }
        else if(a[k] > separator)
        {
            tempArray[secondHalfOfArray] = a[k];
            secondHalfOfArray--; //If the original array's value is greater than the separator's value, then put it after the separator (to the right) and move down one to the left
        }
        else if(a[k] == separator)
        {
            tempArray[firstHalfOfArray] = separator;
            firstHalfOfArray++; //If the original array's value is equal to the separator's value, then have that position be equal to the separator
        }
    }
    
    for(int k = 0; k < n; k++)
    {
        a[k] = tempArray[k]; //Once the sorting is complete, change the original array
    }
    
    for(int k = 0; k < n; k++)
    {
        if(a[k] >= separator)
        {
            return k; //If the value is not less than the separator, then return that value
        }
    }
    return n; //If there are no such values, return n
}

