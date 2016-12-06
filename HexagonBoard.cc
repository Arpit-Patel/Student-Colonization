#include "HexagonBoard.h"
#include "ResourceType.h"
#include "Board.h"
#include "Goal.h"
#include "Criterion.h"
#include "Goose.h"
#include "Subject.h"

#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <memory>

const int NUM_TILES = 19;
const int NUM_GOALS = 72;
const int NUM_CRITERION = 54;
const int NUM_CRITERION_PER_TILE = 6;
const int NUM_GOAL_PER_TILE = 6;
const int BOARD_WIDTH = 5;
const int NUM_ROWS = 9;

std::map<ResourceType, int> RESOURCE_CT
{
    {ResourceType::Tutorial, 3},
    {ResourceType::Study,    3},
    {ResourceType::Caffeine, 4},
    {ResourceType::Lab,      4},
    {ResourceType::Lecture,  4},
    {ResourceType::Netflix,  1}
};

std::vector<std::pair<int, std::vector<int>>> NUM_FROM_RANGE
{
    std::pair<int, std::vector<int>> (1, std::vector<int> {2}),
    std::pair<int, std::vector<int>> (1, std::vector<int> {12}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {3}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {4}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {5}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {6}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {8}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {9}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {10}),
    std::pair<int, std::vector<int>> (2, std::vector<int> {11})
};

std::vector<std::vector<int>> GOAL_IDS_BY_TILE
{
    std::vector<int> {
         0,  2,  7,  10,  6, 1
    },
    std::vector<int> {
         3,  6, 14, 18, 13,  5
    },
    std::vector<int> {
         4,  8, 16, 19, 15,  7
    },
    std::vector<int> {
         9, 13, 21, 26, 20, 12
    },
    std::vector<int> {
        10, 15, 23, 27, 22, 14
    },
    std::vector<int> {
        11, 17, 25, 28, 24, 16
    },
    std::vector<int> {
        18, 22, 31, 35, 30, 21
    },
    std::vector<int> {
        19, 24, 33, 36, 32, 23
    },
    std::vector<int> {
        26, 30, 38, 43, 37, 29
    },
    std::vector<int> {
        27, 32, 40, 44, 39, 31
    },
    std::vector<int> {
        28, 34, 42, 45, 41, 33
    },
    std::vector<int> {
        35, 39, 48, 52, 47, 38
    },
    std::vector<int> {
        36, 41, 50, 53, 49, 40
    },
    std::vector<int> {
        43, 47, 55, 60, 54, 46
    },
    std::vector<int> {
        44, 49, 57, 61, 56, 48
    },
    std::vector<int> {
        45, 51, 59, 62, 58, 50
    },
    std::vector<int> {
        52, 56, 64, 67, 63, 55
    },
    std::vector<int> {
        53, 58, 66, 68, 65, 57
    },
    std::vector<int> {
        61, 65, 70, 71, 69, 64
    }
};

std::vector<std::vector<int>> CRITERION_IDS_BY_TILE
{
    std::vector<int> {
         0,  1,  4,  9,  8,  3
    },
    std::vector<int> {
         2,  3,  8, 14, 13,  7
    },
    std::vector<int> {
         4,  5, 10, 16, 15,  9
    },
    std::vector<int> {
         6,  7, 13, 19, 18, 12
    },
    std::vector<int> {
         8,  9, 15, 21, 20, 14
    },
    std::vector<int> {
        10, 11, 17, 23, 22, 16
    },
    std::vector<int> {
        13, 14, 20, 26, 25, 19
    },
    std::vector<int> {
        15, 16, 22, 28, 27, 21
    },
    std::vector<int> {
        18, 19, 25, 31, 30, 24
    },
    std::vector<int> {
        20, 21, 27, 33, 32, 26
    },
    std::vector<int> {
        22, 23, 29, 35, 34, 28
    },
    std::vector<int> {
        25, 26, 32, 38, 37, 31
    },
    std::vector<int> {
        27, 28, 34, 40, 39, 33
    },
    std::vector<int> {
        30, 31, 37, 43, 42, 36
    },
    std::vector<int> {
        32, 33, 39, 45, 44, 38
    },
    std::vector<int> {
        34, 35, 41, 47, 46, 40
    },
    std::vector<int> {
        37, 38, 44, 49, 48, 43
    },
    std::vector<int> {
        39, 40, 46, 51, 50, 45
    },
    std::vector<int> {
        44, 45, 50, 53, 52, 49
    }
};

HexagonBoard::HexagonBoard(std::shared_ptr<PlayerManager> playerManager, 
                           std::shared_ptr<Goose> goose, Subject& subject, 
                           int seed=0):
        Board(playerManager, goose, subject, seed) {
    tiles = generateTiles(seed);
}

int HexagonBoard::getBoardWidth() {
    return BOARD_WIDTH;
}
int HexagonBoard::getEdgesPerTile() {
    return NUM_GOAL_PER_TILE;
}

int HexagonBoard::getTilesPerBoard() {
    return NUM_TILES;
}

int HexagonBoard::getNumTiles() {
    return NUM_TILES;
}

int HexagonBoard::getNumCriterions() {
    return NUM_CRITERION;
}

int HexagonBoard::getNumGoals() {
    return NUM_GOALS;
}

std::map<ResourceType, int>& HexagonBoard::getResourceCounts() {
    return RESOURCE_CT;
}

std::vector<std::vector<int>>& HexagonBoard::getGoalIdsByTile() {
    return GOAL_IDS_BY_TILE;
}

std::vector<std::vector<int>>& HexagonBoard::getCriterionIdsByTile() {
    return CRITERION_IDS_BY_TILE;
}

std::vector<std::pair<int, std::vector<int>>>& HexagonBoard::getNumValsByRange() {
    return NUM_FROM_RANGE;
}
