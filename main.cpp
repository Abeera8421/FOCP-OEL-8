#include <iostream>
#include <cstdlib>
#include <ctime>
#include "game.hpp"

using namespace std;

int main() {
    srand(time(0));
    int score = 0;
    bool completed[5] = {false};

    cout << "\n==============================================================" << endl;
    cout << "          CRYPTOGRAM: HISTORICAL CODEBREAKER EDITION          " << endl;
    cout << "==============================================================" << endl;

    while (true) {
        cout << "\n--------------------------------------------------------------" << endl;
        cout << "  CURRENT TOTAL SCORE: " << score << endl;
        cout << "--------------------------------------------------------------" << endl;
        cout << "  1 - 54 BC Gaul       : Caesar Shift" << (completed[1] ? " [CLEARED]" : "") << endl;
        cout << "  2 - May 1941 Atlantic: Dual-Rotor Enigma" << (completed[2] ? " [CLEARED]" : "") << endl;
        cout << "  3 - July 1863 Civil War: Vigenere Keyword" << (completed[3] ? " [CLEARED]" : "") << endl;
        cout << "  4 - May 2017 Global  : WannaCry Cyberattack (XOR Hex)" << (completed[4] ? " [CLEARED]" : "") << endl;
        cout << "  5 - View Declassified Codex Archive" << endl;
        cout << "  6 - View Scoring System" << endl;
        cout << "  0 - Exit Game" << endl;
        cout << "--------------------------------------------------------------" << endl;

        int choice = 0;
        cout << "Choose an option (1-6) or 0 to exit: ";
        while (!(cin >> choice) || choice < 0 || choice > 6) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice. Enter 1-6 or 0 to exit: ";
        }
        cin.ignore(1000, '\n');

        if (choice == 0) break;

        if (choice == 5) {
            viewCodexArchive(completed);
            continue;
        }
        if (choice == 6) {
            viewScoringSystem();
            continue;
        }

        int levelScore = 0;
        bool levelComplete = startLevel(choice, levelScore);

        if (levelComplete) {
            if (!completed[choice]) {
                score += levelScore;
                completed[choice] = true;
            } else {
                cout << "  (Practice Mode: Level " << choice << " was already cleared, so 0 pts added to total.)\n" << endl;
            }
        }
    }

    cout << "\n==============================================================" << endl;
    cout << "                 MISSION COMPLETE - GAME OVER                 " << endl;
    cout << "                 FINAL TOTAL SCORE : " << score << endl;
    cout << "==============================================================\n" << endl;
    return 0;
}
