#ifndef RULESET_H
#define RULESET_H

#include "car.h"
#include "road.h"

#include "rulecond.h"

#include "observer.h"

#include <map>
#include <stdexcept>

class RuleSet :
        public Observable<RuleSet>,
        public Observer<Car>,
        public Observer<Road>
{
public:
    RuleSet();
    virtual ~RuleSet();

    enum ViolationType {
        OK,
        TURN_MANEUR_WARN,
        TURN_MANEUR_VIOL,
        TURN_MANEUR_OK,
        SOLID_LINE_CROSS,
        WRONG_DIRECTION,
        NOT_IN_ROAD,
    };

    void update(Road* ev);
    void update(Car *ev);

    ViolationType getViolation() {
        return violation;
    }

    class BadRuleProgram : public std::exception {};

private:
    Road* road;
    ViolationType violation;

private:
    struct StateMachineEntry {
        std::list< const std::pair<RuleStateCond*, int> > conds;

		StateMachineEntry* copy();
        void replaceWithNewState(StateMachineEntry* newState);
        void deleteAllConds();
    };

    std::map<int, StateMachineEntry*> stateMachine;
    std::map<Car*, StateMachineEntry*> carStates;
    std::map<int, ViolationType> violationRules;

    void addRule(int from, int to, RuleStateCond* cond);
    void addViolationRule(int stateNum, ViolationType v);
    void notifyViolation(ViolationType v);
    RuleSetQuickParams generateParams(Car* cur);
};

#endif // RULESET_H
