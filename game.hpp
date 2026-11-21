#pragma once
#include <string>
#include <cstdint>

// Helper & Input Functions
char shiftChar(char c, int shift);
int randomNo(int min, int max);
bool validateInput(const std::string &input);
void getOmittedIndexes(const std::string &word, int omitted[]);
void createDashes(const std::string &word, std::string &dashes);
void printLevel(const std::string &word, const std::string &dashes, const std::string &shiftedWord, uint16_t level);
void takeStringInput(std::string &input);

// Encryption & Conversion Functions
void vigenereEncrypt(std::string &input, const std::string &key);
void encrypt2(std::string &input);
void XOREncrypt(std::string &input, const std::string &key);
void toHex(const std::string &input, std::string &hexOutput);

// Gameplay & Level Functions
bool playLevelLoop(const std::string &randomWord, const std::string &dashes, const std::string &encryptedDisplay, uint16_t levelNum, uint16_t basePoints, uint16_t &levelScore);
bool startLevel1(uint16_t &levelScore);
bool startLevel2(uint16_t &levelScore);
bool startLevel3(uint16_t &levelScore);
bool startLevel4(uint16_t &levelScore);
