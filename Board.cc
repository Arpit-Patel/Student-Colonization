#include "Board.h"
#include "Tile.h"
#include "Goal.h"
#include "Subject.h"
#include "Goose.h"
#include "Observer.h"
#include "Criterion.h"
#include "CriterionType.h"
#include "ResourceType.h"
#include "PlayerManager.h"

#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <utility>

const CriterionType fullyCompletedType = CriterionType::Exam;

std::map<CriterionType, std::map<ResourceType, int>> criterionUpgradeCosts
{
    {CriterionType::Assignment,
        std::map<ResourceType, int>
        {
            {ResourceType::Tutorial,  1},
            {ResourceType::Study,     0},
            {ResourceType::Caffeine,  1},
            {ResourceType::Lab,       1},
            {ResourceType::Lecture,   1}
        }
    },
    {CriterionType::Midterm,
        std::map<ResourceType, int>
        {
            {ResourceType::Tutorial,  0},
            {ResourceType::Study,     3},
            {ResourceType::Caffeine,  0},
            {ResourceType::Lab,       0},
            {ResourceType::Lecture,   2}
        }
    },
    {CriterionType::Exam,
        std::map<ResourceType, int>
        {
            {ResourceType::Tutorial,  1},
            {ResourceType::Study,     2},
            {ResourceType::Caffeine,  3},
            {ResourceType::Lab,       2},
            {ResourceType::Lecture,   2}
        }
    },
};

std::map<ResourceType, int> goalCost
{
    {ResourceType::Tutorial, 1},
    {ResourceType::Study,     1},
    {ResourceType::Caffeine,  0},
    {ResourceType::Lab,       0},
    {ResourceType::Lecture,   0}
};

void Board::resetCriterionsAndGoals() {
    for (unsigned int i = 0; i < goals.size(); ++i) {
        goals.at(i).player = '\0';
    }
    for (unsigned int i = 0; i < criterions.size(); ++i) {
        criterions.at(i).player = '\0';
        criterions.at(i).typeCrit = CriterionType::None;
    }
}

Board::Board(std::shared_ptr<PlayerManager> playerManager, 
             const std::shared_ptr<Goose> goose, Subject& tileSubject, 
             const int seed):
        playerManager{playerManager},
        goose{goose},
        tileSubject{tileSubject} {
}
    
// returns true if the player has enough resources to pay the specified cost,
// or false if they do not
bool canPlayerPay(const std::shared_ptr<PlayerManager> playerManager, 
                  const char player, const std::map<ResourceType, int> cost) {
    for (auto const& pair : cost) {
        if (playerManager->getResourceCount(player, pair.first) < pair.second) {
            return false;
        }
    }
    return true;
}

// deducts the amount of resources from the provided player required to pay
// the provided cost
void pay(std::shared_ptr<PlayerManager> playerManager, char player, std::map<ResourceType, int> cost) {
    for (auto const& pair : cost) {
        playerManager->removeResource(player, pair.first, pair.second);
    }
}

// returns the next criterion in the criterion upgrade list
CriterionType getNextCriterionType(CriterionType current) {
    if (current == CriterionType::None) {
        return CriterionType::Assignment;
    } else if (current == CriterionType::Assignment) {
        return CriterionType::Midterm;
    } else if (current == CriterionType::Midterm) {
        return CriterionType::Exam;
    }
    return CriterionType::None;
}

void Board::loadBoard(std::map<int, Criterion> ownedCriterions, 
                      std::map<int, Goal> ownedGoals, 
                      std::vector<std::pair<ResourceType, int>> tileInfo) {
    resetCriterionsAndGoals();
    for (auto it = ownedCriterions.begin(); it != ownedCriterions.end(); ++it) {
        criterions.at(it->first) = it->second;
    }
    for (auto it = ownedGoals.begin(); it != ownedGoals.end(); ++it) {
        goals.at(it->first) = it->second;
    }
    for (unsigned int i = 0; i < tileInfo.size(); ++i) {
        tiles.at(i)->setResourceType(tileInfo.at(i).first);
        tiles.at(i)->setValue(tileInfo.at(i).second);
    }
}

void Board::loadBoardTiles(std::vector<std::pair<ResourceType, int>> tileInfo) {
    for (unsigned int i = 0; i < tileInfo.size(); ++i) {
        tiles.at(i)->setResourceType(tileInfo.at(i).first);
        tiles.at(i)->setValue(tileInfo.at(i).second);
    }
}

char Board::getGoalPlayer(int id) {
    if (id >= 0 && id < goals.size()) {
        return goals.at(id).player;
    } else {
        throw InvalidIDException();
    }
}

char Board::getCriterionPlayer(const int id) {
    if (id >= 0 && id < criterions.size()) {
        return criterions.at(id).player;
    } else {
        throw InvalidIDException();
    }
}

void Board::upgradeCriterion(const char player, const int id, 
                             const bool inSetupPhase) {
    if (0 > id || id >= criterions.size()) {
        throw InvalidPositionException();
    }
    Criterion& curCrit = criterions.at(id);
    const CriterionType nextType = getNextCriterionType(curCrit.typeCrit);
    if (!canPlayerPay(playerManager, player, criterionUpgradeCosts[nextType]) && !inSetupPhase) {
        throw NotEnoughResourcesException();
    } else if (nextType == CriterionType::None) {
        throw InvalidPositionException();
    } else if (!validCriterionPos(player, id)) {
        throw InvalidPositionException();
    } else {
        if (!inSetupPhase) {
            pay(playerManager, player, criterionUpgradeCosts[nextType]);
        }
        curCrit.typeCrit = nextType;
        if (curCrit.player == '\0') {
            curCrit.player = player;
            playerManager->addCriterion(player, id);
        }
    }
}

bool Board::validCriterionPos(const char player, const int id, 
                              const bool inSetupPhase) {
    if (criterions.at(id).player != '\0' && criterions.at(id).player != player) {
        return false;
    }
    if (criterions.at(id).player == player) {
        if (getNextCriterionType(criterions.at(id).typeCrit) == CriterionType::None) {
            return false;
        } else {
            return true;
        }
    }
    bool foundAdjascentGoal = false;
    // check to make sure there are no adjascent criterions solved, and check
    // if there is at least one goal solved
    for (std::shared_ptr<Tile> tile : tiles) {
        if (tile->containsCriterion(id)) {      
            Criterion& rightCrit = criterions.at(tile->getCriterionToRight(id));
            Criterion& leftCrit = criterions.at(tile->getCriterionToLeft(id));
            if (rightCrit.typeCrit != CriterionType::None ||
                leftCrit.typeCrit != CriterionType::None) {
                return false;
            }
                
            Goal& rightGoal = goals.at(tile->getGoalToRightOfCriterion(id));
            Goal& leftGoal = goals.at(tile->getGoalToLeftOfCriterion(id));
            if (rightGoal.player == player || 
                leftGoal.player == player) {
                foundAdjascentGoal = true;
            }
        }
    }
    return foundAdjascentGoal || inSetupPhase;
}

void Board::completeGoal(const char player, const int id, 
                         const bool inSetupPhase) {
    if (0 > id || id >= goals.size()) {
        throw InvalidPositionException();
    }
    if (!canPlayerPay(playerManager, player, goalCost) && !inSetupPhase) {
        throw NotEnoughResourcesException();
    } else if (!validGoalPos(player, id)) {
        throw InvalidPositionException();
    }
    if (!inSetupPhase) {
        pay(playerManager, player, goalCost);
    }
    goals.at(id).player = player;
    playerManager->addGoal(player, id);
}

// returns true if the criterion has an adjascent edge owned by the provided 
// player
bool Board::hasAdjascentPlayer(char player, int id) {
    for (std::shared_ptr<Tile> tile : tiles) {
        if (tile->containsCriterion(id)) {
            const int rightGoalID = tile->getGoalToRightOfCriterion(id);
            const int leftGoalID = tile->getGoalToLeftOfCriterion(id);
            if (goals.at(rightGoalID).player == player ||
                goals.at(leftGoalID).player == player) {
                return true;
            }
        }
    }
    return false;
}

bool Board::validGoalPos(const char player, const int id) {
    if (goals.at(id).player != '\0') {
        return false;
    }
    for (std::shared_ptr<Tile> tile : tiles) {
        if (tile->containsGoal(id)) {
            const int rightCritID = tile->getCriterionToRightOfGoal(id);
            const int leftCritID = tile->getCriterionToLeftOfGoal(id);
                
            Criterion rightCrit = criterions.at(rightCritID);
            Criterion leftCrit = criterions.at(leftCritID);
            if ((rightCrit.player == player && rightCrit.typeCrit != CriterionType::None) ||
                (leftCrit.player == player && leftCrit.typeCrit != CriterionType::None)) {
                return true;
            }
            if (hasAdjascentPlayer(player, rightCritID) || 
                hasAdjascentPlayer(player, leftCritID)) {
                return true;
            }
            // we only need to scan one tile to determine if the position is
            // valid, so we return if the first tile containing the goal does
            // not find any adjascent edges or criterions
            return false;
        }
    }
    return false;
}

CriterionType Board::getCriterionType(char player, int id) {
    const Criterion& criterion = criterions.at(id);
    if (criterion.player == player) {
        return criterion.typeCrit;
    }
    return CriterionType::None;
}

Tile Board::getTile(unsigned int id) {
    if (id >= tiles.size()) {
        throw InvalidIDException();
    } else {
        return *tiles.at(id);
    }
}

int Board::getGoosePosition() {
    return goose->getPosition();
}

std::vector<std::shared_ptr<Tile>> Board::generateTiles(int seed) {
    criterions.clear();
    goals.clear();
    for (int i = 0; i < getNumCriterions(); ++i) {
        criterions.emplace(criterions.begin(), Criterion{CriterionType::None, '\0'});
    }
    for (int i = 0; i < getNumGoals(); ++i) {
        goals.emplace(goals.begin(), Goal{'\0'});
    }
    std::srand(seed);
    // create a vector containing all possible resource types for the tiles in
    // the quantities specified
    std::vector<ResourceType> possibleResources;
    for (auto const& pair : getResourceCounts()) {
        for (int i = 0; i < pair.second; ++i) {
            possibleResources.emplace_back(pair.first);
        }
    }

    // create a vector containing all tile values for the board
    std::vector<int> tileValues;
    for (auto const& numAndRange : getNumValsByRange()) {
        for (int i = 0; i < numAndRange.first; ++i) {
            int index = rand() % numAndRange.second.size();
            tileValues.emplace_back(numAndRange.second.at(index));
        }
    }

    std::vector<std::shared_ptr<Tile>> tiles;
    for (int i = 0; i < getNumTiles(); ++i) {
        int possibleResourcesLen = possibleResources.size();
        int resIndex = rand() % possibleResourcesLen;
        
        ResourceType type = possibleResources.at(resIndex);
        // Remove the resource type being used for this tile from the list of 
        // possibilities, and move the last resource in the list of 
        // posibilities to the empty spot created by the removal
        ResourceType tempRes = possibleResources.at(possibleResourcesLen - 1);
        possibleResources.at(possibleResourcesLen - 1) = possibleResources.at(resIndex);
        possibleResources.at(resIndex) = tempRes;
        possibleResources.pop_back();
    
        int value;
        if (type == ResourceType::Netflix) {
            value = 0;
        } else {
            int possibleTileValuesLen = tileValues.size();
            int valIndex = rand() % possibleTileValuesLen;
            value = tileValues.at(valIndex);
            // Remove the value being used for this tile from the list of 
            // possibilities, and replace it with the last item in the list
            int tempVal = tileValues.at(possibleTileValuesLen - 1);
            tileValues.at(possibleTileValuesLen - 1) = tileValues.at(valIndex);
            tileValues.at(valIndex) = tempVal;
            tileValues.pop_back();
        }
        
        std::shared_ptr<Tile> newTile(new Tile(playerManager, goose, this,
                         getGoalIdsByTile().at(i),
                         getCriterionIdsByTile().at(i),
                         type,
                         value));
        tiles.emplace_back(newTile);
        tileSubject.attach(newTile);
    }
    return tiles;
}
