#include <iostream>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#ifdef _WIN32
#include <windows.h>
#endif


using namespace std;
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";
int main() {
    srand(time(0));
    int score = 0;
    bool completed[5] = {false};

    cout << "\n" << CYAN;
    cout << "==============================================================" << endl;
    cout << "          CRYPTOGRAM: HISTORICAL CODEBREAKER EDITION          " << endl;
    cout << "==============================================================" << RESET << endl;

    while (true) {
        cout << "\n" << CYAN << "--------------------------------------------------------------" << RESET << endl;
        cout << "  CURRENT TOTAL SCORE: " << GREEN << score << RESET << endl;
        cout << CYAN << "--------------------------------------------------------------" << RESET << endl;
        cout << YELLOW << "  1 - 54 BC Gaul       : Caesar Shift" << RESET << (completed[1] ? GREEN + " [CLEARED]" + RESET : "") << endl;
        cout << YELLOW << "  2 - May 1941 Atlantic: Dual-Rotor Enigma" << RESET << (completed[2] ? GREEN + " [CLEARED]" + RESET : "") << endl;
        cout << YELLOW << "  3 - July 1863 Civil War: Vigenere Keyword" << RESET << (completed[3] ? GREEN + " [CLEARED]" + RESET : "") << endl;
        cout << YELLOW << "  4 - May 2017 Global  : WannaCry Cyberattack (XOR-Hex + Atbash)" << RESET << (completed[4] ? GREEN + " [CLEARED]" + RESET : "") << endl;
        cout << "  5 - View Declassified Codex Archive" << endl;
        cout << "  6 - View Scoring System" << endl;
        cout << RED << "  0 - Exit Game" << RESET << endl;
        cout << "--------------------------------------------------------------" << endl;

        int choice = 0;
        cout << "Choose an option (1-6) or 0 to exit: ";
       while (!(cin >> choice) || choice < 0 || choice > 6) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << RED << "[!] Invalid choice. Enter 1-6 or 0 to exit: " << RESET;
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

   cout << "\n" << CYAN;
cout << "==============================================================" << endl;
cout << "                 MISSION COMPLETE - GAME OVER                 " << endl;
cout << "                 FINAL TOTAL SCORE : " << GREEN << score << CYAN << endl;
cout << "==============================================================" << RESET << "\n" << endl;
    return 0;
}
