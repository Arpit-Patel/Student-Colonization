#ifndef __USERIO_H__
#define __USERIO_H__

#include "BaseIO.h"

#include <string>
#include <vector>
#include <memory>

class EndOfFileException{};

class Board;

class UserIO : public BaseIO {
private:
    // prints i spaces
    std::string printSpace(int i); 
    // adds however number of spaces required to value so that there are FRONT 
    // total characters and then adds spaces to the back of the string equal to int back
    std::string addSpace(int front, int back, std::string value);
    // prints a citerion based on the id from the board, in format "|00|"
    std::string printCriterion(int id, std::shared_ptr<Board> b);
    // prints a goal based on the id from the board, in format "00"
    std::string printGoal(int id, std::shared_ptr<Board> b);
    // prints a resource based on the id from the board
    std::string printResource(int id, std::shared_ptr<Board> b);
    // prints "GEESE" if the current tile is a goose tile or just space if not 
    void printGoose(std::string &  output, int & gooseChar);
    // prints the value if it is != 0 and a blank if = 0
    std::string printValue(int value);
public:
    int getIntFromUser(const std::string& errorMsg) override;
    int getIntFromMessage(const std::string& msg, const std::string& errorMsg) override;
    std::string getStringFromUser() override;
    std::string getStringFromMessage(const std::string& msg) override;
    void promptInput() override;
    void sendMessage(const std::string& msg) override;
    void printBoard(const std::shared_ptr<Board> b) override;
};

#endif
