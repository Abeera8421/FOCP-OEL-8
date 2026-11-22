#pragma once
#include <string>

// Level 1 (54 BC - Gaul / Caesar Cipher)
const std::string words1[] = {
    "SEND LEGIONS", "HOLD CAMP", "MARCH AT DAWN", "CROSS RHINE", "FORM SHIELD WALL",
    "RELIEF NEAR", "GUARD GATE", "STRIKE AT NIGHT", "ROMA INVICTA", "DEFEND FORT"
};

// Level 2 (1941 - WWII Atlantic / Dual-Rotor Enigma)
const std::string words2[] = {
    "SINK U BOAT", "ALLIED CONVOY", "DIVE DEEP NOW", "NORTH ATLANTIC", "BREAK ENIGMA",
    "SUBMARINE SPOTTED", "CHANGE COURSE", "PROTECT SHIPS", "NAVAL SIGNAL", "HUNT WOLFPACK"
};

// Level 3 (1863 - Siege of Vicksburg / Vigenere Cipher)
const std::string words3[] = {
    "HOLD VICKSBURG", "GUARD RIVER", "UNION ADVANCE", "SEND SUPPLIES", "NIGHT ATTACK",
    "DEFEND LINE", "REINFORCE FORT", "CUT TELEGRAPH", "SCOUT FLANK", "FINAL STAND"
};

// Level 4 (2017 - WannaCry Cyberattack / XOR & Hex)
const std::string words4[] = {
    "KILL SWITCH", "BLOCK PORT", "STOP MALWARE", "PATCH SYSTEM", "TRACE DOMAIN",
    "LOCKED FILES", "DECRYPT DATA", "BREACH DETECTED", "ISOLATE HOST", "SYSTEM SAVED"
};

// 10 secret keywords for Levels 3 and 4
const std::string keys[] = {
    "CAESAR", "ENIGMA", "TURING", "CIPHER", "VICTORY",
    "VICKSBURG", "SHADOW", "BINARY", "HUNTER", "SHIELD"
};

// Era titles (index 1 to 4)
const std::string eraTitles[] = {
    "",
    "54 BC - GAUL (ROMAN REPUBLIC)",
    "MAY 1941 - NORTH ATLANTIC (WWII)",
    "JULY 1863 - SIEGE OF VICKSBURG",
    "MAY 2017 - WANNACRY CYBERATTACK"
};

// True-event mission briefings shown before guessing
const std::string missionBriefings[] = {
    "",
    "54 BC, Gaul. Enemy tribes have surrounded a Roman army camp. Julius Caesar sends a secret letter tied to a spear to warn his troops. Shift the letters back to read his message!",
    "May 1941, Atlantic Ocean. British sailors just captured a secret Enigma code machine from a German submarine. Crack the radio signal to save Allied supply ships!",
    "July 1863, Vicksburg. Union scouts just intercepted a Confederate telegraph locked with a secret keyword. Break the keyword shift to uncover their troop plans!",
    "May 2017, Global Cyberattack. A dangerous computer worm is locking hospital computers worldwide. Decode the hex bytes to find the hidden kill-switch and stop it!"
};

// Historical note explaining why real dispatches use short phrases
const std::string cipherStyleNote =
    "FIELD INTEL: Real military cryptographers avoided filler words like 'THE', 'IS', or 'WILL'—"
    "sending short telegraphic phrases saved critical time and prevented enemy codebreakers from spotting common word patterns.";

// Educational Codex entries unlocked after winning each level
const std::string codexEntries[] = {
    "",
    "HOW IT WORKED (CAESAR SHIFT): Julius Caesar hid military orders by shifting every letter forward in the alphabet (usually by 3). Even if enemy scouts caught the messenger, they couldn't read the plan.",
    "HOW IT WORKED (ROTOR MACHINES): WWII Enigma machines changed the letter shift on every keypress using spinning wheels (rotors). Alan Turing's team at Bletchley Park cracked these daily settings and helped end the war early.",
    "HOW IT WORKED (VIGENERE CIPHER): Instead of one shift for the whole phrase, the Vigenere cipher uses a repeating keyword so the same letter turns into different letters—making it much harder to guess.",
    "HOW IT WORKED (BITWISE XOR & HEX): Modern computers encrypt data using XOR—a fast math trick that flips binary bits with a secret key. Running XOR a second time with the same key instantly unlocks the original text."
};
