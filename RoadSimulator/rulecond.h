#ifndef RULECOND_H
#define RULECOND_H

#include <iostream>
#include <cmath>

struct RuleSetQuickParams {
    float x,y,angle;
    int roType;
    bool isOnUpHand;
    struct {
        float x,y,angle;
        int roType;
        bool isOnUpHand;
    } past;

    int width, height;
    int w_2;
};

class RuleStateCond {
public:
	virtual ~RuleStateCond() {};

    virtual RuleStateCond* copy() = 0;

    bool check(RuleSetQuickParams& obj) {
        if (doNotInvertCheck) return ruleCheck(obj);
        else return !ruleCheck(obj);
    }

protected:
    virtual bool ruleCheck(RuleSetQuickParams& obj) = 0;
    bool doNotInvertCheck;

    RuleStateCond(bool doNotInvertCheck = true) :
        doNotInvertCheck(doNotInvertCheck) {}
};

namespace RuleConds {
/*
    class _NAME_ : public RuleStateCond {
        RuleStateCond* copy() { return new _NAME_( _RUNTIME_VARS_ ); }
        bool check(RuleSetQuickParams& obj) {
            return (_COND_);
        }
    };
*/

// RightDirection(true)
// RightDirection(false)
// NotInRoad()
// NotInRoad(false)
// CrossLine()
// CrossLine(false)
// WrongDirectionTrustZone()
// CheckRoadType(int type)

    class RightDirection : public RuleStateCond {
    public:
        RightDirection(bool doNotInvertCheck = true) : RuleStateCond(doNotInvertCheck) {}

        RuleStateCond* copy() { return new RightDirection(doNotInvertCheck); }
        bool ruleCheck(RuleSetQuickParams &obj) {
            bool res = (abs(obj.angle) < 90);
            if (obj.isOnUpHand) return res; else return !res;
        }
    };

    class NotInRoad : public RuleStateCond {
    public:
        NotInRoad(bool doNotInvertCheck = true) : RuleStateCond(doNotInvertCheck) {}
        RuleStateCond* copy() { return new NotInRoad(doNotInvertCheck); }
        bool ruleCheck(RuleSetQuickParams& obj) {
            if (obj.x > obj.width) return true;
            if (obj.x < 0) return true;
            if (obj.y > obj.height) return true;
            if (obj.y < 0) return true;
            else return false;
        }
    };

    class CrossLine : public RuleStateCond {
    public:
        CrossLine(bool doNotInvertCheck = true) : RuleStateCond(doNotInvertCheck) {}
        RuleStateCond* copy() { return new CrossLine(doNotInvertCheck); }
        bool ruleCheck(RuleSetQuickParams& obj) {
            if (
                ( obj.x <= obj.w_2 && obj.past.x >= obj.w_2 ) ||
                ( obj.x >= obj.w_2 && obj.past.x <= obj.w_2 ) )
                return true;
            else return false;
        }
    };

    class WrongDirectionTrustZone : public RuleStateCond {
        float crossY;
        bool firstCall;
        float safeZone;
    public:
        WrongDirectionTrustZone(float safeZone) : firstCall(true), safeZone(safeZone) {}
        RuleStateCond* copy() { return new WrongDirectionTrustZone(safeZone); }
        bool ruleCheck(RuleSetQuickParams& obj) {
            if (firstCall) {
                // first call happens right after state change, we're safe
                crossY = obj.y;
                firstCall = false;
            }
            if ( abs(crossY - obj.y) > safeZone ) return true;
            else return false;
        }
    };

    class CheckRoadType : public RuleStateCond {
        int type;
    public:
        CheckRoadType(int type) : type(type) {}
        RuleStateCond* copy() { return new CheckRoadType(type); }
        bool ruleCheck(RuleSetQuickParams& obj) {
            return (obj.roType == type);
        }
    };

}


#endif // RULECOND_H
