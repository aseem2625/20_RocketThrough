//
//  Rocket.cpp
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/05/03.
//
//

#include "Rocket.h"

Rocket::Rocket():
_targetRotation(0),
_dr(0),
_ar(0),
_vr(0),
_rotationSpring(.2),
_rotationDamping(.5),
_pivot(Point(-1, -1)),
_rotationOrientation(RotationOrientation::NONE)
{
    this->reset();
}

void Rocket::reset () {
    
    _speed = 50;
	_pivot = Point(-1,1);
    _rotationOrientation = RotationOrientation::NONE;
    this->setRotation(-90);
	_targetRotation = -90;
    float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
	_vector = Point(_speed * cos( angle ),
                    -_speed * sin( angle ));
}

Rocket *Rocket::create()
{
    Rocket *rocket = new Rocket();
    if (rocket && rocket->initWithSpriteFrameName("rocket.png")) {
        rocket->autorelease();
        rocket->setRadius(rocket->getContentSize().width/2);
        return rocket;
    }
    CC_SAFE_DELETE(rocket);
    return NULL;
}

void Rocket::select(bool flag)
{
    if (flag) {
        this->setSpriteFrame("rocket_on.png");
    } else {
        this->setSpriteFrame("rocket.png");
    }
}

void Rocket::update(float dt)
{
    Point position = this->getPosition();
    
    if (_rotationOrientation == RotationOrientation::NONE) {
        position += _vector * dt;
        
    } else {
        // rotate point around a pivot by a certain amount
        Point rotatedPoint = position.rotateByAngle(_pivot, _angularSpeed * dt);
        position = rotatedPoint;
        
        float rotatedAngle = 0;
        Point clockwise = (position + _pivot).getRPerp();
        
        if (_rotationOrientation == RotationOrientation::COUNTER_CLOCKWIZE) {
            rotatedAngle = atan2(-clockwise.y, -clockwise.x);
        } else {
            rotatedAngle = atan2(clockwise.y, clockwise.x);
        }
        
        // update rocket vector
        _vector.x = cos(rotatedAngle);
        _vector.y = sin(rotatedAngle);
        
        this->setRotationFromVector();
        
        if (this->getRotation() > 0) {
            this->setRotation(fmodf(this->getRotation(), 360.0));
        } else {
            this->setRotation(fmodf(this->getRotation(), -360.0));
        }
    }
    
    if (_targetRotation > this->getRotation() + 180) {
        _targetRotation -= 360;
    }
    if (_targetRotation < this->getRotation() - 180) {
        _targetRotation += 360;
    }
    
    this->setPosition(position);
    
    _dr = _targetRotation - this->getRotation();
    _ar = _dr * _rotationSpring;
    _vr += _ar;
    this->setRotation(this->getRotation() + _vr);
}

bool Rocket::isCollidedWithSides()
{
    Size screenSize = CCDirector::getInstance()->getWinSize();
    
    if (_position.x > screenSize.width - _radius) {
        _position.x =  screenSize.width - _radius;
        _rotationOrientation = RotationOrientation::NONE;
        _vector = Point(this->getVector().x * -1, this->getVector().y);
        this->setRotationFromVector();
        return true;
    }
    
    if (_position.x < _radius) {
        _position.x =  _radius;
       	_rotationOrientation =  RotationOrientation::NONE;
        _vector =  Point(this->getVector().x * -1, this->getVector().y);
        this->setRotationFromVector();
        return true;
    }
    
    if (_position.y < _radius) {
        _position.y = _radius;
       	_rotationOrientation = RotationOrientation::NONE;
        _vector =  Point(this->getVector().x, this->getVector().y * -1);
        this->setRotationFromVector();
        return true;
    }
    
    if (_position.y > screenSize.height - _radius) {
        _position.y =  screenSize.height - _radius;
       	_rotationOrientation = RotationOrientation::NONE;
        _vector =  Point(this->getVector().x, this->getVector().y * -1);
        this->setRotationFromVector();
        return true;
    }
    
    return false;
}