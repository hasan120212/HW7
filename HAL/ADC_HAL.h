/*
 * ADC_HAL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef ADC_HAL_H_
#define ADC_HAL_H_


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef enum {left, notleft} _joystickleftState;
typedef enum {up, notup}_joystickupState;
typedef enum {down, notdown}_joystickdownState;
typedef enum {right, notright}_joystickrightState;
struct _Joystick
{

    int x;///x-horizantal reading of the joystick
    int y; //y-vertical reading of the joystick
    _joystickleftState leftState;
    bool isTappedtoLeft;

    _joystickupState upState;
    bool isTappedtoUp;

    _joystickdownState downState;
    bool isTappedtoDown;

    _joystickrightState rightState;
    bool isTappedtoRight;
};
typedef struct _Joystick Joystick;


Joystick Joystick_construct();
void Joystick_refresh(Joystick *);
bool Joystick_isPushedToleft(Joystick *);
bool Joystick_isTappedToleft(Joystick *);

bool Joystick_isPressedUp(Joystick *);
bool Joystick_isTappedToUp(Joystick *);

bool Joystick_isPressedDown(Joystick *);
bool Joystick_isTappedToDown(Joystick *);

bool Joystick_isPressedRight(Joystick *);
bool Joystick_isTappedToRight(Joystick *);





void getSampleJoyStick(unsigned *X, unsigned *Y);

#endif /* ADC_HAL_H_ */
