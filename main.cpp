//
//  main.cpp
//  wordle
//
//  Created by Colin Ding on 10/4/22.
//  Copyright © 2022 Colin Ding. All rights reserved.
//

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#define DIMBLACK "\033[2m\033[30m"    /* Dim Black */
#define DIMRED "\033[2m\033[31m"      /* Dim Red */
#define DIMGREEN "\033[2m\033[32m"    /* Dim Green */
#define DIMYELLOW "\033[2m\033[33m"   /* Dim Yellow */
#define DIMBLUE "\033[2m\033[34m"     /* Dim Blue */
#define DIMMAGENTA "\033[2m\033[35m"  /* Dim Magenta */
#define DIMCYAN "\033[2m\033[36m"     /* Dim Cyan */
#define DIMWHITE "\033[2m\033[37m"    /* Dim White */
#define MOVEUP "\033["                /* add integer, and add F */
#define ERASELINE "\033[2K"           /* erase current line */

const int LENGTH = 5;

struct {
    char wrong = '_';
    char in_word = 'O';
    char correct = '#';
} LETTERVALUES;

std::string KEYBOARDCOLORS[] = {
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE};

std::vector<std::string> readSolutions(int length) {
    std::string line;
    std::vector<std::string> vec;

    std::ifstream fin;
    fin.open("./words/solutions.txt");

    while (getline(fin, line)) {
        if (line.length() == length) {
            vec.push_back(line);
        }
    }

    fin.close();

    return vec;
}

std::vector<std::string> readGuesses(int length) {
    std::string line;
    std::vector<std::string> vec;

    std::ifstream fin;
    fin.open("./words/guesses.txt");

    while (getline(fin, line)) {
        if (line.length() == length) {
            vec.push_back(line);
        }
    }

    fin.close();

    return vec;
}

std::string chooseRandom(std::vector<std::string> vec) {
    auto iterator = vec.begin();
    int random_number = rand() % vec.size();

    advance(iterator, random_number);

    return iterator[0];
}

char getNthLetter(int n) {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n - 1];
}

int getNumFromChar(char x) {
    return ((int)x) - 64;
}

std::string checkWord(std::string text, std::string correct, int len) {
    std::string out;

    for (int i = 0; i < len; i++) {
        char text_character = text[i];
        char correct_character = correct[i];

        if (text_character == correct_character) {
            out += LETTERVALUES.correct;  // string(GREEN) + LETTERVALUES.correct + string(RESET);
            correct[i] = '0';
        } else if (correct.find(text_character) != std::string::npos) {
            out += LETTERVALUES.in_word;  // string(YELLOW) + LETTERVALUES.in_word + string(RESET);
            correct[i] = '0';
        } else {
            out += LETTERVALUES.wrong;  // string(DIMWHITE) + LETTERVALUES.wrong + string(RESET);
        }
    }

    return out;
}

std::string getKeyboardForPrinting() {
    std::string out;
    int i = 0;

    for (auto color : KEYBOARDCOLORS) {
        out += std::string(color) + getNthLetter(i) + std::string(RESET);
        i++;
    }

    return out;
}

void play(std::string solution, std::vector<std::string> all_guesses) {
    std::cout << BOLDWHITE;
    std::cout << "Welcome to Wordle!" << std::endl;
    std::cout << "Play by guessing 5-letter words." << std::endl;
    std::cout << "The feedback will give you clues:" << std::endl;
    std::cout << " GRAY   = Character not in word at all" << std::endl;
    std::cout << " GREEN  = Character found and position correct" << std::endl;
    std::cout << " YELLOW = Character found but position incorrect" << std::endl;
    std::cout << RESET;

    std::cout << "\n\n";
    std::cout << getKeyboardForPrinting() << std::endl;

    int lines_since_keyboard = 1;

    for (int i = 0; i <= 5; i++) {
        std::string guess;

        std::cout << BOLDWHITE << "Guess a word (" + std::to_string(i + 1) + "/6): " << RESET;
        std::cin >> guess;
        transform(guess.begin(), guess.end(), guess.begin(), ::toupper);

        lines_since_keyboard++;

        if (guess == "STOP") {
            return;
        }

        if (find(all_guesses.begin(), all_guesses.end(), guess) != all_guesses.end()) {
            std::string feedback = checkWord(guess, solution, LENGTH);

            std::string to_write = "Guess a word (" + std::to_string(i + 1) + "/6): ";

            for (int j = 0; j < 5; j++) {
                char inputted_character = guess[j];
                char feedback_character = feedback[j];

                int keyboard_colors_index = getNumFromChar(inputted_character);  // inputted_character);

                std::string color_to_make;

                if (feedback_character == LETTERVALUES.wrong) {
                    color_to_make = DIMWHITE;
                } else if (feedback_character == LETTERVALUES.correct) {
                    color_to_make = GREEN;
                } else if (feedback_character == LETTERVALUES.in_word) {
                    color_to_make = YELLOW;
                } else {
                    color_to_make = BLACK;
                }

                to_write += color_to_make + inputted_character + RESET;

                if (KEYBOARDCOLORS[keyboard_colors_index] == WHITE) {
                    KEYBOARDCOLORS[keyboard_colors_index] = color_to_make;
                } else if (KEYBOARDCOLORS[keyboard_colors_index] != GREEN) {
                    if ((KEYBOARDCOLORS[keyboard_colors_index] != RED) && (color_to_make == GREEN)) {
                        KEYBOARDCOLORS[keyboard_colors_index] = color_to_make;
                    }
                } else {
                    KEYBOARDCOLORS[keyboard_colors_index] = color_to_make;
                }
            }

            std::cout << std::string(MOVEUP) + "1F";
            std::cout << to_write << std::endl;

            std::cout << std::string(MOVEUP) + std::to_string(lines_since_keyboard) + "F";
            std::cout << getKeyboardForPrinting() << std::endl;
            std::cout << std::string(MOVEUP) + std::to_string(lines_since_keyboard) + "E";

            if (guess == solution) {
                std::cout << "You won in " << i + 1 << " moves!" << std::endl;
                return;
            }
        } else {
            std::cout << std::string(MOVEUP) + "1F";
            std::cout << ERASELINE;

            std::cout << "Guess a word (" + std::to_string(i + 1) + "/6): " << BOLDRED << "INVALID";
            std::cout << std::string(MOVEUP) + "1E";

            i--;
        }
    }

    std::cout << "You lost!" << std::endl;
    std::cout << "The correct word was " << solution << std::endl;
}

void printHelpExit() {
    std::cout << "Usage: wordle [-h|--help] [-s|--showsolution]" << std::endl;
    std::cout << "\n";
    std::cout << "Option\t\t\tBehavior" << std::endl;
    std::cout << "------\t\t\t--------" << std::endl;
    std::cout << "-s, -showsolution\tShow the solution at beginning (debug)" << std::endl;
    std::cout << "-h, --help\t\tPrint this help text and quit" << std::endl;

    exit(0);
}

bool contains(std::vector<char> c, char e) {
    return find(begin(c), end(c), e) != end(c);
};

std::vector<std::string> applyRule(std::vector<std::string> all_words, std::vector<char> s1, std::vector<char> s2, std::vector<char> s3, std::vector<char> s4, std::vector<char> s5) {
    std::vector<std::string> new_solutions;

    std::copy_if(all_words.begin(), all_words.end(), back_inserter(new_solutions),
                 [s1, s2, s3, s4, s5](std::string const& x) { return (contains(s1, x[0]) && (contains(s2, x[1])) && (contains(s3, x[2])) && (contains(s4, x[3])) && (contains(s5, x[4]))); });

    return new_solutions;
}

void solver(std::vector<std::string> all_solutions, bool log_s) {
    std::cout << BOLDCYAN;
    std::cout << "Welcome to the Wordle solver!!" << std::endl;
    std::cout << "To use this solver, simply follow the instructions;" << std::endl;
    std::cout << "Input the word it suggests in your game:" << std::endl;
    std::cout << "Make sure to give the program the feedback!" << std::endl;

    std::cout << "'_' = gray tile" << std::endl;
    std::cout << "'+' = yellow tile" << std::endl;
    std::cout << "'=' = green tile" << std::endl;

    std::cout << RESET;

    std::cout << "\n\n";

    std::vector<char> s1, s2, s3, s4, s5;

    char letters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    for (char c : letters) {
        s1.push_back(c);
        s2.push_back(c);
        s3.push_back(c);
        s4.push_back(c);
        s5.push_back(c);
    }

    std::cout << BOLDMAGENTA;

    for (int i = 0; i < 6; i++) {
        std::string response, chosen_word;

        if (i == 0) {
            chosen_word = "CRATE";
        } else {
            chosen_word = chooseRandom(all_solutions);
        }

        while (response.size() != 5) {
            if (response == "STOP") {
                return;
            } else {
                if (response == "LIST") {
                    // todo, pritn all_solutions
                }
                std::cout << "Try the word " + chosen_word + ": ";
                std::cin >> response;
            }
        }

        for (int j = 0; j < 5; j++) {
            std::vector<char>* to_change = nullptr;
            std::vector<char>* all[5] = {&s1, &s2, &s3, &s4, &s5};

            if (j == 0) {
                to_change = &s1;
            } else if (j == 1) {
                to_change = &s2;
            } else if (j == 2) {
                to_change = &s3;
            } else if (j == 3) {
                to_change = &s4;
            } else if (j == 4) {
                to_change = &s5;
            }

            char resp_c = response[j];
            char word_c = chosen_word[j];

            if (resp_c == '=') {
                to_change->clear();
                to_change->push_back(word_c);
            } else if (resp_c == '-') {
                to_change->erase(remove(to_change->begin(), to_change->end(), word_c), to_change->end());

                for (std::vector<char>* v : all) {
                    if (v == to_change) {
                        continue;
                    }

                    v->erase(remove(v->begin(), v->end(), word_c), v->end());
                }
            } else if (resp_c == '+') {
                (*to_change).erase(remove(to_change->begin(), to_change->end(), word_c), to_change->end());

                for (std::vector<char>* v : all) {
                    if (v == to_change) {
                        continue;
                    }

                    v->push_back(word_c);
                    std::sort(v->begin(), v->end());
                    v->erase(unique(v->begin(), v->end()), v->end());
                }
            } else {
                std::cout << "Invalid input format, exiting." << std::endl;
            }
        }

        if (log_s) {
            for (char c : s1) {
                std::cout << c;
            }
            std::cout << std::endl;
            for (char c : s2) {
                std::cout << c;
            }
            std::cout << std::endl;
            for (char c : s3) {
                std::cout << c;
            }
            std::cout << std::endl;
            for (char c : s4) {
                std::cout << c;
            }
            std::cout << std::endl;
            for (char c : s5) {
                std::cout << c;
            }
            std::cout << std::endl;
        }

        all_solutions = applyRule(all_solutions, s1, s2, s3, s4, s5);

        if (all_solutions.size() == 1) {
            std::cout << "The solution is " << all_solutions[0] << "." << std::endl;
            return;
        }
    }
}

int main(int argc, const char* argv[]) {
    srand(time(0));
    bool show_sol = false;
    bool solver_on = false;
    bool show_alpha = false;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];

            if ((arg == "-h") || (arg == "--help")) {
                printHelpExit();
            } else if ((arg == "-d") || (arg == "--debug")) {
                std::cout << BOLDBLUE << "DEBUG ON" << RESET << std::endl;
                show_sol = true;
            } else if ((arg == "-s") || (arg == "--solver")) {
                std::cout << BOLDBLUE << "SOLVER ON" << RESET << std::endl;
                solver_on = true;
            } else if (arg == "--showalpha") {
                std::cout << BOLDBLUE << "SHOWING ALPHA" << RESET << std::endl;
                show_alpha = true;
            } else {
                std::cout << "Invalid argument: " << arg << std::endl;
                printHelpExit();
            }
        }
    }

    std::vector<std::string> all_solutions = readSolutions(LENGTH);
    std::vector<std::string> all_guesses = readGuesses(LENGTH);

    for (std::string s : all_solutions) {
        all_guesses.push_back(s);
    }

    auto const solution = chooseRandom(all_solutions);

    if (show_sol == true) {
        std::cout << BOLDGREEN << solution << RESET << std::endl;
    }

    if (solver_on == false) {
        play(solution, all_guesses);
    } else {
        solver(all_solutions, show_alpha);
    }

    return 0;
}
