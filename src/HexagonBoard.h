#ifndef __HEXAGON_BOARD__
#define __HEXAGON_BOARD__

#include "Board.h"
#include "Goose.h"
#include "ResourceType.h"
#include "PlayerManager.h"
#include "Subject.h"

#include <vector>
#include <map>
#include <memory>

class HexagonBoard : public Board {
public:
    HexagonBoard(std::shared_ptr<PlayerManager> playerManager, 
                 std::shared_ptr<Goose> goose, Subject& subject, int seed);
    int getBoardWidth() override;
    int getEdgesPerTile() override;
    int getTilesPerBoard() override;
private:
    int getNumTiles() override;
    std::map<ResourceType, int>& getResourceCounts() override;
    std::vector<std::vector<int>>& getGoalIdsByTile() override;
    std::vector<std::vector<int>>& getCriterionIdsByTile() override;
    std::vector<std::pair<int, std::vector<int>>>& getNumValsByRange() override;
    int getNumCriterions() override;
    int getNumGoals() override;
};

#endif
