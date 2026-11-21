#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include "game.hpp"

using namespace std;

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
