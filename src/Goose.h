#ifndef _GOOSE_H_
#define _GOOSE_H_

class Goose {
    int curTile;
public:
    Goose();
    // returns the tile the goose is currently on
    int getPosition();
    // sets the position of the goose
    void setPosition(int newPosition);
    // gets the value of the die roll required for the goose to move
    int getDieRollForMove();
};

#endif
