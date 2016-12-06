#include "Player.h"
#include "ResourceType.h"

#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

Player::Player():fair{true},victoryPoints{0} {}

Player::Player(const vector<int> criterion, const vector<int> goals, 
               const map<ResourceType,int> resources, const char color, 
               const int victoryPoints):
        resources{resources}, criterion{criterion}, goals{goals}, color{color}, 
        fair{true}, victoryPoints{victoryPoints} {}

void Player::addResource(const ResourceType resType, const int count){
    if (resources.count(resType) == 0) {
        resources[resType] = count;
    } else {
        resources[resType] += count;
    }
}

void Player::removeResource(const ResourceType resType, const int count){
    if (resources.count(resType) != 0) {
        resources[resType] -= count;
    }
}

void Player::reset() {
    for (auto resType : RESOURCE_VECTOR) {
        resources[resType] = 0;
    }
    victoryPoints = 0;
    criterion.clear();
    goals.clear();
}

void Player::removeRandomResources(int numToRemove) {
    int numResources = 0;
    for (ResourceType resType : RESOURCE_VECTOR) {
        numResources += resources[resType];
    }
    while (numToRemove > 0) {
        srand(time(NULL));
        const int posToRemove = rand() % numResources;
        int currentPos = 0;
        for (ResourceType resType : RESOURCE_VECTOR) {
            currentPos += resources[resType];
            if (currentPos >= posToRemove) {
                --resources[resType];
                break;
            }
        }
        --numToRemove;
        --numResources;
    }
}

ResourceType Player::removeRandomResource() {
    int numResources = 0;
    for (ResourceType resType : RESOURCE_VECTOR) {
        if (resources.count(resType) == 0) {
            continue;
        }
        numResources += resources[resType];
    }
    if (numResources == 0) {
        throw RandomSelecFailException();
    }
    srand(time(NULL));
    const int posToRemove = rand() % numResources;
    int currentPos = 0;
    for (ResourceType resType : RESOURCE_VECTOR) {
        if (resources.count(resType) == 0) {
            continue;
        }
        currentPos += resources[resType];
        if (currentPos >= posToRemove) {
            --resources[resType];
            return resType;
        }
    }
    throw RandomSelecFailException();
}

int Player::getResourceCount(const ResourceType resType){
    if (resources.count(resType) == 0) {
        return 0;
    } else {
        return resources[resType];
    }
}
char Player::getColor(){
  return color;
}

int Player::getNumGoals(){
  return goals.size();
}
int Player::getNumCriterions(){
  return criterion.size();
}
void Player::addCriterion(int id){
  criterion.emplace_back(id);
}
void Player::addGoal(int id){
  goals.emplace_back(id);
}
void Player::addVictoryPoint() {
    ++victoryPoints;
}
bool Player::hasNoResources() {
    for (ResourceType resType : RESOURCE_VECTOR) {
        if (resources.count(resType) == 0) {
            continue;
        }
        if (resources.at(resType) != 0) {
            return false;
        }
    }
    return true;
}
vector<int> Player::getCriterions(){
  return criterion;
}
vector<int> Player::getGoals(){
  return goals;
}
int Player::getVictoryPoints() {
    return victoryPoints;
}
void Player::loadDice(const int dieNum){
  fair = false;
  loadedNum = dieNum;
}
void Player::randomizeDice(){
  fair = true;
}
bool Player::checkFair(){
  return fair;
} 

int Player::getLoadedInt(){  
  return loadedNum;
}


