#include <string>
#include <vector>
#include <map>

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int MAX_RESOURCES_FOR_GOOSE = 10;
const int WINNING_VICTORY_POINTS = 10;
const int numP = 4;
const int numA = 8;
// invalid command messages
const std::string invalidR = "Invalid roll.";
const std::string invalidC = "Invalid command.";
const std::string invalidS = "You cannot build here.";
const std::string invalidN = "You do not have enough resources.";

// color information
const std::vector<char> DEFAULT_PLAYER_NAMES {'b', 'r', 'o', 'y'};
const std::map<char, std::string> COLOURS_BY_CHAR {
    {'b', "Blue"},
    {'y', "Yellow"},
    {'r', "Red"},
    {'o', "Orange"},
};

// general messages (mainly here so we can translate the program later if
// needed)
const std::string STUDENT = "Student ";
const std::string WHERE_TO_COMPLETE = ", where do you want to complete an Assignment?";
const std::string LOSES = " loses ";
const std::string TO_THE_GEESE = " to the geese. They lose:";
const std::string CHOSE_GEESE_POS = "Choose where to place the GEESE";
const std::string NO_STUDENTS_TO_STEAL = " has no students to steal from.";
const std::string CAN_STEAL_FROM = " can chose to steal from";
const std::string CHOSE_STUDENT_TO_STEAL = "Choose a student to steal from.";
const std::string STEALS = " steals ";
const std::string FROM_STUDENT = " from student ";
const std::string GAINED = " gained:";
const std::string NO_STUDENT_GAINED = "No students gained resources";
const std::string TURN = "'s turn.";
const std::string INPUT_ROLL = "Input a roll between 2 and 12:";
const std::string LECTURES = " lectures, ";
const std::string TUTORIALS_AND = " tutorials, and ";
const std::string STUDIES = " studies.";
const std::string HAS = " has " ;
const std::string GRADES = " grades, ";
const std::string CAFFIENES = " caffeines, ";
const std::string LABS = " labs, ";
const std::string VALID_COMMANDS = "Valid commands: ";
const std::string HAS_COMPLETED = " has completed:";
const std::string OFFERS = " offers ";
const std::string ONE = " one ";
const std::string FOR_ONE = " for one ";
const std::string DOES = "Does ";
const std::string DO_THEY_ACCEPT = " accept this offer?";
const std::string YES = "yes";
const std::string NO = "no";
const std::string PLAY_AGAIN = "Would you like to play again?";

// commands
const std::string LOAD_DICE = "load";
const std::string MAKE_DICE_FAIR = "fair";
const std::string ROLL_DICE = "roll";
const std::string PRINT_BOARD = "board";
const std::string GET_STATUS = "status";
const std::string GET_CRITERIA = "criteria";
const std::string GET_ACHIEVEMENT = "achieve";
const std::string COMPLETE_CRITERIA = "complete";
const std::string UPGRADE_CRITERIA = "improve";
const std::string TRADE = "trade";
const std::string NEXT_TURN = "next";
const std::string SAVE = "save";
const std::string HELP = "help";
const std::string SEED = "-seed";
const std::string LOAD_GAME = "-load";
const std::string LOAD_BOARD = "-board";
const std::string INFO = "info";
#endif
