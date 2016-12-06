#ifndef __TILE_H__
#define __TILE_H__

#include "ResourceType.h"
#include "Subject.h"
#include "Observer.h"
#include "Goose.h"

#include <vector>
#include <string>
#include <set>
#include <memory>

class InvalidLineException {};

class PlayerManager;

class Board;

class Tile : public Observer {
public:
    // id's must be ordered in clockwise order, where the first criterion ID
    // appears immediately before the first goal ID in the graphical
    // representation of the board
    Tile(const std::shared_ptr<PlayerManager> playerManager, 
         const std::shared_ptr<Goose> goose, 
         Board* parent, const std::vector<int> goalIds, 
         const std::vector<int> criterionIds, const ResourceType type, 
         const int value);
    void notify(int dieRoll);
    // sets the resource type of this tile
    void setResourceType(ResourceType type);
    // sets the value of this tile
    void setValue(int value);
    // gets the resource type of this tile
    ResourceType getResourceType();
    // returns true if the goal with the provided ID is in the tile
    bool containsGoal(int id);
    // returns true if the criterion with the provided ID is in the tile
    bool containsCriterion(int id);
    // returns the ID of the goal to the right of the criterion with the
    // provided ID, or throws an InvalidIDException if the criterion was not
    // found
    int getGoalToRightOfCriterion(int id);
    // returns the ID of the goal to the left of the criterion with the
    // provided ID, or throws an InvalidIDException if the criterion was not
    // found
    int getGoalToLeftOfCriterion(int id); 
    // returns the ID of the goal to the right of the goal with the provided
    // ID, or throws an InvalidIDException if the provided goal ID was not 
    // found
    int getGoalToRight(int id);
    // returns the ID of the goal to the left of the goal with the provided
    // ID, or throws an InvalidIDException if the provided goal ID was not 
    // found
    int getGoalToLeft(int id);
    // returns the ID of the criterion to the right of the goal with the
    // provided ID, or throws an InvalidIDException if the goal was not
    // found
    int getCriterionToRightOfGoal(int id);
    // returns the ID of the criterion to the left of the goal with the
    // provided ID, or throws an InvalidIDException if the goal was not
    // found
    int getCriterionToLeftOfGoal(int id);
    // returns the ID of the criterion to the right of the criterion with the 
    // provided ID, or throws an InvalidIDException if the provided goal ID 
    // was not found
    int getCriterionToRight(int id);
    // returns the ID of the criterion to the left of the criterion with the 
    // provided ID, or throws an InvalidIDException if the provided goal ID 
    // was not found
    int getCriterionToLeft(int id);
    // returns the value of the tile (i.e. the die roll required to get the
    // resources from it)
    int getValue();
    // gets all players that own a criterion on this tile
    std::set<char> getAdjascentPlayers();
private:
    std::shared_ptr<PlayerManager> playerManager;
    std::shared_ptr<Goose> goose;
    Board* parent;
    long unsigned int numGoals;
    long unsigned int numCriterions;
    std::vector<int> goalIds;
    std::vector<int> criterionIds;
    ResourceType type;
    int value;
};
#endif
