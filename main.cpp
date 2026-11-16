#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
// Function prototypes
char shiftChar(char c, int shift);
int randomNo(int min, int max);
bool validateInput(const string &input);

int main()
{
    srand(time(0)); // Seed the random number generator
    return 0;
}

// Function to validate input string (only alphabetic characters and spaces are allowed)
bool validateInput(const string &input)
{
    for (char c : input)
    {
        if (!isalpha(c) && !isspace(c))
        {
            return false; // Invalid character found
        }
        if (islower(c))
        {
            c = toupper(c); // Convert to uppercase for uniformity
        }
    }
    return true; // All characters are valid
}

// Function to shift a character by a given number of positions in the alphabet
char shiftChar(char c, int shift)
{
    if (isalpha(c))
    {
        char base = islower(c) ? 'a' : 'A';
        return ((c - base + shift) % 26) + base;
    }
    return c; // Non-alphabetic characters are not shifted
}

// Function to generate a random number between min and max (inclusive)
int randomNo(int min, int max)
{
    return rand() % (max - min + 1) + min;
}