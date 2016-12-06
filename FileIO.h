#ifndef _FILEIO_H
#define _FILEIO_H

#include "Board.h"
#include "Player.h"
#include "PlayerManager.h"
#include "BaseIO.h"

#include <memory>
#include <string>

class FileIO{
public:
    // saves the current state of the game in the file with the specified name
    void save(const std::shared_ptr<Board> b, 
              const std::shared_ptr<PlayerManager> playerManager, const int curP, 
              const std::string &fileName);
    // loads the board saved in the provided save file, but keeps the players,
    // criterions, and goals the same
    void loadBoard(const std::string& saveFile, const std::shared_ptr<Board> board);
    // loads the game saved in the provided save file. Must be saved in the
    // format output by the save function
    void load(const std::string& saveFile, const std::shared_ptr <Board> board, 
              const std::shared_ptr<PlayerManager> playerManager, 
              int &startingPlayerPos, const std::vector<char>& playerChars);
};

#endif
