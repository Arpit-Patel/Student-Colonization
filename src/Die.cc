#include "Subject.h"
#include "Observer.h"
#include "Die.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Die::Die(){}

void Die::rollRandom(){
    srand(time(NULL));
    int num1 = rand() % 6 + 1;
    int num2 = rand() % 6 + 1;
    int val = num1 + num2;
    notifyObservers(val);
    lastRoll = val;
}

void Die::rollLoaded(int val){
    notifyObservers(val);
    lastRoll = val;
} 

void Die::notifyObservers(int val) {
    for (unsigned int i = 0; i < observers.size(); ++i) {
        observers[i]->notify(val); 
    }
}

int Die::getLastRoll() { return lastRoll; }
