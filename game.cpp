#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include "words.hpp" // Brings in themed phrases, secret keys, briefings, and codex entries
#include "game.hpp"  // Brings in all function declarations for this file

using namespace std;

// Picks a random whole number between min and max (inclusive)
int randomNo(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Shifts one letter forward by 'shift' places (spaces stay unchanged)
char shiftChar(char c, int shift) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return ((c - base + shift) % 26) + base;
    }
    return c;
}

// Returns true if the player's guess contains only letters and spaces
bool validateInput(string input) {
    for (int i = 0; i < input.length(); i++) {
        if (!isalpha(input[i]) && !isspace(input[i])) {
            return false;
        }
    }
    return true;
}

// Hides half of the alphabetic letters with '_' while keeping spaces visible
string createDashes(string word) {
    int letterCount = 0;
    for (int i = 0; i < word.length(); i++) {
        if (isalpha(word[i])) {
            letterCount++; // Count only actual letters so spaces are never hidden
        }
    }

    string dashes(word.length(), ' '); // Start with spaces everywhere
    int hidden = 0;
    while (hidden < letterCount / 2) {
        int idx = randomNo(0, word.length() - 1);
        // Only hide alphabetic letters that aren't already hidden
        if (isalpha(word[idx]) && dashes[idx] != '_') {
            dashes[idx] = '_';
            hidden++;
        }
    }
    return dashes;
}

// Prints the era header, mission briefing, field intel note, puzzle board, and cipher chart
void printLevel(string word, string dashes, string encryptedWord, int level) {
    cout << "\n+--------------------------------------------------------------+" << endl;
    cout << "  LEVEL " << level << " : " << eraTitles[level] << endl;
    cout << "  LENGTH  : " << word.length() << " CHARACTERS (INCLUDING SPACES)" << endl;
    cout << "+--------------------------------------------------------------+" << endl;
    cout << "  MISSION BRIEFING:" << endl;
    cout << "  " << missionBriefings[level] << "\n" << endl;
    cout << "  " << cipherStyleNote << endl;
    cout << "+--------------------------------------------------------------+" << endl;

    cout << "  PUZZLE    : ";
    for (int i = 0; i < word.length(); i++) {
        if (dashes[i] == '_') cout << "_ ";      // Hide letter with underscore
        else if (word[i] == ' ') cout << "   ";   // Extra space so word gaps stand out
        else cout << word[i] << " ";              // Show hint letter
    }
    cout << "\n  ENCRYPTED : " << encryptedWord << endl;
    cout << "+--------------------------------------------------------------+" << endl;

    if (level <= 3) {
        cout << "  CIPHER REFERENCE (A=0 ... Z=25):" << endl;
        cout << "   A  B  C  D  E  F  G  H  I  J  K  L  M" << endl;
        cout << "   0  1  2  3  4  5  6  7  8  9 10 11 12" << endl;
        cout << "  --------------------------------------" << endl;
        cout << "   N  O  P  Q  R  S  T  U  V  W  X  Y  Z" << endl;
        cout << "  13 14 15 16 17 18 19 20 21 22 23 24 25\n" << endl;
    } else {
        cout << "  DUAL-CLUE TIP: Decode using EITHER the XOR Hex bytes OR the ATBASH-CIPHER clue (read backwards + flip A<->Z)!\n" << endl;
    }
    cout << "  (Type MENU or 0 anytime to switch levels)\n" << endl;
}

// Level 2 Cipher (Dual-Rotor): Shifts even positions with one key and odd positions with another
void encrypt2(string &input) {
    int shiftEven = randomNo(1, 25);
    int shiftOdd = randomNo(1, 25);
    for (int i = 0; i < input.length(); i++) {
        if (i % 2 == 0) input[i] = shiftChar(input[i], shiftEven);
        else input[i] = shiftChar(input[i], shiftOdd);
    }
}

// Level 3 Cipher (Vigenere): Shifts each letter using a repeating secret keyword
void vigenereEncrypt(string &input, string key) {
    for (int i = 0; i < input.length(); i++) {
        int shift = toupper(key[i % key.length()]) - 'A';
        input[i] = shiftChar(input[i], shift);
    }
}

// Level 4 Cipher (XOR): Bitwise XOR against a repeating secret keyword
void XOREncrypt(string &input, string key) {
    for (int i = 0; i < input.length(); i++) {
        input[i] = input[i] ^ key[i % key.length()];
    }
}

// Turns XOR-encrypted characters into 2-digit hexadecimal pairs
string toHex(string input) {
    string hexOutput = "";
    string hexDigits = "0123456789ABCDEF";
    for (int i = 0; i < input.length(); i++) {
        unsigned char byte = input[i];
        hexOutput += hexDigits[byte / 16]; // First hex digit
        hexOutput += hexDigits[byte % 16]; // Second hex digit
    }
    return hexOutput;
}

// Level 4 Secondary Cipher: Flips A<->Z and reverses the text right-to-left
string reversedAtbash(string input) {
    string mirrored = "";
    for (int i = 0; i < input.length(); i++) {
        if (isalpha(input[i])) {
            mirrored += ('Z' - (toupper(input[i]) - 'A'));
        } else {
            mirrored += input[i]; // Keep spaces unchanged
        }
    }

    string reversed = "";
    for (int i = mirrored.length() - 1; i >= 0; i--) {
        reversed += mirrored[i];
    }
    return reversed;
}

// Prints the 5-life bar and remaining seconds cleanly in one place
void printStatusBar(int lives, int remaining) {
    cout << "  LIVES: [";
    for (int i = 0; i < 5; i++) {
        cout << (i < lives ? "#" : "-");
    }
    cout << "] (" << lives << "/5)  |  TIME LEFT: " << remaining << "s   ";
}

// Runs the live 90-second ticking countdown, lives bar, scoring, and Codex unlock
bool playLevelLoop(string answer, string dashes, string encryptedWord, int level, int basePoints, int &levelScore) {
    printLevel(answer, dashes, encryptedWord, level);

    string input = "";
    int wrongGuesses = 0;
    int lives = 5;
    int timeLimit = 90;
    time_t startTime = time(0);
    bool levelRunning = true;
    bool timeExpired = false;

    cout << "\n";
    printStatusBar(lives, timeLimit);
    cout << "\n  Enter your guess: " << flush;

    // Background countdown that updates TIME LEFT every second while you type
    thread timerThread([&]() {
        while (levelRunning) {
            this_thread::sleep_for(chrono::seconds(1));
            if (!levelRunning) break;

            int remaining = timeLimit - (time(0) - startTime);
            if (remaining <= 0) {
                timeExpired = true;
                levelRunning = false;
                cout << "\n\n  TIME'S UP! Press Enter to continue..." << flush;
                break;
            }

            // Save cursor, move up 1 line to update status bar, restore cursor
            cout << "\033[s\033[1A\r";
            printStatusBar(lives, remaining);
            cout << "\033[u" << flush;
        }
    });

    while (input != answer && wrongGuesses < 5 && !timeExpired) {
        getline(cin, input);

        if (timeExpired || (time(0) - startTime >= timeLimit)) {
            timeExpired = true;
            break;
        }

        if (input == "") {
            cout << "  Input cannot be empty.\n\n";
            printStatusBar(lives, timeLimit - (time(0) - startTime));
            cout << "\n  Enter your guess: " << flush;
            continue;
        }

        for (int i = 0; i < input.length(); i++) {
            input[i] = toupper(input[i]);
        }

        if (input == "0" || input == "MENU") {
            levelRunning = false;
            timerThread.join();
            cout << "\n  Returning to level menu...\n" << endl;
            return false;
        }

        if (!validateInput(input)) {
            cout << "  Invalid input (letters and spaces only).\n\n";
            printStatusBar(lives, timeLimit - (time(0) - startTime));
            cout << "\n  Enter your guess: " << flush;
            continue;
        }

        if (input == answer) {
            levelRunning = false;
            timerThread.join();

            int timeTaken = time(0) - startTime;
            cout << "\n  MISSION ACCOMPLISHED! Decoded in " << timeTaken << "s." << endl;

            int earned = basePoints;
            if (wrongGuesses == 0) {
                earned += 2;
                cout << "  First-Try Bonus: +2 points!" << endl;
            } else {
                earned -= wrongGuesses;
                cout << "  Wrong Guess Penalty (" << wrongGuesses << " missed): -" << wrongGuesses << " points" << endl;
            }

            if (timeTaken <= 20) {
                earned += 3;
                cout << "  Lightning Speed Bonus (<= 20s): +3 points!" << endl;
            } else if (timeTaken <= 45) {
                earned += 2;
                cout << "  Quick Speed Bonus (21-45s): +2 points!" << endl;
            }

            if (earned < 1) earned = 1;
            levelScore = earned;
            cout << "  Level " << level << " Score Earned: " << levelScore << " pts" << endl;

            cout << "\n==============================================================" << endl;
            cout << "  DECLASSIFIED CODEX #0" << level << " : " << eraTitles[level] << endl;
            cout << "--------------------------------------------------------------" << endl;
            cout << "  " << codexEntries[level] << endl;
            cout << "==============================================================\n" << endl;
            return true;
        } else {
            wrongGuesses++;
            lives = 5 - wrongGuesses;
            if (wrongGuesses < 5 && !timeExpired) {
                cout << "  Incorrect guess. Try again.\n\n";
                printStatusBar(lives, timeLimit - (time(0) - startTime));
                cout << "\n  Enter your guess: " << flush;
            }
        }
    }

    levelRunning = false;
    if (timerThread.joinable()) timerThread.join();

    if (timeExpired) {
        cout << "\n  Time's up! The intercepted message was: " << answer << "\n" << endl;
    } else {
        cout << "\n  Out of attempts! The intercepted message was: " << answer << "\n" << endl;
    }
    return false;
}

// Single unified function to set up and start any level (1 to 4)
bool startLevel(int level, int &levelScore) {
    cout << "\nStarting Level " << level << "..." << endl;
    int idx = randomNo(0, 9);
    string answer = "";
    int basePoints = level + 2; // Level 1=3, Level 2=4, Level 3=5, Level 4=6

    if (level == 1) answer = words1[idx];
    else if (level == 2) answer = words2[idx];
    else if (level == 3) answer = words3[idx];
    else if (level == 4) answer = words4[idx];

    string dashes = createDashes(answer);
    string encryptedWord = answer;

    if (level == 1) {
        int shift = randomNo(1, 25);
        for (int i = 0; i < encryptedWord.length(); i++) {
            encryptedWord[i] = shiftChar(encryptedWord[i], shift);
        }
    } else if (level == 2) {
        encrypt2(encryptedWord);
    } else if (level == 3) {
        vigenereEncrypt(encryptedWord, keys[randomNo(0, 9)]);
        } else if (level == 4) {
        string xorCopy = answer;
        XOREncrypt(xorCopy, keys[randomNo(0, 9)]);
        encryptedWord = toHex(xorCopy) + "\n  ATBASH-CIPHER: " + reversedAtbash(answer);
    }
    return playLevelLoop(answer, dashes, encryptedWord, level, basePoints, levelScore);
}

// Shows unlocked Codex entries from the main menu
void viewCodexArchive(bool completed[]) {
    cout << "\n==============================================================" << endl;
    cout << "                 DECLASSIFIED CODEX ARCHIVE                   " << endl;
    cout << "==============================================================" << endl;
    for (int i = 1; i <= 4; i++) {
        cout << "\n  [CODEX #0" << i << " - " << eraTitles[i] << "]" << endl;
        if (completed[i]) cout << "  " << codexEntries[i] << endl;
        else cout << "  [LOCKED - Clear Level " << i << " to decrypt this historical entry.]" << endl;
    }
    cout << "\n==============================================================\n" << endl;
}

// Shows the scoring rules and min/max points table
void viewScoringSystem() {
    cout << "\n====================================================================" << endl;
    cout << "                      SCORING SYSTEM & RULES                        " << endl;
    cout << "====================================================================" << endl;
    cout << "  * Time Limit      : 90 seconds per level" << endl;
    cout << "  * Lives           : 5 attempts per level" << endl;
    cout << "  * First-Try Bonus : +2 points (solved with 0 wrong guesses)" << endl;
    cout << "  * Miss Penalty    : -1 point per wrong guess" << endl;
    cout << "  * Speed Bonuses   : +3 pts (<= 20s) | +2 pts (21-45s) | +0 (> 45s)" << endl;
    cout << "  * Replay Rule     : Replaying a [CLEARED] level is Practice Mode" << endl;
    cout << "                      (0 extra points added to total)" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    cout << "  LEVEL     | BASE | 1ST TRY | SPEED | MIN (FAIL/WIN) | MAX TOTAL" << endl;
    cout << "  ----------+------+---------+-------+----------------+----------" << endl;
    cout << "  Level 1   |  +3  |   +2    |  +3   |   0 / 1 pt     |   8 pts" << endl;
    cout << "  Level 2   |  +4  |   +2    |  +3   |   0 / 1 pt     |   9 pts" << endl;
    cout << "  Level 3   |  +5  |   +2    |  +3   |   0 / 1 pt     |  10 pts" << endl;
    cout << "  Level 4   |  +6  |   +2    |  +3   |   0 / 1 pt     |  11 pts" << endl;
    cout << "  ----------+------+---------+-------+----------------+----------" << endl;
    cout << "  FULL GAME |  18  |   +8    |  +12  |   0 / 4 pts    |  38 pts" << endl;
    cout << "====================================================================\n" << endl;
}
