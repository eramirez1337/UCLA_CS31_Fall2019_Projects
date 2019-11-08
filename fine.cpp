#include <iostream>
using namespace std;

int main()
{
    string defendant;
    int amountPaid;
    char fakeOrNot;
    //Defining variables
    
    cout << "Defendant: ";
    getline(cin, defendant);
    
    cout << "Amount paid (in thousands): ";
    cin >> amountPaid;
    
    cout << "Fake athlete? (y/n): ";
    cin >> fakeOrNot;
    
    cout << "---" << endl;
    //Receiving all inputs before checking for errors. Also placed the three hyphens for final output
    
    if (defendant == "")
    {
        cout << "You must enter a defendant name.";
        return 0;
    }
    //Terminates program if defendant name is empty
    
    if (amountPaid < 0)
    {
        cout << "The amount paid must not be negative.";
        return 0;
    }
    //Terminates program if amount paid is negative
    
    if (!(fakeOrNot == 'y' || fakeOrNot == 'n'))
    {
        cout << "You must enter y or n.";
        return 0;
    }
    //Terminates program if the athlete credentials are not verified via y or n

    double finesDue;
    
    if (amountPaid > 40)
    {
        if (fakeOrNot == 'n')
        {
            if (amountPaid > 250)
            {
                finesDue = 20 + (40*0.66) + (210*0.10) + 0.14*(amountPaid-250);
            } //This fine is paid when the amount paid is above $250k and the athlete is legitimate
            else
            {
                finesDue = 20 + (40*0.66) + 0.10*(amountPaid-40);
            } //This fine is paid when the amount paid is less than $250k but greater than $40k, and the athlete is still legitimate

        }
        else
        {
            if (amountPaid > 250)
            {
                finesDue = 20 + (40*0.66) + (210*0.22) + 0.14*(amountPaid-250);
            } //This fine is paid when the athlete is not legitimate and paid over $250k
            else
            {
                finesDue = 20 + (40*0.66) + 0.22*(amountPaid-40);
            } //This fine is paid when the illegitimate athlete paid less than $250k but greater than $40k
        }
    }
    else
    {
        finesDue = 20 + 0.66*(amountPaid);
    } //This fine is paid by the athlete (regardless of legitimacy) if he/she paid less than $40k
    
    cout.setf(ios::fixed);
    cout.precision(1);
    //Making sure that the fines due show only 1 value after the decimal point
    
    cout << "The suggested fine for " << defendant << " is $" << finesDue << " thousand." << endl;
}
