#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_

#include "Player.h"
#include "ResourceType.h"

#include <vector>
#include <string>
#include <map>

enum class ResourceType;

class PlayerManager {
    std::map<char, Player> players;
    std::map<char, std::map<ResourceType, int>> resourcesAtLastReset;
public:
    PlayerManager();
    // replaces the current players map with a new one
    void setPlayers(const std::map<char, Player> players);
    // creates a new player with the provided identifying character
    void addPlayer(const char player);
    // removes the player with the provided identifying character
    void removePlayer(const char player);
    // sets all the players to their original states
    void resetPlayers();
    // adds resources to the player
    void addResource(const char player, const ResourceType resType, const int count);
    // removes resources from the player
    void removeResource(const char player, const ResourceType resType, const int count);
    // removes the specified number of resources from the player randomly
    void removeRandomResources(const char player, const int numToRemove);
    // removes a random resource from the provided player, returning the
    // resource that was removed
    ResourceType removeRandomResource(const char player);
    // gets the number of resources with the provided resource type the player has
    int getResourceCount(const char player, const ResourceType resType);
    // gets the number of goals the player has completed
    int getNumGoals(const char player);
    // gets the number of criterions the player has completed
    int getNumCriterions(const char player);
    // increments the number of criterions the player has completed
    void addCriterion(const char player, const int id);
    // increments the number of goals the player has completed
    void addGoal(const char player, const int id);
    // adds a victory point
    void addVictoryPoint(const char player);
    // gets the number of victory points
    int getVictoryPoints(const char player);
    // returns true if the player does not have any resources
    bool hasNoResources(const char player);
    // gets a vector containing all of the criterion IDs
    std::vector<int> getGoals(const char player);
    // gets a vector containing all of the goal IDs
    std::vector<int> getCriterions(const char player);
    // makes the players dice loaded
    void loadDice(const char player, const int dieNum);
    // ranomizes the players dice
    void randomizeDice(const char player); 
    // returns true if the players dice are fair, false if they are not
    bool checkFair(const char player); 
    // gets the integer the players dice are currently loaded to
    int getLoadedInt(const char player);
    // reset tracking of much of each resource every player gains
    void resetResourceChanges();
    // get how much of each resource every player has gained since the last
    // call of resetResourceChanges
    std::map<ResourceType, int> getResourceChanges(const char player);
    // gets the name of the color with the specified character
    std::string getColorName(const char player);
};

#endif

