#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <cstdint>
#include "words.hpp"
#include <windows.h>

using namespace std;
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";
// Function prototypes
char shiftChar(char c, int shift);
int randomNo(int min, int max);
bool validateInput(const string &input);
void createDashes(const string &word, vector<bool> &dashes);
void printLevel(const string &word, const vector<bool> &dashes, const string &shiftedWord, uint16_t level);
void takeStringInput(string &input);
void vigenereEncrypt(string &input, const string &key);
void encrypt2(string &input);
void XOREncrypt(string &input, const string &key);
void toHex(const string &input, string &hexOutput);
void toChar(const string &hexInput, string &charOutput);
bool startLevel1(uint16_t &levelScore);
bool startLevel2(uint16_t &levelScore);
bool startLevel3(uint16_t &levelScore);
bool startLevel4(uint16_t &levelScore);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));     // Seed the random number generator
    uint8_t level = 1;  // Initialize level to 1, range 1-3
    uint16_t score = 0; // Initialize score to 0, range 1-10

    // simulate game loop for levels 1 to 3
    while (level <= 4)
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
        case 4:
            levelComplete = startLevel4(levelScore);
            break;
        default:
            cout << "Invalid level." << endl;
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
    for (size_t i = 0; i < wordLength; i += 2)
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
            cout << YELLOW << "_ " << RESET; // Print underscore for hidden letters
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

void revealHint(const string &word, vector<bool> &dashes)
{
    for (size_t i = 0; i < word.length(); i++)
    {
        if (dashes[i])
        {
            dashes[i] = false;

            cout << "Hint: The letter at position "
                 << i + 1 << " is " << word[i] << endl;

            return;
        }
    }

    cout << "No hidden letters left to reveal." << endl;
}
void takeStringInput(string &input, bool allowHint = false)
{
    if (allowHint)
        cout << "Enter your guess (or type HINT for a clue): ";
    else
        cout << "Enter your guess: ";

    cin.clear();
    getline(cin, input);
}
void encrypt2(string &input)
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

void vigenereEncrypt(string &input, const string &key)
{
    size_t keyLength = key.length();
    for (size_t i = 0; i < input.length(); ++i)
    {
        char keyChar = key[i % keyLength];     // Repeat the key if input is longer than the key
        int shift = toupper(keyChar) - 'A';    // Calculate shift based on the key character
        input[i] = shiftChar(input[i], shift); // Shift the input character
    }
}

void XOREncrypt(string &input, const string &key)
{
    size_t keyLength = key.length();
    for (size_t i = 0; i < input.length(); ++i)
    {
        input[i] ^= key[i % keyLength]; // XOR each character with the corresponding character in the key
    }
}


void toHex(const string &input, string &hexOutput)
{
    const string hexDigits = "0123456789ABCDEF";

    hexOutput.clear();

    for (unsigned char byte : input)
    {
        hexOutput += hexDigits[(byte >> 4) & 0x0F];
        hexOutput += hexDigits[byte & 0x0F];
    }
}

void toChar(const string &hexInput, string &charOutput)
{
    auto hexVal = [](char c) -> uint8_t
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        return 0;
    };
    charOutput.clear(); // Clear the output string before appending
    for (size_t i = 0; i < hexInput.length(); i += 2)
    {
        uint8_t highNibble = hexVal(hexInput[i]);     // Get the high nibble (4 bits)
        uint8_t lowNibble = hexVal(hexInput[i + 1]);  // Get the low nibble (4 bits)
        uint8_t byte = (highNibble << 4) | lowNibble;
        charOutput += byte;                                                     // Append the character to the output string
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
        takeStringInput(input, false);
        if (!validateInput(input))
        {
            cout << "Invalid input. Please enter only alphabetic characters and spaces." << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (input == randomWord)
        {
           cout <<GREEN<< "Congratulations! You've guessed the word correctly." << RESET <<endl;
            break; // Exit the loop if the word is guessed correctly
        }
        else
        {
           cout << RED << "Incorrect guess!Try Again" << RESET << endl;
            attempts++; // Increment attempts for incorrect guesses
        }
    }
    levelScore += 3; // Random score for level 1
    cout << GREEN << "Level completed! with score: "<<levelScore << RESET << endl;
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
    string shiftedWord = randomWord;                // Create a copy of the random word to shift
    encrypt2(shiftedWord);                          // Create a copy of the random word to shift
    printLevel(randomWord, dashes, shiftedWord, 2); // Print the initial state of the level
    while (input != randomWord && attempts < 5)     // Allow up to 3 attempts
    {
       takeStringInput(input, false);
        if (!validateInput(input))
        {
            cout << "Invalid input. Please enter only alphabetic characters and spaces." << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (input == randomWord)
        {
            cout <<GREEN<< "Congratulations! You've guessed the word correctly." << RESET <<endl;
            break; // Exit the loop if the word is guessed correctly
        }
        else
        {
            cout << RED << "Incorrect guess!Try Again" << RESET << endl;
            attempts++; // Increment attempts for incorrect guesses
        }
    }
    levelScore += 4; // Random score for level 2
    cout <<GREEN<< "Level 2 completed with score: " << levelScore <<RESET<< endl;
    return true; // Indicate level completion
}

bool startLevel3(uint16_t &levelScore)
{
    cout << endl;
    cout << "Starting Level 3..." << endl;
    size_t attempts = 0;                               // Initialize attempts to 0
    size_t hintsUsed = 0;
    const size_t maxHints = 2;
    const string &randomWord = words3[randomNo(0, 9)]; // Random word selection from words3
    const int wordLength = randomWord.length();
    vector<bool> dashes(wordLength); // Array for dashes to represent the hidden word
    createDashes(randomWord, dashes);
    string shiftedWord = randomWord;                // Create a copy of the random word to shift
    string key = keys[randomNo(0, 9)];              // Random key selection from keys
    XOREncrypt(shiftedWord, key);                   // Encrypt the shifted word using XOR
    // Convert encrypted bytes into readable hexadecimal
    string hexOutput;
    toHex(shiftedWord, hexOutput);
    printLevel(randomWord, dashes, hexOutput, 3); // Print the initial state of the level
    string input = "";
    while (input != randomWord && attempts < 5) // Allow up to 3 attempts
    {
        takeStringInput(input, true);
        if (input == "HINT" || input == "hint" || input == "Hint")
{
    if (hintsUsed < maxHints)
    {
        revealHint(randomWord, dashes);
        hintsUsed++;
    }
    else
    {
        cout << RED << "⚠ No hints remaining!" << RESET << endl;
    }

    printLevel(randomWord, dashes, hexOutput, 3);
    input = "";
    continue;
}
        if (!validateInput(input))
        {
            cout << YELLOW << "⚠ Invalid input. Please enter only alphabetic characters and spaces." << RESET << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (input == randomWord)
        {
            cout <<GREEN<< "Congratulations! You've guessed the word correctly." << RESET <<endl;
            break; // Exit the loop if the word is guessed correctly
        }
        else
        {
            cout << RED << "Incorrect guess!Try Again" << RESET << endl;
            attempts++; // Increment attempts for incorrect guesses
        }
    }
    levelScore += 5; // Random score for level 3
    cout << GREEN<<"Level 3 completed with score: " << levelScore <<RESET<< endl;
    return true; // Indicate level completion
}

bool startLevel4(uint16_t &levelScore)
{
    cout << endl;
    cout << "Starting Level 4..." << endl;
    size_t attempts = 0;                               // Initialize attempts to 0
    size_t hintsUsed = 0;
    const size_t maxHints = 1;
    const string &randomWord = words3[randomNo(0, 9)]; // Random word selection from words4
    const int wordLength = randomWord.length();
    vector<bool> dashes(wordLength); // Array for dashes to represent the hidden word
    createDashes(randomWord, dashes);
    string shiftedWord = randomWord;                // Create a copy of the random word to shift
    string key = keys[randomNo(0, 9)];              // Random key selection from keys
    XOREncrypt(shiftedWord, key);                   // Encrypt the shifted word using XOR
    string input = "";
    string hexOutput;
    toHex(shiftedWord, hexOutput);              // Convert the shifted word to hexadecimal representation
    printLevel(randomWord, dashes, hexOutput, 4);// Print the initial state of the level
    while (input != randomWord && attempts < 5) // Allow up to 5 attempts
    {
        takeStringInput(input, true);
       if (input == "HINT" || input == "hint" || input == "Hint")
{
    if (hintsUsed < maxHints)
    {
        revealHint(randomWord, dashes);
        hintsUsed++;
    }
    else
    {
        cout << RED << "⚠ No hints remaining!" << RESET << endl;
    }

    printLevel(randomWord, dashes, hexOutput, 4);
    input = "";
    continue;
}
        if (input.empty())
        {
            cout << YELLOW << "⚠ Input cannot be empty. Please enter a valid guess." << RESET << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (!validateInput(input))
        {
            cout << YELLOW << "⚠ Invalid input. Please enter only alphabetic characters and spaces." << RESET << endl;
            continue; // Skip the rest of the loop and prompt for input again
        }
        if (input == randomWord)
        {
            cout <<GREEN<< "Congratulations! You've guessed the word correctly." << RESET <<endl;
            break; // Exit the loop if the word is guessed correctly
        }
        else
        {
            cout << RED << "Incorrect guess!Try Again" << RESET << endl;
            attempts++; // Increment attempts for incorrect guesses
        }
    }
    levelScore += 5; // Random score for level 4
    cout <<GREEN<< "Level 4 completed with score: " << levelScore <<RESET<< endl;
    return true; // Indicate level completion
}