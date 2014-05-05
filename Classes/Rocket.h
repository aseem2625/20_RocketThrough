//
//  Rocket.h
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/05/03.
//
//

#ifndef ___0_RocketThrough__Rocket__
#define ___0_RocketThrough__Rocket__

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

enum class RotationOrientation : int {
    NONE,
    CLOCKWIZE,
    COUNTER_CLOCKWIZE
};

class Rocket : public GameSprite
{
public:
    Rocket(void);
    static Rocket *create();
    
    void update(float dt);
    void reset(void);
    bool isCollidedWithSides(void);
    void select(bool flag);
    
    inline void setTargetRotation(float value) {
        _targetRotation = value;
    }
    
    inline void setRotationFromVector() {
        this->setTargetRotation(CC_RADIANS_TO_DEGREES(atan2(-_vector.y, _vector.x)));
    }
    
    CC_SYNTHESIZE(Point, _vector, Vector);
    CC_SYNTHESIZE(Point, _pivot, Pivot);
    CC_SYNTHESIZE(float, _speed, Speed);
    CC_SYNTHESIZE(float, _angularSpeed, AngularSpeed);
    CC_SYNTHESIZE(RotationOrientation, _rotationOrientation, RotationOrientation);
    
protected:
    
    
private:
	
	float _targetRotation;
	float _dr;
	float _ar;
	float _vr;
	float _rotationSpring;
	float _rotationDamping;
    
};

#endif /* defined(___0_RocketThrough__Rocket__) */
