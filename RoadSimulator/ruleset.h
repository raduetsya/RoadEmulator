#ifndef RULESET_H
#define RULESET_H

#include "car.h"
#include "road.h"

#include "rulecond.h"

#include "utils.h"

#include <map>
#include <stdexcept>

class RuleSet :
        public Observable<RuleSet>,
        public Observer<Car>,
        public Observer<Road>
{
public:
    RuleSet() {
        using namespace RuleConds;

        // http://www.mindmup.com/#m:a10063d470ba690131feaf0a11bda4f85c

        addRule(0,  1, new RightDirection );
        addRule( 1,  4, new CrossLine );
        addRule(  4,  5, new CheckRoadType(RoadType::DASHED) );
        addRule(   5,  11, new CrossLine(false) );
        addRule(    11, 9, new RightDirection );
        addRule(     9,  0, nullptr );
        addRule(    11, 7, new RightDirection(false) );
        addRule(     7,  3, new NotInRoad );
        addRule(     7,  8, new WrongDirectionTrustZone(Config::get().data.SAFE_TURN_MANEUR_ZONE) );
        addRule(      8,  3, new NotInRoad );
        addRule(      8,  0, new RightDirection );
        addRule(     7,  9, new RightDirection );
        addRule(      9,  0, nullptr );
        addRule(  4,  6, new CheckRoadType(RoadType::SOLID) );
        addRule(   6,  3, new NotInRoad);
        addRule(   6,  10, new CrossLine(false) );
        addRule(    10, 1, nullptr );
        addRule( 1,  3, new NotInRoad );
        addRule(  3,  0, new NotInRoad(false) );
        addRule( 1,  2, new RightDirection(false) );
        addRule(  2,  0, new RightDirection );

        addViolationRule(7, TURN_MANEUR_WARN);
        addViolationRule(8, TURN_MANEUR_VIOL);
        addViolationRule(9, TURN_MANEUR_OK);
        addViolationRule(6, SOLID_LINE_CROSS);
        addViolationRule(2, WRONG_DIRECTION);
        addViolationRule(3, NOT_IN_ROAD);
        addViolationRule(0, OK);
    }

    ~RuleSet()
    {
        for (auto i = stateMachine.begin(); i != stateMachine.end(); ++i) {
            (*i).second->deleteAllConds();
			delete (*i).second;
		}

        for (auto i = carStates.begin(); i != carStates.end(); ++i) {
            (*i).second->deleteAllConds();
			delete (*i).second;
		}

    }

    enum ViolationType {
        OK,
        TURN_MANEUR_WARN,
        TURN_MANEUR_VIOL,
        TURN_MANEUR_OK,
        SOLID_LINE_CROSS,
        WRONG_DIRECTION,
        NOT_IN_ROAD,
    };

    void update(Road* ev)
    {
        road = ev;
    }

    void update(Car *ev)
    {
        if (road == nullptr) return;

        RuleSetQuickParams params = generateParams(ev);

        int debug_state = -1;

        StateMachineEntry* state;
        if (carStates.count(ev) != 0)
            state = carStates[ev];
        else {
			carStates[ev] = stateMachine[0]->copy();
            state = carStates[ev];
		}

        unsigned long int ic = 0; // iteration counter, gotta reduce eternity
        do
        {
            bool isNewStateFounded = false;
            for(auto i = state->conds.begin();
                i != state->conds.end();
                ++i)
            {
                if ( (*i).first == nullptr || (*i).first->check(params) ) {
                    isNewStateFounded = true;
                    debug_state = (*i).second;
                    //std::cout << "New state: " << debug_state << std::endl;
                    if ( violationRules.count( (*i).second ) > 0 )
                        notifyViolation( violationRules[(*i).second] );
					StateMachineEntry* nextState = stateMachine[ (*i).second ]->copy();
                    state->replaceWithNewState( nextState );
					delete nextState;
                    break;
                }
            }
            if (!isNewStateFounded) break;
            if (ic++ > 100) {
                // obvious eternal cycle
                // set breakpoint here and correct your state rules
                ic++;
                if (ic > 1000) throw BadRuleProgram();
            }
        } while (true);
        carStates[ev] = state;

        violation = OK;
    }

    ViolationType getViolation() {
        return violation;
    }

    class BadRuleProgram : public std::exception {};

private:
    Road* road;
    ViolationType violation;

private:
    struct StateMachineEntry {
		StateMachineEntry() { 
			std::cout << "StateMachineEntry::ctor" << std::endl;
		}

        std::list< const std::pair<RuleStateCond*, int> > conds;

        StateMachineEntry* copy() {
			StateMachineEntry* newState = new StateMachineEntry();
            for ( auto i = this->conds.begin();
                    i != this->conds.end();
                    ++i) {
                if ((*i).first != nullptr)
                    newState->conds.push_back( 
						std::pair<RuleStateCond*, int>(
							(*i).first->copy(), (*i).second
						));
                else
                    newState->conds.push_back( 
						std::pair<RuleStateCond*, int>(
							nullptr, (*i).second
						));
            }
            return newState;
        }

        void replaceWithNewState(StateMachineEntry* newState) {
			deleteAllConds();
            conds = newState->conds;
        }

        void deleteAllConds() {
            for ( auto i = this->conds.begin();
                    i != this->conds.end();
                    ++i) {
                if ((*i).first != nullptr)
                    delete (*i).first;
            }
        }
    };

    std::map<int, StateMachineEntry*> stateMachine;
    std::map<Car*, StateMachineEntry*> carStates;
    std::map<int, ViolationType> violationRules;

    void addRule(int from, int to, RuleStateCond* cond) {
		if (stateMachine.count(from) == 0)
			stateMachine[from] = new StateMachineEntry();
        stateMachine[from]->conds.push_back(
			std::pair<RuleStateCond*, int>(
				cond, to
			));
    }

    void addViolationRule(int stateNum, ViolationType v) {
        violationRules[stateNum] = v;
    }

    void notifyViolation(ViolationType v) {
        violation = v;
        notify();
        // maybe expand to support many violations at same time
    }

    RuleSetQuickParams generateParams(Car* cur)
    {
        RuleSetQuickParams params;

        params.x = cur->getNewState()->getPos('x');
        params.past.x = cur->getPos('x');

        params.y = cur->getNewState()->getPos('y');
        params.past.y = cur->getPos('y');

        params.angle = cur->getNewState()->getAngle();
        params.past.angle = cur->getAngle();

        params.roType = road->getRoadType( params.y );
        params.past.roType = road->getRoadType( params.past.y );
        params.width = (int)road->getSize('w');
        params.height = (int)road->getSize('h');
        params.w_2 = params.width / 2;

        if (road->isRightHanded())
            params.isOnUpHand = (params.x > params.w_2);
        else
            params.isOnUpHand = (params.x < params.w_2);

        return params;
    }
};

#endif // RULESET_H
