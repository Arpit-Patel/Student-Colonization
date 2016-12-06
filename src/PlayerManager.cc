#include "PlayerManager.h"
#include "ResourceType.h"
#include "Player.h"
#include "Constants.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>

std::map<ResourceType, int> createEmptyResourceMap() {
    std::map<ResourceType, int> resourceChanges;
    resourceChanges[ResourceType::Netflix] = 0;
    resourceChanges[ResourceType::Caffeine] = 0;
    resourceChanges[ResourceType::Lab] = 0;
    resourceChanges[ResourceType::Study] = 0;
    resourceChanges[ResourceType::Lecture] = 0;
    resourceChanges[ResourceType::Tutorial] = 0;
    return resourceChanges;
}

PlayerManager::PlayerManager() {}

void PlayerManager::setPlayers(const std::map<char, Player> newPlayers) {
    players = newPlayers;
}

void PlayerManager::addPlayer(const char player) {
    players[player] = Player();
    resourcesAtLastReset[player] = createEmptyResourceMap();
}

void PlayerManager::removePlayer(const char player) {
    players.erase(player);
}

void PlayerManager::resetPlayers() {
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second.reset();
    }
}

void PlayerManager::addResource(const char player, ResourceType resType, 
                                const int count) {
    players[player].addResource(resType, count);
    
}

void PlayerManager::removeResource(const char player, const ResourceType resType, 
                                   const int count) {
    players[player].removeResource(resType, count);
}

void PlayerManager::removeRandomResources(const char player, 
                                          const int numToRemove) {
    players[player].removeRandomResources(numToRemove);
}

ResourceType PlayerManager::removeRandomResource(const char player) {
    return players[player].removeRandomResource();
}

int PlayerManager::getResourceCount(const char player, 
                                    const ResourceType resType) {
    return players[player].getResourceCount(resType);
}

int PlayerManager::getNumGoals(const char player) {
    return players[player].getNumGoals();
}

int PlayerManager::getNumCriterions(const char player) {
    return players[player].getNumCriterions();
}

void PlayerManager::addCriterion(const char player, const int id) {
    players[player].addCriterion(id);
}

void PlayerManager::addGoal(const char player, const int id) {
    players[player].addGoal(id);
}

void PlayerManager::addVictoryPoint(const char player) {
    players[player].addVictoryPoint();
}

int PlayerManager::getVictoryPoints(const char player) {
    return players[player].getVictoryPoints();
}

bool PlayerManager::hasNoResources(const char player) {
    return players[player].hasNoResources();
}

std::vector<int> PlayerManager::getGoals(const char player) {
    return players[player].getGoals();
}

std::vector<int> PlayerManager::getCriterions(const char player) {
    return players[player].getCriterions();
}


void PlayerManager::loadDice(const char player, const int dieNum) {
    players[player].loadDice(dieNum);
}

void PlayerManager::randomizeDice(const char player) {
    players[player].randomizeDice();
}

bool PlayerManager::checkFair(const char player) {
    return players[player].checkFair();
}

int PlayerManager::getLoadedInt(const char player) {
    return players[player].getLoadedInt();
}

void PlayerManager::resetResourceChanges() {
    for (auto pIt = players.begin(); pIt != players.end(); ++pIt) {
        for (auto rIt = RESOURCE_VECTOR.begin(); rIt != RESOURCE_VECTOR.end(); ++rIt) {
            resourcesAtLastReset[pIt->first][*rIt] = getResourceCount(pIt->first, *rIt);
        }
    }
}

std::map<ResourceType, int> PlayerManager::getResourceChanges(const char player) {
    std::map<ResourceType, int> resourceChange;
    for (auto rIt = RESOURCE_VECTOR.begin(); rIt != RESOURCE_VECTOR.end(); ++rIt) {
        resourceChange[*rIt] = getResourceCount(player, *rIt) - resourcesAtLastReset.at(player).at(*rIt);
    }
    return resourceChange;
}

std::string PlayerManager::getColorName(char player) {
    return COLOURS_BY_CHAR.at(player);
}
