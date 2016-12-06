#ifndef __BOARD_H__
#define __BOARD_H__

#include "Tile.h"
#include "Goal.h"
#include "Subject.h"
#include "Criterion.h"
#include "PlayerManager.h"
#include "Goose.h"

#include <vector>
#include <map>
#include <utility>

enum CriterionType;

class InvalidPositionException{};
class NotEnoughResourcesException{};
class CriterionFullyUpgradedException{};
class InvalidIDException{};

class Board {
public:
    Board(std::shared_ptr<PlayerManager> playerManager, 
          const std::shared_ptr<Goose> goose, Subject& subject, 
          const int seed);
    // wipes the board and replaces it using the provided info
    void loadBoard(std::map<int, Criterion> ownedCriterions, 
                   std::map<int, Goal> ownedGoals, 
                   std::vector<std::pair<ResourceType, int>> tileInfo);
    // replaces the board tiles with the tiles in the provided vector
    void loadBoardTiles(const std::vector<std::pair<ResourceType, int>> tileInfo);
    // gets the character representing the player who has achieved the goal
    // with the provided ID, or the null character if the goal has not been
    // achieved
    char getGoalPlayer(const int id);
    // gets the character representing the player who owns the criterion
    // with the provided ID, or the null character if the criterion is
    // unclaimed
    char getCriterionPlayer(const int id);
    // completes or upgrades the criterion with the provided id for
    // the provided player. behavior is undefined if the specified player is
    // not able to upgrade the criterion for any reason
    void upgradeCriterion(const char player, const int id, const bool inSetupPhase=false);
    // completes the provided goal for the provided player. behavior is
    // undefined if the player is not able to complete the goal for any reason
    void completeGoal(const char player, const int id, const bool inSetupPhase=false);
    // returns the criterion type of the criterion with the provided id if it
    // was completed by the provided player, or criterion type 'none' if it was
    // not
    CriterionType getCriterionType(const char player, const int id);
    // gets the tile with the provided ID, or throws an
    // InvalidPositionException if the ID is invalid
    Tile getTile(const unsigned int id);
    // gets the current position of the goose
    int getGoosePosition();
    // gets the width of the board, in tiles, where the width is the length of
    // the shortest line of connected tiles that go from one side of the board
    // to the other
    virtual int getBoardWidth()=0;
    // gets the number of edges in each tile
    virtual int getEdgesPerTile()=0;
    // gets the number of tiles in each board
    virtual int getTilesPerBoard()=0;
    // ensures the criterions and goals have the player set to '\0'
    void resetCriterionsAndGoals();
protected:
    std::vector<std::shared_ptr<Tile>> tiles;
    // generates the board tiles
    std::vector<std::shared_ptr<Tile>> generateTiles(int seed);
private:
    std::vector<Goal> goals;
    std::vector<Criterion> criterions;
    std::shared_ptr<PlayerManager> playerManager;
    const std::shared_ptr<Goose> goose;
    Subject& tileSubject;
    // gets the number of tiles in the board
    virtual int getNumTiles()=0;
    // gets a map mapping each ResourceType to an integer quantity representing 
    // their rate of occurence in the board
    virtual std::map<ResourceType, int>& getResourceCounts()=0;
    // gets a vector containing all vectors of goal ID's for every tile
    virtual std::vector<std::vector<int>>& getGoalIdsByTile()=0;
    // gets a vector containing all vectors of criterionID's for every tile
    virtual std::vector<std::vector<int>>& getCriterionIdsByTile()=0;
    // gets a vector containing pairs mapping integers representing the number
    // of values in the board from a specific range to the range
    virtual std::vector<std::pair<int, std::vector<int>>>& getNumValsByRange()=0;
    // gets the total number of criterions
    virtual int getNumCriterions()=0;
    //gets the total nubmer of goals
    virtual int getNumGoals()=0;
    // returns true if the criterion has an adjascent edge owned by the provided 
    // player
    bool hasAdjascentPlayer(char player, int id);
    // returns true if the goal with the provided id can be completed by the
    // provided player, assuming they have enough resources
    bool validGoalPos(const char player, const int id);
    // returns true if the provided criterion can be completed by the provided
    // player, or false if it cannot be, assuming the player has enough
    // resources
    bool validCriterionPos(const char player, const int id, const bool requireAdjascentGoal=true);
};

#endif
