#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

char shiftChar(char c, int shift)
{
    if (isalpha(c))
    {
        char base = islower(c) ? 'a' : 'A';
        return ((c - base + shift) % 26) + base;
    }
    return c; // Non-alphabetic characters are not shifted
}

int randomNo(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int main()
{
    srand(time(0)); // Seed the random number generator
    return 0;
}