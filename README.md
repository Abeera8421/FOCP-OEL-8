**Course:** CS110 — Fundamentals of Computer Programming (Class: BESE-17A)  
**Lab:** Lab 08 — Open Ended Lab (OEL)  
**Instructor:** Dr. Momina Moetesum | **Lab Engineer:** Ms. Sundus Dawood  

# Cryptogram: Historical Codebreaker Edition (`FOCP-OEL-8`)

**GitHub Repository:** [](https://github.com/Abeera8421/FOCP-OEL-8.git)

A modular, color-coded terminal C++ cryptography game where players step into the roles of historical codebreakers and modern cybersecurity analysts across four real-world eras—from Julius Caesar's campaigns in Gaul to the 2017 WannaCry cyberattack—cracking intercepted transmissions against a live ticking countdown timer to unlock declassified historical files.

---

## Table of Contents
1. [Project Architecture](#project-architecture)
2. [How to Compile & Run](#how-to-compile--run)
3. [How to Play & Menu Controls](#how-to-play--menu-controls)
4. [Encryption & Decryption Techniques](#encryption--decryption-techniques)
5. [Key Design Decisions](#key-design-decisions)
6. [Scoring System](#scoring-system)
7. [AI Tool Usage & Transparency](#ai-tool-usage--transparency)
8. [Individual Team Contributions](#individual-team-contributions)

---

## Project Architecture

The codebase is organized into four modular header and source files for clean separation of concerns and conflict-free team collaboration:

```text
FOCP-OEL-8/
├── main.cpp      # Entry point, main menu loop, input validation, cumulative score, and replay protection
├── game.hpp      # Function prototypes and ANSI color definitions (no 'using namespace std;' in headers)
├── game.cpp      # Cipher algorithms, live std::thread countdown timer, HINT system, and Codex viewer
├── words.hpp     # 4 historical phrase banks, 10 secret keywords, true-event briefings, and Codex entries
└── README.md     # Complete project documentation, build guide, design rationale, and contribution breakdown
```

---

## How to Compile & Run

### Prerequisites
* A **C++17** compatible compiler (`g++` or `clang++`) with POSIX thread support (`-pthread`).

### macOS / Linux
```bash
g++ -std=c++17 -pthread main.cpp game.cpp -o game
./game
```

### Windows (MinGW / PowerShell / VS Code Terminal)
```powershell
g++ -std=c++17 -pthread main.cpp game.cpp -o game.exe
.\game.exe
```

---

## How to Play & Menu Controls

1. **Main Menu Controls (`0–6`):**
   * **`1` to `4`:** Launch any of the four historical missions (playable in any order).
   * **`5`:** Open the **Declassified Codex Archive** to read unlocked real-world cipher histories.
   * **`6`:** Open the **Scoring System & Rules** breakdown table.
   * **`0`:** **Exit Game** (`0` terminates the main menu loop and displays your final total score).
2. **In-Level Commands & Gameplay:**
   * Read the **Mission Briefing**, **Field Intel**, and the color-coded **Puzzle Board** (hidden `_` letters highlighted in yellow while word spaces remain clear).
   * Beat the **90-second live countdown timer** and protect your **5 lives** by typing the decoded phrase (case-insensitive).
   * Type **`HINT`** during a puzzle to reveal a hidden letter when stuck.
   * Type **`MENU`** or **`0`** anytime to return to the main menu.

---

## Encryption & Decryption Techniques

Our game implements **five distinct cryptographic and encoding techniques** spanning ancient military history to modern cyber defense:

| Level | Historical Era | Technique(s) | Technical Implementation |
| :---: | :--- | :--- | :--- |
| **1** | **54 BC — Gaul (Roman Republic)** | **Caesar Shift Cipher** | Picks a random shift (`1–25`) and shifts every alphabetic letter forward via `((c - base + shift) % 26) + base`, keeping spaces unchanged. |
| **2** | **May 1941 — North Atlantic (WWII)** | **Dual-Rotor Alternating Shift** | Simulates multi-wheel rotor encryption by applying two independent random shifts (`1–25`)—one to even-indexed characters and another to odd-indexed characters. |
| **3** | **July 1863 — Siege of Vicksburg** | **Vigenère Polyalphabetic Cipher** | Encrypts letters using a repeating secret keyword (`A=0 ... Z=25`), ensuring identical plaintext letters map to different ciphertext letters depending on position. |
| **4** | **May 2017 — WannaCry Cyberattack** | **Bitwise XOR + Hex Dump & Reversed Atbash** | Provides two complementary clues: (1) bitwise XOR (`input[i] ^ key[i % len]`) formatted into 2-digit hexadecimal bytes (`toHex`), and (2) **Reversed Atbash** (`A <-> Z` alphabet mirror reversed right-to-left) so players can solve or cross-check either layer. |

---

## Key Design Decisions

* **Historical Narrative & Declassified Codex:** Grounded each level in a real historical event using short telegraphic command phrases (avoiding filler words like *"the"* or *"is"*). Winning a level unlocks a **Declassified Codex** entry explaining that cipher's real-world history.
* **ANSI Color-Coded Terminal UI (`\033[...m`):** Used standard ANSI escape sequences (`GREEN` for wins/scores, `RED` for wrong guesses/alerts, `YELLOW` for hidden puzzle underscores, `RESET` after every output) to boost visual clarity and immediate player feedback across terminals.
* **Interactive `HINT` Lifeline (`revealHint`):** Added an in-game `HINT` command that uncovers the next hidden `_` letter on the puzzle board and refreshes the display without costing a life.
* **Space-Preserving Multi-Word Puzzles (`createDashes`):** Counts only alphabetic characters (`isalpha`) when hiding 50% of letters so spaces between words stay visible and phrases remain readable.
* **Live Background Countdown (`std::thread` + ANSI Cursor Control):** Because `getline()` blocks input until `Enter` is pressed, a lightweight `std::thread` uses ANSI cursor save/restore codes (`\033[s`, `\033[1A`, `\033[u`) to tick `TIME LEFT` live every second without disrupting typing.
* **Unified Level Architecture (`startLevel`):** Consolidated four duplicate level functions into a single `startLevel(int level, int &levelScore)` function, eliminating repeated code while keeping each cipher distinct.
* **Practice Mode (Replay Protection):** Tracks cleared levels via `bool completed`, letting players replay `[CLEARED]` levels for practice (`0` extra points) without score farming.
* **Cross-Platform Portability:** Avoided OS-locked headers (`<windows.h>`) and `using namespace std;` in `.hpp` files so the game compiles cleanly on both macOS/Linux and Windows.

---

## Scoring System

* **Base Score Principle (`Base Points = Level + 2`):** Each level's base reward scales linearly with difficulty using the formula **`Base Points = Level + 2`** (Level 1 = `3`, Level 2 = `4`, Level 3 = `5`, Level 4 = `6`).
* **First-Try Bonus:** `+2` points (solved with `0` wrong guesses).
* **Wrong Guess Penalty:** `-1` point per missed attempt (up to `5` lives).
* **Speed Bonuses:** `+3` points (`<= 20s`) | `+2` points (`21–45s`) | `+0` points (`> 45s`).
* **Minimum Win Floor:** Clearing a level always awards at least `1` point (`0` points if failed).

| Level | Base Points (`Level + 2`) | 1st-Try Bonus | Max Speed Bonus | Min (Fail / Win) | Max Total |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **Level 1** | `+3` | `+2` | `+3` | `0 / 1 pt` | **8 pts** |
| **Level 2** | `+4` | `+2` | `+3` | `0 / 1 pt` | **9 pts** |
| **Level 3** | `+5` | `+2` | `+3` | `0 / 1 pt` | **10 pts** |
| **Level 4** | `+6` | `+2` | `+3` | `0 / 1 pt` | **11 pts** |
| **Full Game** | **18** | **+8** | **+12** | **0 / 4 pts** | **38 pts** |

---

## AI Tool Usage & Transparency

In accordance with the Lab 08 guidelines on supplementary tools, AI assistance (Google Gemini) was used constructively during development in the following ways:
* **Bug Hunting:** Identified bugs in early iterations—including out-of-bounds array indexing, hexadecimal ASCII formatting issues (`toHex`), input buffer clearing (`cin.ignore`), OS-specific header conflicts (`<windows.h>`), and safe background thread cleanup (`timerThread.join()`).
* **Algorithm Verification:** Verification of ASCII modular arithmetic, hexadecimal byte conversion (`toHex`), space-preserving dash generation, cross-platform ANSI escape codes, and safe thread cleanup (`timerThread.join()`), after the code was written by the team.
* **Historical Content & Documentation Formatting:** Assisted in drafting accurate true-event mission briefings, Declassified Codex explanations, and structured Markdown documentation.
* **ANSI Escape Sequences (Live Cursor & Terminal Colours):** Assisted in understanding the implementation and verifying cross-platform ANSI escape codes for both real-time cursor save/restore positioning (`\033[s`, `\033[1A`, `\033[u`) during the live countdown timer and terminal colour styling (`\033[32m` green for wins/scores, `\033[31m` red for wrong guesses/alerts, `\033[33m` yellow for hidden puzzle blanks/warnings, and `\033[0m` to safely reset formatting after each output).
* *All generated suggestions were manually tested, debugged, compiled, and verified by the team across macOS and Windows environments.*

---

## Individual Team Contributions

* **Abdullah Anwar (`@Abdullah860259`):**
  * Created the initial project files (`main.cpp` and `words.hpp`) to establish the foundational C++ codebase, including the main menu loop, level selection flow, and replay state tracking.
  * Structured the initial single-word banks (`words1`–`words3`) and secret encryption keyword arrays (`keys`) inside `words.hpp`.
  * Built the initial puzzle generation logic (`randomNo`, `getOmittedIndexes`, `createDashes`) and formatted board renderer (`printLevel`).
  * Implemented the encryption and encoding engines (`encrypt2`, `vigenereEncrypt`, `XOREncrypt`, `toHex`, and `toChar`).
  * Developed the guess-checking loop, input prompt helper (`takeStringInput`), attempt limits, and input sanitization (`validateInput`).
  * Managed Git branches and pull requests and performed cross-platform testing, debugging, and final submission verification.

* **Hooria Mansoor (`@hooriamansoorahmed-lgtm`):**
  * Implemented the Level 1 **Caesar Shift** encryption logic (`shiftChar`) to encode messages using monoalphabetic letter rotation.
  * Designed and integrated the **ANSI terminal color palette** (`GREEN`, `RED`, `YELLOW`, `RESET`) to provide clear visual feedback for success states, errors, and warnings.
  * Built the interactive **`HINT` lifeline** (`revealHint`) and command handling, allowing players to uncover hidden target characters mid-round without losing an attempt.

* **Abeera Ahsan (`@Abeera8421`):**
  * Refactored the codebase into modular files (`game.hpp`, `game.cpp`, `words.hpp`) and consolidated duplicate level loops into `startLevel()`.
  * Added the Level 4 array (`words4`) and transformed all four word banks (`words1`–`words4`) into historically themed multi-word phrase banks, alongside the 4 historical eras, true-event mission briefings, and the **Declassified Codex Archive** (Option `5`).
  * Rewrote `createDashes()` to preserve spaces in multi-word phrases and added the linear scoring formula (`Base = Level + 2`), speed/first-try bonuses, miss penalties, **Scoring System Table** (Option `6`), and Practice Mode.
  * Built the 90-second live background countdown timer (`std::thread` + ANSI cursor control) and implemented the **Reversed Atbash** mirror cipher (`reversedAtbash`) to power Level 4's dual-clue **XOR Hex + Reversed Atbash** challenge.
  * Ensured cross-platform macOS/Windows compilation, authored the project `README.md` and documentation, and performed final testing and debugging.

---

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
