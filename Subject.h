#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include "Observer.h"

#include <vector>
#include <memory>

class Subject {
protected:
    std::vector<std::shared_ptr<Observer>> observers;
    std::vector<int> value; 
public:
    virtual ~Subject();
    virtual void attach(std::shared_ptr<Observer> o);
    virtual void detach(std::shared_ptr<Observer> o);
    virtual void notifyObservers(int val)=0;
};

#endif
