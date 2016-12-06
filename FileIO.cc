#include "FileIO.h"
#include "PlayerManager.h"
#include "BaseIO.h"
#include "Board.h"
#include "UserIO.h"
#include "ResourceType.h"
#include "Criterion.h"
#include "CriterionType.h"
#include "Goal.h"
#include "Player.h"
#include "Constants.h"

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iterator>
#include <map>
#include <utility>
#include <sstream>
#include <iostream>

using namespace std;

class InvalidResourceException {};

int convertCriterionTypeToInt(CriterionType type){
    switch(type){
        case CriterionType::Assignment : return 1;
        case CriterionType::Exam : return 3;
        case CriterionType::Midterm : return 2;
    }
    return 0;
}

int convertResourceTypeToInt(ResourceType type) {
    switch (type) {
    case ResourceType::Caffeine: return 0;
    case ResourceType::Lab:      return 1;
    case ResourceType::Lecture:  return 2;
    case ResourceType::Study:    return 3;
    case ResourceType::Tutorial: return 4;
    case ResourceType::Netflix:  return 5;
    }
    throw InvalidResourceException();
}

ResourceType convertIntToResourceType(int type) {
    switch (type) {
    case 0: return ResourceType::Caffeine;
    case 1: return ResourceType::Lab;
    case 2: return ResourceType::Lecture;
    case 3: return ResourceType::Study;
    case 4: return ResourceType::Tutorial;
    case 5: return ResourceType::Netflix;
    }
    throw InvalidResourceException();
}

CriterionType convertIntToCriterionType(int type) {
    switch (type) {
    case 1: return CriterionType::Assignment;
    case 2: return CriterionType::Midterm;
    case 3: return CriterionType::Exam;
    }
    return CriterionType::None;
}

// sendPlayerData sends message of each player's data.
void sendPlayerData(shared_ptr<PlayerManager> p, shared_ptr<Board> b, ofstream &file){
    for (unsigned int i = 0; i < numP; ++i){
        char curP = DEFAULT_PLAYER_NAMES[i];
        int numCaf = p->getResourceCount(curP, ResourceType::Caffeine);
        file << numCaf;
        file << " ";
        int numLab = p->getResourceCount(curP, ResourceType::Lab);
        file << numLab;
        file << " ";
        int numLec = p->getResourceCount(curP, ResourceType::Lecture);
        file << numLec;
        file << " ";
        int numStudy = p->getResourceCount(curP, ResourceType::Study);
        file << numStudy;
        file << " ";
        int numTut = p->getResourceCount(curP, ResourceType::Tutorial);
        file << numTut;
        file << " g ";
        vector <int> goals = p->getGoals(curP);
        ostream_iterator <int> output_goals{file, " "};
        copy(goals.begin(), goals.end(), output_goals);
        file << "c ";
        vector <int> crits = p->getCriterions(curP);
        for (unsigned int crit = 0; crit < crits.size(); ++crit){
            file << crits[crit];
            file << " ";
            CriterionType c = b->getCriterionType(curP, crits[crit]);
            file << convertCriterionTypeToInt(c);
            file << " ";
        }
        file << endl;
    }
}

void FileIO::save(const shared_ptr<Board> b, 
                  const shared_ptr<PlayerManager> playerManager, 
                  const int curP, const string &fileName){
    ofstream file{fileName};
    file << curP;
    file << endl;
    sendPlayerData(playerManager, b, file);
    int size = b->getTilesPerBoard();
    int resource;
    for (int i = 0; i < size; ++i){
        resource = convertResourceTypeToInt(b->getTile(i).getResourceType());
        file << resource;
        file << " ";
        file << b->getTile(i).getValue();
        file << " ";
    }
    file.close();
}

void FileIO::loadBoard(const string& saveFile, std::shared_ptr<Board> board) {
    ifstream file{saveFile};
    vector<pair<ResourceType, int>> tileInfo;
    string line;
    getline(file, line);
    stringstream liness(line);
    string word;
    while (liness >> word) {
        ResourceType res = convertIntToResourceType(stoi(word));
        liness >> word;
        int val = stoi(word);
        tileInfo.emplace_back(pair<ResourceType, int>(res, val));
    }
    board->loadBoardTiles(tileInfo);
}

void FileIO::load(const string& saveFile, const shared_ptr<Board> board, 
                  const shared_ptr<PlayerManager> playerManager, 
                  int& startingPlayerPos, const vector <char> &playerChars){
    ifstream file{saveFile};
    string line;
    
    map<int, Criterion> ownedCriterions;
    map<int, Goal> ownedGoals;
    
    // get the player whos turn it is, and assign it through reference
    getline(file, line);
    startingPlayerPos = stoi(line);
    
    // read in all the players information
    map<char, Player> players;
    for (int playerId = 0; playerId < numP; ++playerId) {
        int resourceBeingRead = 0;
        const char playerChar = playerChars.at(playerId);
        vector<int> crits;
        vector<int> goals;
        bool readingGoals = false;
        bool readingCriterions = false;
        map<ResourceType, int> playerResources;
        int victoryPoints = 0;

        getline(file, line);
        stringstream liness(line);
        string word;
        while (liness >> word) {
            if (word == "g"){
                readingGoals = true;
                readingCriterions = false;
            } else if(word == "c") {
                readingGoals = false;
                readingCriterions = true;
            } else {
                int numIn = stoi(word);
                if (readingCriterions) {
                    // insert the criterion id into the list
                    crits.push_back(numIn);
                    liness >> word;
                    // get the integer representing the criterion type
                    const int criterionTypeInt = stoi(word);
                    victoryPoints += criterionTypeInt;
                    // get the actual type of the criterion
                    const CriterionType type = convertIntToCriterionType(criterionTypeInt);
                    // create a new criterion object, and store it in a list
                    // with the rest of the criterions the player owns
                    ownedCriterions[numIn] = Criterion{type, playerChar};
                }
                else if(readingGoals){
                    goals.push_back(numIn);
                    ownedGoals[numIn] = Goal{playerChar};
                }
                else{
                    playerResources[convertIntToResourceType(resourceBeingRead++)] = numIn;
                }
            }
        }
        players[playerChar] = Player(crits, goals, playerResources, playerChar, victoryPoints);
    }
    playerManager->setPlayers(players);
    
    // read in the board
    vector<pair<ResourceType, int>> tileInfo;
    getline(file, line);
    stringstream liness(line);
    string word;
    while (liness >> word) {
        ResourceType res = convertIntToResourceType(stoi(word));
        liness >> word;
        int val = stoi(word);
        tileInfo.emplace_back(pair<ResourceType, int>(res, val));
    }
    board->loadBoard(ownedCriterions, ownedGoals, tileInfo);
}
