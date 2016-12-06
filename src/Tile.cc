#include "Tile.h"
#include "Board.h"
#include "ResourceType.h"
#include "PlayerManager.h"
#include "Subject.h"
#include "Observer.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

const std::map<CriterionType, int> CRITERION_REWARD {
    {CriterionType::None, 0},
    {CriterionType::Assignment,1},
    {CriterionType::Midterm, 2},
    {CriterionType::Exam, 3},
};

Tile::Tile(const std::shared_ptr<PlayerManager> playerManager, 
           const std::shared_ptr<Goose> goose, Board* parent, 
           const std::vector<int> goalIds, const std::vector<int> criterionIds, 
           const ResourceType type, const int value):
        playerManager{playerManager}, goose{goose}, parent{parent},
        numGoals{goalIds.size()}, numCriterions{criterionIds.size()},
        goalIds{goalIds}, criterionIds{criterionIds},
        type{type}, value{type == ResourceType::Netflix ? 0 : value} {
}

void Tile::notify(int dieRoll) {
    if (dieRoll == value && goose->getPosition() != value) {
        for (int id : criterionIds) {
            char criterionPlayer = parent->getCriterionPlayer(id);
            if (criterionPlayer != '\0') {
                const CriterionType& criterionType = parent->getCriterionType(criterionPlayer, id);
                playerManager->addResource(criterionPlayer, type, CRITERION_REWARD.at(criterionType));
            }
        }
    }
}

void Tile::setResourceType(ResourceType newType) {
    type = newType;
    if (type == ResourceType::Netflix) {
        value = 0;
    }
}

void Tile::setValue(int newValue) {
    value = newValue;
    if (type == ResourceType::Netflix) {
        value = 0;
    }
}

ResourceType Tile::getResourceType() {
    return type;
}

// gets the position of the provided ID in the provided vector, or throws an
// InvalidIDException if the id could not be found
int getPosition(std::vector<int> vec, int id) {
    for (unsigned int i = 0; i < vec.size(); ++i) {
        if (vec.at(i) == id) {
            return i;
        }
    }
    throw InvalidIDException();
}

bool Tile::containsGoal(int id) {
    return (std::find(goalIds.begin(), goalIds.end(), id) != goalIds.end());
}

bool Tile::containsCriterion(int id) {
    return (std::find(criterionIds.begin(), criterionIds.end(), id) != criterionIds.end());
}

int Tile::getGoalToRightOfCriterion(int id) {
    const int targetPos = getPosition(criterionIds, id);
    // we do not add anything because the n-th goal is to the right of the
    // n-th criterion
    return goalIds.at(targetPos);
}

int Tile::getGoalToLeftOfCriterion(int id) {
    const int targetPos = getPosition(criterionIds, id);
    int resultPos = targetPos - 1;
    if (resultPos < 0) {
        resultPos += numGoals;
    }
    return goalIds.at(resultPos);
}

int Tile::getGoalToRight(int id) {
    const int targetPos = getPosition(goalIds, id);
    int resultPos = targetPos + 1;
    while (resultPos >= numGoals) {
        resultPos -= numGoals;
    }
    return goalIds.at(resultPos);
}

int Tile::getGoalToLeft(int id) {
    const int targetPos = getPosition(goalIds, id);
    int resultPos = targetPos - 1;
    if (resultPos < 0) {
        resultPos += numGoals;
    }
    return goalIds.at(resultPos);
}

int Tile::getCriterionToRightOfGoal(int id) {
    const int targetPos = getPosition(goalIds, id);
    int resultPos = targetPos + 1;
    while (resultPos >= numCriterions) {
        resultPos -= numGoals;
    }
    return criterionIds.at(resultPos);
}

int Tile::getCriterionToLeftOfGoal(int id) {
    const int targetPos = getPosition(goalIds, id);
    // we do not subtract anything because the goal positions are offset, so
    // the n-th goal is to the right of the n-th criterion
    return criterionIds.at(targetPos);
}

int Tile::getCriterionToRight(int id) {
    const int targetPos = getPosition(criterionIds, id);
    int resultPos = targetPos + 1;
    while (resultPos >= numCriterions) {
        resultPos -= numGoals;
    }
    return criterionIds.at(resultPos);
}

int Tile::getCriterionToLeft(int id) {
    const int targetPos = getPosition(criterionIds, id);
    int resultPos = targetPos - 1;
    if (resultPos < 0) {
        resultPos += numCriterions;
    }
    return criterionIds.at(resultPos);
}

std::set<char> Tile::getAdjascentPlayers() {
    std::set<char> adjascentPlayers;
    for (int id : criterionIds) {
        char player = parent->getCriterionPlayer(id);
        if (player != '\0') {
            adjascentPlayers.insert(player);
        }
    }
    return adjascentPlayers;
}

int Tile::getValue() {
    return value;
}
