#ifndef _DIE_H_
#define _DIE_H_
#include "Subject.h"

class Die : public Subject {
    int lastRoll;
public:
    Die();
    // randomely rolls the dice, and notifies the tiles of the roll so they can
    // distribute the appropreate resources
    void rollRandom();
    // simulates a die roll with the specified value, and notifies the tiles
    // of this roll so they can distribute resources
    void rollLoaded(int val);
    // gets the value of the roll
    int getLastRoll();
    void notifyObservers(int val) override;
};

#endif
