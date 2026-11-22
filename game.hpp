#pragma once
#include <string>

// Helper & Display Functions
int randomNo(int min, int max);
char shiftChar(char c, int shift);
bool validateInput(std::string input);
std::string createDashes(std::string word);
void printLevel(std::string word, std::string dashes, std::string encryptedWord, int level);

// Encryption Functions
void encrypt2(std::string &input);
void vigenereEncrypt(std::string &input, std::string key);
void XOREncrypt(std::string &input, std::string key);
std::string toHex(std::string input);

// Gameplay & Menu Screens
bool playLevelLoop(std::string answer, std::string dashes, std::string encryptedWord, int level, int basePoints, int &levelScore);
bool startLevel(int level, int &levelScore);
void viewCodexArchive(bool completed[]);
void viewScoringSystem();
