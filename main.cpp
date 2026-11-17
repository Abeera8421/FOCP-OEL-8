#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <cstdint>
#include "words.hpp"

using namespace std;
// Function prototypes
char shiftChar(char c, int shift);
int randomNo(int min, int max);
bool validateInput(const string &input);
void createDashes(const string &word, vector<bool> &dashes);
void printLevel(const string &word, const vector<bool> &dashes, const string &shiftedWord, uint16_t level);
void takeStringInput(string &input);
void encrypt2(string input);
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

void createDashes(const string &word, vector<bool> &dashes)
{
    uint16_t wordLength = word.length();
    for (size_t i = 0; i < wordLength; ++i)
    {
        dashes[i] = false; // Initialize all dashes to false (hidden)
    }
    uint16_t numDashesIndexes = wordLength / 2; // Number of dashes to create (half the word length)
    for (size_t i = 0; i <= numDashesIndexes; i += 2)
    {
        dashes[i] = true; // Set the alternate indexes to true (hidden)
    }
}

void printLevel(const string &word, const vector<bool> &dashes, const string &shiftedWord, uint16_t level)
{
    cout << "Current level: " << level << endl;
    cout << "Word length: " << word.length() << endl;
    cout << "Word: ";
    for (size_t i = 0; i < word.length(); ++i)
    {
        if (dashes[i])
        {
            cout << "_ "; // Print underscore for hidden letters
        }
        else
        {
            cout << word[i] << " "; // Print the actual letter
        }
    }
    cout << "Encrypted word: ";
    cout << shiftedWord << endl;
    cout << endl; // New line after printing the level
}

void takeStringInput(string &input)
{
    cout << "Enter a string: ";
    cin.clear();
    getline(cin, input);
}

void encrypt2(string input)
{
    uint8_t randomShiftEven = randomNo(1, 25); // Random shift for even indexed characters
    uint8_t randomShiftOdd = randomNo(1, 25);  // Random shift for odd indexed characters
    for (size_t i = 0; i < input.length(); ++i)
    {
        if (i % 2 == 0)
        {
            input[i] = shiftChar(input[i], randomShiftEven); // Shift even indexed characters
        }
        else
        {
            input[i] = shiftChar(input[i], randomShiftOdd); // Shift odd indexed characters
        }
    }
}

bool startLevel1(uint16_t &levelScore)
{
    string input = "";
    size_t attempts = 0;                               // Initialize attempts to 0
    const string &randomWord = words1[randomNo(0, 9)]; // Random word selection from words1
    const int wordLength = randomWord.length();
    vector<bool> dashes(wordLength); // Array for dashes to represent the hidden word
    createDashes(randomWord, dashes);
    uint8_t randomShift = randomNo(1, 25); // Random shift between 1 and 25
    string shiftedWord = randomWord;       // Create a copy of the random word to shift
    for (char &c : shiftedWord)
    {
        c = shiftChar(c, randomShift); // Shift each character in the word
    }
    cout << "Starting Level 1..." << endl;
    printLevel(randomWord, dashes, shiftedWord, 1); // Print the initial state of the level
    while (input != randomWord && attempts < 5)     // Allow up to 3 attempts
    {
        takeStringInput(input);
        if (!validateInput(input))
        {
            cout << "Invalid input. Please enter only alphabetic characters and spaces." << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (input == randomWord)
        {
            cout << "Congratulations! You've guessed the word correctly." << endl;
            break; // Exit the loop if the word is guessed correctly
        }
        else
        {
            cout << "Incorrect guess. Try again." << endl;
            attempts++; // Increment attempts for incorrect guesses
        }
    }
    levelScore += 3; // Random score for level 1
    cout << "Level 1 completed with score: " << levelScore << endl;
    return true; // Indicate level completion
}

bool startLevel2(uint16_t &levelScore)
{
    cout << "Starting Level 2..." << endl;
    string input = "";
    size_t attempts = 0;                               // Initialize attempts to 0
    const string &randomWord = words2[randomNo(0, 9)]; // Random word selection from words2
    const int wordLength = randomWord.length();
    vector<bool> dashes(wordLength); // Array for dashes to represent the hidden word
    createDashes(randomWord, dashes);
    string shiftedWord = randomWord;       // Create a copy of the random word to shift
    encrypt2(shiftedWord);      // Create a copy of the random word to shift
    printLevel(randomWord, dashes, shiftedWord, 2); // Print the initial state of the level
    while (input != randomWord && attempts < 5)     // Allow up to 3 attempts
    {
        takeStringInput(input);
        if (!validateInput(input))
        {
            cout << "Invalid input. Please enter only alphabetic characters and spaces." << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (input == randomWord)
        {
            cout << "Congratulations! You've guessed the word correctly." << endl;
            break; // Exit the loop if the word is guessed correctly
        }
        else
        {
            cout << "Incorrect guess. Try again." << endl;
            attempts++; // Increment attempts for incorrect guesses
        }
    }
    levelScore += 4; // Random score for level 2
    cout << "Level 2 completed with score: " << levelScore << endl;
    return true; // Indicate level completion
}

bool startLevel3(uint16_t &levelScore)
{
    cout << "Starting Level 3..." << endl;
    // Level 3 logic here
    // For demonstration, we will just simulate a successful level completion
    levelScore = randomNo(1, 10); // Random score for level 3
    cout << "Level 3 completed with score: " << levelScore << endl;
    return true; // Indicate level completion
}