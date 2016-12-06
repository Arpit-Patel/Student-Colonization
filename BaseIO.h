#ifndef __BASEIO_H__
#define __BASEIO_H__

#include <string>
#include <vector>
#include <memory>

class Board;

class BaseIO {
public:
    // gets an integer from the user, without displaying a prompt
    virtual int getIntFromUser(const std::string& errorMsg)=0;
    // gets an integer from the user, first outputting  the provided message, 
    // and then, if they do not properly input an integer, entering an invalid 
    // input message prompting them for input until they enter an integer
    virtual int getIntFromMessage(const std::string& msg, const std::string& errorMsg)=0;
    // gets a string from the user, without displaying a prompt
    virtual std::string getStringFromUser()=0;
    // gets a string from the user, prompting them to with the provided message
    virtual std::string getStringFromMessage(const std::string& msg)=0;
    // displays a prompt to the user which indicates that they should provide
    // some more input
    virtual void promptInput()=0;
    // displays the provided message to the user
    virtual void sendMessage(const std::string& msg)=0;
    // prints out a graphical representation of the provided board
    virtual void printBoard(const std::shared_ptr<Board> b)=0;
};

#endif
