#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Die.h"

#include <vector>
#include <string>
#include <map>

class RandomSelecFailException{};

enum class ResourceType;

class Player {
    std::map<ResourceType, int> resources;
    std::vector<int> criterion;
    std::vector<int> goals;
    char color; 
    bool fair;
    int loadedNum;
    int victoryPoints;
public:
    Player();
    Player(const std::vector<int> criterion, const std::vector<int> goals, 
           const std::map<ResourceType,int> resources, const char color, 
           int victoryPoints);
    // adds the specified resource to the player
    void addResource(const ResourceType resType, const int count);
    // removes the specified resource from the player
    void removeResource(const ResourceType resType, const int count);
    // resets the player, removing all criterions, goals, and resources
    void reset();
    // removes the specified number of resources randomly from the player
    void removeRandomResources(int numToRemove);
    // remove a random resource from the player, and return the resource that
    // was removed
    ResourceType removeRandomResource();
    // get the total number of resources of the specified type the player has
    int getResourceCount(const ResourceType resType);
    // get the color character that represents this player
    char getColor();
    // get the number of goals the player has completed
    int getNumGoals();
    // get the number of criterions the player has completed
    int getNumCriterions();
    // add a new criterion to the player's list of completed criterions
    void addCriterion(int id);
    // add a new goal to the player's list of completed goals
    void addGoal(int id);
    // add a victory point to the player's victory point counter
    void addVictoryPoint();
    // get the number of victory points the player has
    int getVictoryPoints();
    // return true if the player has no resources
    bool hasNoResources();
    // get a list of the ids of all the criterions the player owns
    std::vector<int> getCriterions();
    // get a list of the ids of all the goals the player owns
    std::vector<int> getGoals();
    // set the player's die to be loaded to the specified number
    void loadDice(const int dieNum);
    // set the player's die to be random
    void randomizeDice(); 
    // return true if the player's die is random
    bool checkFair(); 
    // get the number the player's die is loaded to
    int getLoadedInt();  
};

#endif
