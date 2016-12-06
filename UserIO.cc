#include "UserIO.h"
#include "Board.h"
#include "CriterionType.h"
#include "ResourceType.h"
#include "Constants.h"

#include <string>
#include <iostream>
#include <vector>
#include <iomanip> 
#include <sstream>
#include <memory>

using namespace std;

const string PROMPT = "> ";

int UserIO::getIntFromUser(const std::string& errorMsg) {
    string input;
    cin >> input;
    stringstream ss(input);
    int result;
    bool gotValidResult = false;
    try {
        result = stoi(input);
        gotValidResult = true;
    } catch (invalid_argument e) {
    }
    while (!gotValidResult) {
        if (cin.eof()) {
            throw EndOfFileException();
        } else if (cin.fail()) {
            cin.clear();
            cin.ignore();
        }
        cout << errorMsg << endl << PROMPT;
        cin >> input;
        try {
            result = stoi(input);
            gotValidResult = true;
        } catch (invalid_argument e) {
        }
    }
    return result;
}

int UserIO::getIntFromMessage(const string& msg, const std::string& errorMsg) {
    cout << msg << endl << PROMPT;
    return getIntFromUser(errorMsg);
}

string UserIO::getStringFromUser() {
    string input;
    cin >> input;
    while (cin.fail()) {
        if (cin.eof()) {
            throw EndOfFileException();
        }
        cin.clear();
        cin.ignore();
        cout << invalidC << endl << PROMPT;
        cin >> input;
    }
    for (unsigned int i = 0; i < input.length(); ++i) {
        input[i] = tolower(input[i]);
    }
    return input;
}

string UserIO::getStringFromMessage(const string& msg) {
    cout << msg << endl << PROMPT;
    return getStringFromUser();
}

void UserIO::promptInput() {
    cout << PROMPT;
}

void UserIO::sendMessage(const string& msg) {
    cout << msg << endl;
}

string UserIO::printSpace(int n){
    string spaces = "";
    for (int i = 0; i < n; i++){
        spaces += " ";
    }
    return spaces;
}

string UserIO::addSpace(int front, int back, string value){
    stringstream ss;
    ss << setw(front) << setfill(' ') << value;
    return ss.str() + printSpace(back);
}

string UserIO::printCriterion(int id, shared_ptr<Board> b) {
    string crit = "";
    char player = b->getCriterionPlayer(id);
    crit += player;
    if (player != '\0'){
        switch (b->getCriterionType(player, id)) {
            case CriterionType::Exam : crit += "E";
                                break;
            case CriterionType::Assignment : crit += "A";
                                break;
            case CriterionType::Midterm : crit += "M";
                                break;
        }
    } else { 
        crit = to_string(id);
    }
    return "|"+addSpace(2,0,crit)+"|";
}

string UserIO::printGoal(int id, shared_ptr<Board> b){
    string goal = "";
    char player = b->getGoalPlayer(id);
    if (player != '\0'){
        goal = player;    
    } else {
        goal = to_string(id);
    }
    return addSpace(2,0,goal);
}

string UserIO::printResource(int id, shared_ptr<Board> b){
    switch (b->getTile(id).getResourceType()){
        case ResourceType::Netflix : return "NETFLIX";
        case ResourceType::Study : return "STUDY";
        case ResourceType::Lecture : return "LECTURE";
        case ResourceType::Lab : return "LAB";
        case ResourceType::Tutorial : return "TUTORIAL";
        case ResourceType::Caffeine : return "CAFFEINE";
    }
    return "FAIL";
}        

void UserIO::printGoose(string& output , int&  gooseChar){
    string goose = "GEESE";
    if (gooseChar != -1){    
        for (int i = 4; i >= 0; i--){
            output [gooseChar--] = goose[i];
        }
    }
    gooseChar = -1;
}

string UserIO::printValue(int value){
    if (value == 0) return " ";
    return to_string(value);
}

void UserIO::printBoard(const shared_ptr<Board> b) {
    int size = b->getTilesPerBoard();
    int gooseLocation = b->getGoosePosition();
    int gooseChar = -1;
    int maxTileWidth;
    int maxTileHeight;
    int space;
    int numTiles;
    bool done = false;
    int resourceCount, locationCount, valueCount, criterionCount, goalCount;
    goalCount = criterionCount = resourceCount = locationCount = valueCount = 0;
    if (size == 19){
        maxTileWidth = 5;
        maxTileHeight = 5;  
        numTiles = 1;
        
        for (int tileHeight = 1; tileHeight <= maxTileHeight + 1; ++tileHeight){
            for (int line = 0; line < 8; ++line){
                bool hi = false;
                string output = "";
                string beginningAdd;
                beginningAdd = ""; 
                if (line == 0){
                    if (tileHeight == 2) numTiles += 2;
                    space = 5 + (15 * ((maxTileWidth - numTiles)/2));
                    output += printSpace(space);
                    for (int i = 1; i <= numTiles; ++i){
                        if (i % 2 == 0){
                            output += addSpace(9,7, printValue(b->getTile(valueCount++).getValue()));

                        } else {
                            output += printCriterion(criterionCount++,b) + "--" + printGoal(goalCount++,b); 
                            output +=  "--" + printCriterion(criterionCount++,b);                        
                        }
                    }                
                    if (tileHeight == 5) numTiles -=2;
                    if (tileHeight == maxTileHeight + 1) { 
                        done = true;
                    }        
                }    
                else if (line == 1){
    
                    space = 5 + (15 *  ((maxTileWidth - numTiles)/2));
                    if (numTiles % 4 == 3){
                        space -= 2;
                        beginningAdd = "\\";
                        hi = true;
                    }
                    output += printSpace(space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){
                        if (hi){
                            output += printSpace(16);
                            hi = false;
                        } else {
                            hi = true;
                            output += "/" + printSpace(12) + "\\";
                        }
                    }    
                    if (numTiles % 4 == 3) output += "/";
                    printGoose(output, gooseChar);
                }
                else if (line == 2 ){
                    space = 3 + (15 *  ((maxTileWidth - numTiles)/2));
                    if (numTiles % 4 == 3){
                        beginningAdd = printGoal(goalCount++,b);
                        hi = true;
                        space += 1;
                    }
                    output += printSpace(space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){
                        if (hi){
                            hi = false;
                            output +=  printSpace(13);
                        } else {
                            hi = true;
                            output += printGoal(goalCount++,b) + addSpace(8, 5, to_string(locationCount++));
                            if (locationCount - 1 == gooseLocation) gooseChar = output.size() - 5;
                            output +=  printGoal(goalCount++,b);
                        }
                    }
                    if (numTiles % 4 == 3) output += printGoal(goalCount++,b);
                    
                }
                else if (line == 3){
                    space = 3 + (15 *  ((maxTileWidth - numTiles)/2));
                    if (numTiles % 4 == 3){
                        beginningAdd = "\\";
                        hi = true;
                        space += 2;
                    }
                    output += printSpace(space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){
                        if (hi){
                            hi = false;
                            output += printSpace(12);
                        } else {
                            hi = true;
                            output += "/" + addSpace(11, 5, printResource(resourceCount++, b)) + "\\" ;
                        }
                        
                    }                
                    if (numTiles % 4 == 3) output += "/";
                }
                else if (line == 4){
                    if (tileHeight == 1) numTiles += 2;
                    space = (20 * ((maxTileWidth - numTiles)/2))  ; 
                    if (numTiles % 4 == 3) {
                        beginningAdd = printCriterion(criterionCount++,b);
                        hi = true;
                    }
                    output += printSpace(space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){
                        if (hi){
                            hi = false;
                            output += "--" + printGoal(goalCount++, b) + "--";
                        } else {
                            hi = true;
                            output += printCriterion(criterionCount++,b) + addSpace(9, 7,  printValue(b->getTile(valueCount++).getValue())) ;
                            output += printCriterion(criterionCount++,b);
                        }
                    }    
                    if (numTiles % 4 == 3) output += printCriterion(criterionCount++,b);        
                    if (tileHeight == 5) numTiles -= 2;
                }
                else if (line == 5){
                    space = 3 + (15 * ((maxTileWidth - numTiles)/2));
                    if (numTiles % 4 == 3){
                        hi = true;
                        beginningAdd = "/";
                        space += 2;
                    }
                    output += printSpace (space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){
                        if (hi){
                            hi = false;
                            output += printSpace(12);
                        } else {
                            hi = true;
                            output += "\\" + printSpace(16) + "/";
                        }
                    }
                    if (numTiles % 4 == 3) output += "\\";
                    printGoose(output, gooseChar);
                
                }
                else if (line == 6){
                    space = 3 + (15 * ((maxTileWidth - numTiles)/2));
                    if (numTiles % 4 == 3){    
                        beginningAdd = printGoal(goalCount++,b);
                        hi = true; 
                        hi -= 2;
                    }
                    output += printSpace(space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){
                        if (hi == false){
                            hi = true;
                            output += printGoal(goalCount++, b) + printSpace(13);
                            output +=  printGoal(goalCount++, b);
                        } else {
                            hi = false;
                            output += addSpace(8, 5, to_string(locationCount++));

                            if (locationCount - 1== gooseLocation) gooseChar = output.size() - 5;
                        }
                    }
                    if (numTiles % 4 == 3){
                        output += printGoal(goalCount++,b);
                    }
                }
                else{
                    space = 5 + (15 * ((maxTileWidth - numTiles)/2));
                    if (numTiles % 4 == 3){
                        beginningAdd = "/";
                        hi = true;
                        space -= 2;
                    }
                    output += printSpace (space) + beginningAdd;
                    for (int i = 1; i <= numTiles; ++i){                    
                        if (hi){
                            output += addSpace(10,6,printResource(resourceCount++,b));
                            hi = false;
                        } else {
                            hi = true;
                            output += "\\" + printSpace(12) + "/" ;
                        }
                    }
                    if (numTiles % 4 == 3){
                        output += "\\";
                    }
                            
                }
         
                cout << output  <<endl;
                if (done) break;
            }
            if (done) break;    
        }
    }
}
