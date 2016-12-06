#include "Goose.h"

const int DIE_ROLL_FOR_MOVE = 7;

Goose::Goose():curTile{-1} {}

int Goose::getDieRollForMove() { return DIE_ROLL_FOR_MOVE; }

int Goose::getPosition() { return curTile; }

void Goose::setPosition(int newPosition) { curTile = newPosition; }
