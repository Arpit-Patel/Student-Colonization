// watan game files
#include "Board.h"
#include "Goose.h"
#include "HexagonBoard.h"
#include "Criterion.h"
#include "CriterionType.h"
#include "Die.h"
#include "FileIO.h"
#include "Goal.h"
#include "Observer.h"
#include "PlayerManager.h"
#include "ResourceType.h"
#include "Subject.h"
#include "Tile.h"
#include "UserIO.h"
#include "Constants.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

class InvalidStringException {};

// statusP sends message of the status of player theP
void statusP(const char &theP, const shared_ptr <BaseIO> base, 
             shared_ptr<PlayerManager> playerManager){
    string s1, s2, s3, s4, s5, s6;
    string theC = playerManager->getColorName(theP);
    int numG = playerManager->getVictoryPoints(theP);
    int numCaf = playerManager->getResourceCount(theP, ResourceType::Caffeine);
    int numLab = playerManager->getResourceCount(theP, ResourceType::Lab);
    int numLec = playerManager->getResourceCount(theP, ResourceType::Lecture);
    int numTut = playerManager->getResourceCount(theP, ResourceType::Tutorial);
    int numStudy = playerManager->getResourceCount(theP, ResourceType::Study);
    s1 = to_string(numG);
    s2 = to_string(numCaf);
    s3 = to_string(numLab);
    s4 = to_string(numLec);
    s5 = to_string(numTut);
    s6 = to_string(numStudy);
    base->sendMessage(theC + HAS + s1 + GRADES + s2 + CAFFIENES + s3 + LABS + 
                      s4 + LECTURES + s5 + TUTORIALS_AND + s6 + STUDIES);
}

// inHelp prints the output of all valid commands.
void inHelp(shared_ptr<BaseIO> base){
    base->sendMessage(VALID_COMMANDS);
    base->sendMessage("board ");
    base->sendMessage("status ");
    base->sendMessage("criteria ");
    base->sendMessage("achieve <goal> ");
    base->sendMessage("complete <criterion> ");
    base->sendMessage("improve <criterion> ");
    base->sendMessage("trade <colour> <give> <take> ");
    base->sendMessage("next ");
    base->sendMessage("save <file> ");
    base->sendMessage("help");
    base->promptInput();
}

void inInfo(shared_ptr<BaseIO> base){
    base->sendMessage("Creators: Arpit Patel, Jacob Brunsting, Alex Yao ");
    base->sendMessage("CS246 Final Project - Students of Watan ");
    base->sendMessage("The goal of the game is to be the first of the four players to obtain 10 victory points" );
    base->sendMessage("Course Criterion - 1 VP for completion/upgrade ");
    base->sendMessage("    Assignment - 1 Caffeine 1 Lab 1 Lecture 1 Tutorial ");
    base->sendMessage("    Midterm - 2 Lecture 3 Study ");
    base->sendMessage("    Exam - 3 Caffeine, 2 Lab, 2 Lecture, 1 Tutorial, 2 Study ");
    base->sendMessage("Goal Achievement - allow for adjacent criterion to be completed ");
    base->sendMessage("    1 Study 1 Tutorial ");
    base->sendMessage("Geese - A tile can be overrun by geese ");
    base->sendMessage("    no resources are obtained when the value of that tile is rolled");
    
}

// converts the provided string to lower case
void convertToLower(string& str) {
    for (unsigned int i = 0; i < str.size(); ++i) {
        str[i] = tolower(str[i]);
    }
}

// toInt checks if conversion of string to integer is true, if true returns 
// true and changes value of n to the converted string if false returns false 
// and sends error message based on wether roll is true.
bool toInt(const string &s, shared_ptr<BaseIO> base, int &n, bool roll){
    try{
        n = stoi(s);
    }
    catch(invalid_argument){
        roll ? base->sendMessage(invalidR) : base->sendMessage(invalidC);
        base->promptInput();
        return false;
    }
    return true;
}

// gets string type of CrtiterionType.
int getCritTypeInt(CriterionType c){
    if (c == CriterionType::Assignment) return 1;
    else if (c == CriterionType::Midterm) return 2;
    else if (c == CriterionType::Exam) return 3;
    else if (c == CriterionType::None) return 0;
    throw InvalidStringException();
}

// gets ResourceType from string.
ResourceType getResType(string s) {
    convertToLower(s);
    if (s == "netflix"){
        return ResourceType::Netflix;
    }
    else if (s == "caffeine"){
        return ResourceType::Caffeine;
    }
    else if (s == "lab"){
        return ResourceType::Lab;
    }
    else if (s == "study"){
        return ResourceType::Study;
    }
    else if (s == "lecture"){
        return ResourceType::Lecture;
    }
    else if (s == "tutorial"){
        return ResourceType::Tutorial;
    }
    throw InvalidStringException();
}

// gets ResourceType from string.
string getResString(ResourceType resourceString) {
    switch(resourceString) {
        case ResourceType::Netflix:  return "NETFLIX";
        case ResourceType::Caffeine: return "CAFFEINE";
        case ResourceType::Lab:      return "LAB";
        case ResourceType::Study:    return "STUDY";
        case ResourceType::Lecture:  return "LECTURE";
        case ResourceType::Tutorial: return "TUTORIAL";
    }
    throw InvalidStringException();
}

// based on string gets player id.
char getPlayerChar(string str){
    convertToLower(str);
    if (str == "blue" || str == "b"){
        return 'b';
    }
    else if (str == "orange" || str == "o"){
        return 'o';
    }
    else if (str == "red" || str == "r"){
        return 'r';
    }
    else if (str == "yellow" || str == "y"){
        return 'y';
    }
    throw InvalidStringException();
}

// checks if string is valid player.
bool validPlayer(string s){
    try{
        getPlayerChar(s);
    return true;
    }
    catch (InvalidStringException& e){
        return false;
    }
}

// checks if string is valid ResourceType.
bool validResType(string s){
    try{
        getResType(s);
        return true;
    }
    catch (InvalidStringException& e){
        return false;
    }
}

// checks if three string input.
bool validTradeFields(string s1, string s2, string s3){
    if (validPlayer(s1) && validResType(s2) && validResType(s3)) return true;
    return false;
}

// runs start of game assignments for players.
void startGame(const shared_ptr <BaseIO> base, shared_ptr <Board> b, 
               const int (&assignOrder)[numA], shared_ptr<PlayerManager> playerManager, 
               vector <char> players){
    string in;
    for(int i = 0; i < numA; ++i){
        int curA = assignOrder[i];
        char curP = players[curA];
        string curC = playerManager->getColorName(curP);
        while (true) {
            int id = base->getIntFromMessage(STUDENT + curC + WHERE_TO_COMPLETE, invalidC);
            try {
                b->upgradeCriterion(curP, id, true);
                playerManager->addVictoryPoint(curP);
                break;
            } catch(InvalidPositionException){
                base->sendMessage(invalidS);
            }
        }
    }
}

// called when the goose is supposed to move, allows the current player to 
// steal from other players and specify the goose position
void onGooseMove(const shared_ptr<Board> b, shared_ptr<Goose> goose, 
                 std::shared_ptr<PlayerManager> playerManager, 
                 const vector<char> players, const shared_ptr<BaseIO> io, 
                 const char curPlayer){
    playerManager->resetResourceChanges();
    for (char player : players){
        int totalResources = 0;
        for (ResourceType type : RESOURCE_VECTOR){
            totalResources += playerManager->getResourceCount(player, type);
        }
        if (totalResources >= MAX_RESOURCES_FOR_GOOSE){
            int resourcesToRemove = totalResources - totalResources / 2;
            playerManager->removeRandomResources(player, resourcesToRemove);
            io->sendMessage(STUDENT + playerManager->getColorName(player) + 
            LOSES + to_string(resourcesToRemove) + TO_THE_GEESE);
            for (auto resourceCountChange : playerManager->getResourceChanges(player)) {
                if (resourceCountChange.second == 0) {
                    continue;
                }
                io->sendMessage(to_string(-1 * resourceCountChange.second) + " " + 
                                getResString(resourceCountChange.first));
            }
        }
    }
    int newPos = io->getIntFromMessage(CHOSE_GEESE_POS, invalidC);
    while (newPos < 0 || newPos >= b->getTilesPerBoard() || newPos == goose->getPosition()) {
        newPos = io->getIntFromMessage(invalidC, invalidC);
    }
    goose->setPosition(newPos);
    set<char> playersToSteal = b->getTile(newPos).getAdjascentPlayers();
    for (auto it = playersToSteal.begin(); it != playersToSteal.end(); ++it) {
        if (playerManager->hasNoResources(*it) || *it == curPlayer) {
            playersToSteal.erase(it);
        }
    }
    if (playersToSteal.size() == 0){
        io->sendMessage(STUDENT + playerManager->getColorName(curPlayer) + NO_STUDENTS_TO_STEAL);
        return;
    }
    stringstream stealFromPlayers;
    stealFromPlayers << STUDENT << playerManager->getColorName(curPlayer) << CAN_STEAL_FROM;
    bool first = true;
    for (char player : playersToSteal){
        if (!first){
            stealFromPlayers << ",";
        }
        else{
            first = false;
        }
        stealFromPlayers << " " << playerManager->getColorName(player);
    }
    stealFromPlayers << ".";
    io->sendMessage(stealFromPlayers.str());
    string playerToSteal;
    playerToSteal = io->getStringFromMessage(CHOSE_STUDENT_TO_STEAL);
    while (!validPlayer(playerToSteal)){
        io->sendMessage(invalidC);
        playerToSteal = io->getStringFromMessage(CHOSE_STUDENT_TO_STEAL);
    }
    char charPlayerToSteal = getPlayerChar(playerToSteal);
    ResourceType stolenRes = playerManager->removeRandomResource(charPlayerToSteal);
    io->sendMessage(STUDENT + playerManager->getColorName(curPlayer) + STEALS + 
                    getResString(stolenRes) + FROM_STUDENT + playerManager->getColorName(charPlayerToSteal));
}

// called after a regular die roll, displays the the number of resources each
// player has gained/lost
void onRollComplete(shared_ptr<PlayerManager> playerManager, 
                    const vector<char> players, const shared_ptr<BaseIO> io, 
                    const char curStudent){
    bool allPlayersNoChange = true;
    for (char playerChar : players){
        bool firstLinePrinted = false;
        for (auto resourceChange : playerManager->getResourceChanges(playerChar)){
            if (resourceChange.second != 0){
                if (!firstLinePrinted){
                    firstLinePrinted = true;
                    allPlayersNoChange = false;
                    io->sendMessage(STUDENT + playerManager->getColorName(playerChar) + GAINED);
                }
                io->sendMessage(to_string(resourceChange.second) + " " + getResString(resourceChange.first));
            }
        }
    }
    if (allPlayersNoChange){
        io->sendMessage(NO_STUDENT_GAINED);
    }
}

// takes commands for the first half of the turn
void playTurnStart(const shared_ptr<Board> b, const shared_ptr<BaseIO> base, 
                   const shared_ptr<PlayerManager> playerManager, 
                   const shared_ptr<Goose> goose, Die d, 
                   const vector<char> players, const char curPlayer){
    while (true){
        string in = base->getStringFromUser();
        // if user inpurs load user is prompted to input an integer to set load dice
        if (in == LOAD_DICE){
            base->sendMessage(INPUT_ROLL);
            base->promptInput();
            int dieNum;
            while (true){
                dieNum = base->getIntFromUser(invalidR);
                if (dieNum <= 12 && dieNum >= 2) {
                    break;
                }
                base->sendMessage(invalidR);
                base->promptInput();
            }
            playerManager->loadDice(curPlayer, dieNum);
            base->promptInput();
        }
        // sets die to fair if upon players input
            else if(in == MAKE_DICE_FAIR){
            playerManager->randomizeDice(curPlayer);
            base->promptInput();
        }
        // rolls dice upon player input and resources are gained based on dice 
        // roll to all players
        else if(in == ROLL_DICE){
            playerManager->resetResourceChanges();
            playerManager->checkFair(curPlayer) ? d.rollRandom()
                                                : d.rollLoaded(playerManager->getLoadedInt(curPlayer));
            if (d.getLastRoll() == goose->getDieRollForMove()){
                onGooseMove(b, goose, playerManager, players, base, curPlayer);
            }
            else{
                onRollComplete(playerManager, players, base, curPlayer);
            }
            base->promptInput();
            return;
        }
        // if input is invalid invalid command will be printed
        else{
            base->sendMessage(invalidC);
            base->promptInput();
        }
    }
}

// takes commands for the second half of the turn, returns true if the player wins
bool playTurnEnd(const shared_ptr<Board> b, const shared_ptr<BaseIO> base, 
                 const shared_ptr<PlayerManager> playerManager, int turnNumber, 
                 const vector<char> players, const char curPlayer){
    FileIO file;
    while (true){
        string in = base->getStringFromUser();
        // prints board upon user input for board
        if (in == PRINT_BOARD){
            base->printBoard(b);
            base->promptInput();
        }
        else if (in == INFO){
            inInfo(base);
            base->promptInput();
        }
        // prints status of all players
        else if(in == GET_STATUS){
            for (int i = 0; i < numP; ++i){
                statusP(players[i], base, playerManager);
            }
            base->promptInput();
        }
        // prints criterion completed my current player
        else if(in == GET_CRITERIA){
            base->sendMessage(playerManager->getColorName(curPlayer) + HAS_COMPLETED);
            const auto crits = playerManager->getCriterions(curPlayer);
            const int numCrit = playerManager->getNumCriterions(curPlayer);
            for (int id = 0; id < numCrit; ++id){
                const string stringInt = to_string(crits[id]);
                const int critTypeInt = getCritTypeInt(b->getCriterionType(curPlayer, crits[id]));
                if (critTypeInt == 0 ) {
                    continue;
                }
                string s1 = to_string(critTypeInt);
                base->sendMessage(stringInt + " " + s1);
            }
            base->promptInput();
        }
        // completes goal upon users input
        else if(in == GET_ACHIEVEMENT){
            int n = base->getIntFromUser(invalidC);
            try{
                b->completeGoal(curPlayer, n);
            }
            catch(InvalidPositionException){
                base->sendMessage(invalidS);
            }
            catch(NotEnoughResourcesException){
                base->sendMessage(invalidN);
            }
            base->promptInput();
        }
        // upgrades criterion upon user input and adds victory point if successful
        else if(in == UPGRADE_CRITERIA){
            int n = base->getIntFromUser(invalidC);
            try{
                if (b->getCriterionPlayer(n) == curPlayer){
                    b->upgradeCriterion(curPlayer, n, false);
                    playerManager->addVictoryPoint(curPlayer);
                }
                else{
                    base->sendMessage(invalidS);                             
                }
            }
            catch (InvalidIDException& e){
                base->sendMessage(invalidS);
            }
            catch(InvalidPositionException){
                base->sendMessage(invalidS);
            }
            catch(NotEnoughResourcesException){
                base->sendMessage(invalidN);
            }
            base->promptInput();
        }
        // completes criterion upon user input
        else if (in == COMPLETE_CRITERIA){
            int n = base->getIntFromUser(invalidC);
            try{
                if (b->getCriterionPlayer(n) == '\0'){
                    b->upgradeCriterion(curPlayer, n, false);
                    playerManager->addVictoryPoint(curPlayer);
                }
                else{
                    base->sendMessage(invalidS);                      
                }
            }
            catch (InvalidIDException& e){
                base->sendMessage(invalidS);
            }
            catch(InvalidPositionException){
                base->sendMessage(invalidS);
            }
            catch(NotEnoughResourcesException){
                base->sendMessage(invalidN);
            }
            base->promptInput();
        }
        // intialtes trade of resources upon users input
        else if(in == TRADE){
            string tradePlayerStr = base->getStringFromUser();
            string resourceSendStr = base->getStringFromUser();
            string resourceGetStr = base->getStringFromUser();
            if (validTradeFields(tradePlayerStr, resourceSendStr, resourceGetStr)){
                ResourceType resourceSend;
                ResourceType resourceGet;
                int rTrade = 0;
                int rGet = 0;
                char tradePlayer = getPlayerChar(tradePlayerStr);
                while (true){
                    try{
                        resourceSend = getResType(resourceSendStr);
                        resourceGet = getResType(resourceGetStr);
                        rTrade = playerManager->getResourceCount(curPlayer, resourceSend);
                        rGet = playerManager->getResourceCount(tradePlayer, resourceGet);
                        break;
                    }
                    catch (InvalidStringException& e){
                        base->sendMessage(invalidC);
                        base->promptInput();
                    }
                }
                if (rTrade != 0 && rGet != 0){
                    base->sendMessage(playerManager->getColorName(curPlayer) + OFFERS + 
                                      playerManager->getColorName(tradePlayer) + ONE + 
                                      getResString(resourceSend) + FOR_ONE + 
                                      getResString(resourceGet) + ".");
                    base->sendMessage(DOES + tradePlayerStr + DO_THEY_ACCEPT);
                    base->promptInput();
                    while (true){
                        in = base->getStringFromUser();
                        if (in == YES || in == "y"){
                            playerManager->removeResource(curPlayer, resourceSend, 1);
                            playerManager->removeResource(tradePlayer, resourceGet, 1);
                            playerManager->addResource(curPlayer, resourceGet, 1);
                            playerManager->addResource(tradePlayer, resourceSend, 1);
                            base->promptInput();
                            break;
                        }
                        else if (in == NO || in == "n"){
                            base->promptInput();
                            break;
                        }
                        else{
                            base->sendMessage(invalidC);
                            base->promptInput();
                        }
                    }
                }
                else{
                    base->sendMessage(invalidN);
                    base->promptInput();
                }
            }
            else{
                base->sendMessage(invalidC);
                base->promptInput();
            }
        }
        // initiates next players turn upon user input
        else if(in == NEXT_TURN){
            return false;
        }
        // saves file with cur game state upon user input
        else if(in == SAVE){
            in = base->getStringFromUser();
            file.save(b, playerManager, turnNumber, in);
            base->promptInput();
        }
        // prints valid commands if input is "help".
        else if(in == HELP){
            inHelp(base);
        }
        // prints invalid input if user inputs invalid command.
        else{
            base->sendMessage(invalidC);
            base->promptInput();
        }
        // if players victory points are greater than or equal to 10 at the end 
        // of an upgrade game ends with with the winner being current player.
        if (playerManager->getVictoryPoints(curPlayer) >= WINNING_VICTORY_POINTS){
            return true;
        }
    }
}

// playGame handles game play of settlers of watan.
void playGame(const shared_ptr<Board> b, const shared_ptr<BaseIO> base, 
              const shared_ptr<PlayerManager> playerManager, 
              const shared_ptr<Goose> goose, Die d, 
              const vector<char> players, 
              const int startingPlayerPos, const bool skipSetupAndFirstTurn){
    
    int assignOrder[numA] = {0, 1, 2, 3, 3, 2, 1, 0};
    int pTrack = startingPlayerPos;
    
    // turns
    try {
        // start of game
        if (skipSetupAndFirstTurn) {
            if (pTrack == 3){
                pTrack = 0;
            }
            else{
                ++pTrack;
            }
        } else {
            startGame(base, b, assignOrder, playerManager, players);
            base->printBoard(b);
        }
        
        while (1){
            char curPlayer = players.at(pTrack);
            base->sendMessage(STUDENT + playerManager->getColorName(curPlayer) + TURN);
            statusP(curPlayer, base, playerManager);
            base->promptInput();
            playTurnStart(b, base, playerManager, goose, d, players, curPlayer);
            if (playTurnEnd(b, base, playerManager, pTrack, players, curPlayer)) {
                break;
            }
            
            // go to the next player
            if (pTrack == 3){
                pTrack = 0;
            }
            else{
                ++pTrack;
            }
        }
        // End of game
        string in = base->getStringFromMessage(PLAY_AGAIN);
        while(true) {
            if (in == YES || in == "y"){
                pTrack = 0;
                playerManager->resetPlayers();
                b->resetCriterionsAndGoals();
                playGame(b, base, playerManager, goose, d, players, 0, false);
                break;
            }
            else if(in == NO || in == "n") {
                return;
            } else{
                in = base->getStringFromMessage(invalidC);
            }
        }
    } catch (EndOfFileException& e) {
        FileIO file;
        file.save(b, playerManager, pTrack, "backup.sv");
    }
}

int main(int argc, char *argv[]){
    string first, second;
    if (argc == 3){
        first = argv[1];
        second = argv[2];
    }
    shared_ptr<BaseIO> base(new UserIO());
    Die d;
    shared_ptr<PlayerManager> playerManager(new PlayerManager());
    shared_ptr <Goose> goose(new Goose());
    playerManager->addPlayer(DEFAULT_PLAYER_NAMES.at(0));
    playerManager->addPlayer(DEFAULT_PLAYER_NAMES.at(1));
    playerManager->addPlayer(DEFAULT_PLAYER_NAMES.at(2));
    playerManager->addPlayer(DEFAULT_PLAYER_NAMES.at(3));
    FileIO loadFile;
    int in;
    int startingPlayerPos = 0;
    if (first == SEED){
        in = stoi(second);
    }
    else{
        in = 0;
    }
    shared_ptr <Board> b (new HexagonBoard(playerManager, goose, d, in));
    bool skipSetupAndRoll = false;
    if (first == LOAD_GAME){
        loadFile.load(second, b, playerManager, startingPlayerPos, DEFAULT_PLAYER_NAMES);
    skipSetupAndRoll = true;
    }
    else if (first == LOAD_BOARD){
        loadFile.loadBoard(second, b);
    }
    playGame(b, base, playerManager, goose, d, DEFAULT_PLAYER_NAMES, startingPlayerPos, skipSetupAndRoll);
}
