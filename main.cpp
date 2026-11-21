#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdint>
#include "words.hpp"

using namespace std;

// Function prototypes
char shiftChar(char c, int shift);
int randomNo(int min, int max);
bool validateInput(const string &input);
void getOmittedIndexes(const string &word, int omitted[]);
void createDashes(const string &word, string &dashes);
void printLevel(const string &word, const string &dashes, const string &shiftedWord, uint16_t level);
void takeStringInput(string &input);
void vigenereEncrypt(string &input, const string &key);
void encrypt2(string &input);
void XOREncrypt(string &input, const string &key);
void toHex(const string &input, string &hexOutput);
bool startLevel1(uint16_t &levelScore);
bool startLevel2(uint16_t &levelScore);
bool startLevel3(uint16_t &levelScore);
bool startLevel4(uint16_t &levelScore);
bool playLevelLoop(const string &randomWord, const string &dashes, const string &encryptedDisplay, uint16_t levelNum, uint16_t basePoints, uint16_t &levelScore);

int main() {
    srand(time(0));              // Seed the random number generator
    uint16_t score = 0;          // Initialize total score across all levels to 0
    bool completed[5] = {false}; // Tracks cleared levels (indexes 1-4) to prevent score farming

    cout << "\n==================================================" << endl;
    cout << "          CRYPTOGRAM: CODEBREAKER EDITION         " << endl;
    cout << "==================================================" << endl;

    // Main menu loop allowing the player to choose any level in any order
    while (true) {
        cout << "\n--------------------------------------------------" << endl;
        cout << "  CURRENT TOTAL SCORE: " << score << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "  Level 1 - Caesar Cipher" << (completed[1] ? " [CLEARED]" : "") << endl;
        cout << "  Level 2 - Dual-Rotor Shift" << (completed[2] ? " [CLEARED]" : "") << endl;
        cout << "  Level 3 - Vigenere Keyword Cipher" << (completed[3] ? " [CLEARED]" : "") << endl;
        cout << "  Level 4 - XOR Hexadecimal Dump" << (completed[4] ? " [CLEARED]" : "") << endl;
        cout << "  0 - Exit Game" << endl;
        cout << "--------------------------------------------------" << endl;

        int choice = 0;
        cout << "Choose a level to play (1-4) or 0 to exit: ";
        while (!(cin >> choice) || choice < 0 || choice > 4) {
            cin.clear();             // Reset error flag if non-numeric input was entered
            cin.ignore(1000, '\n');  // Discard invalid characters from input buffer
            cout << "Invalid choice. Enter 1-4 to play or 0 to exit: ";
        }
        cin.ignore(1000, '\n');      // Clear leftover newline so getline() works properly inside levels

        if (choice == 0) break;      // Exit the game loop when user selects 0

        bool levelComplete = false;
        uint16_t levelScore = 0;     // Holds points earned during the selected level

        // Launch the selected level
        if (choice == 1) levelComplete = startLevel1(levelScore);
        else if (choice == 2) levelComplete = startLevel2(levelScore);
        else if (choice == 3) levelComplete = startLevel3(levelScore);
        else if (choice == 4) levelComplete = startLevel4(levelScore);

        // Add points to total score only if the level hasn't been cleared before
        if (levelComplete) {
            if (!completed[choice]) {
                score += levelScore;
                completed[choice] = true; // Mark level cleared so replaying runs in practice mode
            } else {
                cout << "  (Practice Mode: Level " << choice << " was already cleared, so 0 pts added to total.)\n" << endl;
            }
        }
    }

    cout << "\n==================================================" << endl;
    cout << "           MISSION COMPLETE - GAME OVER           " << endl;
    cout << "           FINAL TOTAL SCORE : " << score << endl;
    cout << "==================================================\n" << endl;
    return 0;
}

// Function to validate input string (only alphabetic characters and spaces are allowed)
bool validateInput(const string &input) {
    for (char c : input) {
        if (!isalpha(c) && !isspace(c)) return false; // Invalid character found
    }
    return true; // All characters are valid
}

// Function to shift a character by a given number of positions in the alphabet
char shiftChar(char c, int shift) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A'; // Preserve uppercase or lowercase base
        return ((c - base + shift) % 26) + base;
    }
    return c; // Non-alphabetic characters are not shifted
}

// Function to generate a random number between min and max (inclusive)
int randomNo(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Builds a 0/1 array marking half the word's indexes at random to hide (1 = hide, 0 = show)
void getOmittedIndexes(const string &word, int omitted[]) {
    int wordLength = word.length();
    for (int i = 0; i < wordLength; ++i) {
        omitted[i] = 0; // Start with all letters visible (0)
    }

    int targetOmitted = wordLength / 2; // Omit half the word randomly
    int omittedCount = 0;
    while (omittedCount < targetOmitted) {
        int randomIndex = randomNo(0, wordLength - 1);
        if (omitted[randomIndex] == 0) {
            omitted[randomIndex] = 1; // Mark random index as omitted (1)
            omittedCount++;
        }
    }
}

// Turns the 0/1 omitted array into a string of underscores ('_') and spaces (' ')
void createDashes(const string &word, string &dashes) {
    int wordLength = word.length();
    int omitted[50];                  // Fixed-size array large enough for any word in words.hpp
    getOmittedIndexes(word, omitted); // Populate the 0/1 omitted array

    dashes = string(wordLength, ' '); // Initialize all positions as visible (' ')
    for (int i = 0; i < wordLength; ++i) {
        if (omitted[i] == 1) {
            dashes[i] = '_'; // Omit element where array value is 1
        }
    }
}

// Draws the puzzle card, hidden word, encrypted clue, and alphabet table
void printLevel(const string &word, const string &dashes, const string &shiftedWord, uint16_t level) {
    cout << "\n+------------------------------------------------+" << endl;
    cout << "  LEVEL " << level << "  |  WORD LENGTH: " << word.length() << " LETTERS" << endl;
    cout << "+------------------------------------------------+" << endl;
    cout << "  PUZZLE    : ";
    for (size_t i = 0; i < word.length(); ++i) {
        if (dashes[i] == '_') {
            cout << "_ "; // Hide letter with an underscore
        } else {
            cout << word[i] << " "; // Reveal hint letter
        }
    }
    cout << "\n  ENCRYPTED : " << shiftedWord << endl;
    cout << "+------------------------------------------------+" << endl;

    if (level <= 3) {
        // Show A=0..Z=25 lookup grid for alphabetical shift levels
        cout << "  CIPHER REFERENCE (A=0 ... Z=25):" << endl;
        cout << "   A  B  C  D  E  F  G  H  I  J  K  L  M" << endl;
        cout << "   0  1  2  3  4  5  6  7  8  9 10 11 12" << endl;
        cout << "  --------------------------------------" << endl;
        cout << "   N  O  P  Q  R  S  T  U  V  W  X  Y  Z" << endl;
        cout << "  13 14 15 16 17 18 19 20 21 22 23 24 25\n" << endl;
    } else {
        // Show hex hint for Level 4 XOR output
        cout << "  HEX TIP: Each 2-digit pair = 1 XOR-encrypted byte.\n" << endl;
    }
    cout << "  (Type MENU or 0 anytime to switch levels)\n" << endl;
}

// Reads the player's full guess line cleanly aligned with the puzzle UI
void takeStringInput(string &input) {
    cout << "  Enter your guess: ";
    getline(cin, input);
}

// Level 2 Cipher: Shifts even-indexed letters by one random key and odd-indexed by another
void encrypt2(string &input) {
    uint8_t shiftEven = randomNo(1, 25); // Random shift for even-indexed characters
    uint8_t shiftOdd = randomNo(1, 25);  // Random shift for odd-indexed characters
    for (size_t i = 0; i < input.length(); ++i) {
        input[i] = shiftChar(input[i], (i % 2 == 0) ? shiftEven : shiftOdd);
    }
}

// Level 3 Cipher: Shifts each letter using a repeating secret keyword
void vigenereEncrypt(string &input, const string &key) {
    for (size_t i = 0; i < input.length(); ++i) {
        int shift = toupper(key[i % key.length()]) - 'A'; // Repeat key if word is longer than key
        input[i] = shiftChar(input[i], shift);
    }
}

// Level 4 Cipher: Bitwise XORs each character against a repeating secret keyword
void XOREncrypt(string &input, const string &key) {
    for (size_t i = 0; i < input.length(); ++i) {
        input[i] ^= key[i % key.length()]; // XOR each byte with the corresponding key byte
    }
}

// Converts raw XOR-encrypted bytes into readable 2-digit hexadecimal pairs
void toHex(const string &input, string &hexOutput) {
    hexOutput.clear(); // Clear output string before appending hex pairs
    const string hexDigits = "0123456789ABCDEF";
    for (unsigned char byte : input) {
        hexOutput += hexDigits[byte / 16]; // First hex digit (quotient when divided by 16)
        hexOutput += hexDigits[byte % 16]; // Second hex digit (remainder when divided by 16)
    }
}

// Shared gameplay engine for all levels: timer, lives, validation, menu exit, and scoring
bool playLevelLoop(const string &randomWord, const string &dashes, const string &encryptedDisplay, uint16_t levelNum, uint16_t basePoints, uint16_t &levelScore) {
    printLevel(randomWord, dashes, encryptedDisplay, levelNum); // Display puzzle board and cipher reference

    string input = "";
    int attempts = 0;           // Track wrong guesses (max 5 allowed)
    const int TIME_LIMIT = 60;  // 60-second countdown allowed per level
    time_t startTime = time(0); // Record exact timestamp when the level begins

    while (input != randomWord && attempts < 5) {
        int remaining = TIME_LIMIT - (time(0) - startTime); // Calculate remaining seconds on the clock
        if (remaining <= 0) {
            cout << "\n  Time's up! You ran out of time. The word was: " << randomWord << "\n" << endl;
            return false;
        }

        // Print visual lives bar ('#' = remaining life, '-' = lost life)
        int livesLeft = 5 - attempts;
        cout << "\n  LIVES: [";
        for (int i = 0; i < 5; i++) {
            if (i < livesLeft) cout << "#";
            else cout << "-";
        }
        cout << "] (" << livesLeft << "/5)  |  TIME LEFT: " << remaining << "s" << endl;

        takeStringInput(input);

        // Check if the 60-second limit expired while the player was typing
        if (time(0) - startTime >= TIME_LIMIT) {
            cout << "\n  Time's up! You ran out of time. The word was: " << randomWord << "\n" << endl;
            return false;
        }

        // Ignore empty Enter presses so they don't waste a life
        if (input.empty()) {
            cout << "\n  Input cannot be empty. Please enter a valid guess." << endl;
            continue;
        }

        // Convert every letter to uppercase so lowercase guesses work too
        for (char &c : input) {
            c = toupper(c);
        }

        // Allow player to type 0 or MENU anytime to abort and switch levels
        if (input == "0" || input == "MENU") {
            cout << "\n  Returning to level menu...\n" << endl;
            return false;
        }

        // Ensure the guess contains only alphabetic characters and spaces
        if (!validateInput(input)) {
            cout << "\n  Invalid input. Please enter only alphabetic characters and spaces." << endl;
            continue;
        }

        // Check if the player guessed the right word
        if (input == randomWord) {
            int timeTaken = time(0) - startTime;
            cout << "\n  Congratulations! You've guessed the word correctly in " << timeTaken << "s." << endl;

            int earned = basePoints; // Start with the level's base points (3, 4, 5, or 6)

            // 1. Accuracy bonus (+2 on 1st try) or penalty (-1 per missed guess)
            if (attempts == 0) {
                earned += 2;
                cout << "  First-Try Bonus: +2 points!" << endl;
            } else {
                earned -= attempts;
                cout << "  Wrong Guess Penalty (" << attempts << " missed): -" << attempts << " points" << endl;
            }

            // 2. Tiered speed bonus (<= 15s gives +3, 16-30s gives +2)
            if (timeTaken <= 15) {
                earned += 3;
                cout << "  Lightning Speed Bonus (<= 15s): +3 points!" << endl;
            } else if (timeTaken <= 30) {
                earned += 2;
                cout << "  Quick Speed Bonus (16-30s): +2 points!" << endl;
            }

            if (earned < 1) earned = 1; // Always award at least 1 point for winning
            levelScore += earned;
            cout << "  Level " << levelNum << " Score Earned: " << levelScore << " pts\n" << endl;
            return true; // Only return true when guessed right
        } else {
            cout << "\n  Incorrect guess. Try again." << endl;
            attempts++;
        }
    }

    cout << "\n  Out of attempts! The word was: " << randomWord << "\n" << endl;
    return false; // Return false if all 5 attempts fail
}

// Level 1: Caesar Cipher (uniform forward shift across all letters)
bool startLevel1(uint16_t &levelScore) {
    cout << "Starting Level 1..." << endl;
    const string &randomWord = words1[randomNo(0, 9)]; // Random word selection from words1
    string dashes = "";                                // Uses a simple string instead of vector<bool>
    createDashes(randomWord, dashes);                  // Generate random hidden positions for the puzzle board
    uint8_t randomShift = randomNo(1, 25);             // Random shift between 1 and 25
    string shiftedWord = randomWord;
    for (char &c : shiftedWord) {
        c = shiftChar(c, randomShift);                 // Shift each character in the word
    }
    return playLevelLoop(randomWord, dashes, shiftedWord, 1, 3, levelScore);
}

// Level 2: Dual-Rotor Shift (separate random shifts for even and odd letter positions)
bool startLevel2(uint16_t &levelScore) {
    cout << "Starting Level 2..." << endl;
    const string &randomWord = words2[randomNo(0, 9)]; // Random word selection from words2
    string dashes = "";                                // Uses a simple string instead of vector<bool>
    createDashes(randomWord, dashes);                  // Generate random hidden positions for the puzzle board
    string shiftedWord = randomWord;
    encrypt2(shiftedWord);                             // Apply alternating even/odd Caesar shifts
    return playLevelLoop(randomWord, dashes, shiftedWord, 2, 4, levelScore);
}

// Level 3: Vigenere Keyword Cipher (polyalphabetic shift driven by a secret keyword)
bool startLevel3(uint16_t &levelScore) {
    cout << "\nStarting Level 3..." << endl;
    const string &randomWord = words3[randomNo(0, 9)]; // Random word selection from words3
    string dashes = "";                                // Uses a simple string instead of vector<bool>
    createDashes(randomWord, dashes);                  // Generate random hidden positions for the puzzle board
    string shiftedWord = randomWord;
    vigenereEncrypt(shiftedWord, keys[randomNo(0, 9)]); // Encrypt using repeating keyword cipher
    return playLevelLoop(randomWord, dashes, shiftedWord, 3, 5, levelScore);
}

// Level 4: XOR Byte Cipher + Hexadecimal Encoding (bitwise XOR displayed as clean hex pairs)
bool startLevel4(uint16_t &levelScore) {
    cout << "\nStarting Level 4..." << endl;
    const string &randomWord = words4[randomNo(0, 9)]; // Random word selection from words4
    string dashes = "";                                // Uses a simple string instead of vector<bool>
    createDashes(randomWord, dashes);                  // Generate random hidden positions for the puzzle board
    string shiftedWord = randomWord;
    XOREncrypt(shiftedWord, keys[randomNo(0, 9)]);      // Encrypt the word using bitwise XOR
    string hexOutput;
    toHex(shiftedWord, hexOutput);                     // Convert raw XOR bytes to clean hexadecimal pairs
    return playLevelLoop(randomWord, dashes, hexOutput, 4, 6, levelScore);
}