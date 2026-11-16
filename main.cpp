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
bool startLevel1(uint16_t &levelScore);
bool startLevel2(uint16_t &levelScore);
bool startLevel3(uint16_t &levelScore);

int main()
{
    srand(time(0));     // Seed the random number generator
    uint8_t level = 1;  // Initialize level to 1, range 1-3
    uint16_t score = 0; // Initialize score to 0, range 1-10

    // simulate game loop for levels 1 to 3
    while (level <= 3)
    {
        bool levelComplete = false;
        uint16_t levelScore = 0; // Initialize level score to 0
        switch (level)
        {
        case 1:
            levelComplete = startLevel1(levelScore);
            break;
        case 2:
            levelComplete = startLevel2(levelScore);
            break;
        case 3:
            levelComplete = startLevel3(levelScore);
            break;
        }
        if (levelComplete)
        {
            score += 1; // Increment score for completing a level
            level++;    // Move to the next level
        }
        else
        {
            break; // If a level is not completed, stop the game
        }
    }
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

