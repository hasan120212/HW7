/*
 * ADC_HAL.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */
#include <HAL/ADC_HAL.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define LEFT_THRESHOLD 2000
#define UP_THREHOLD 16000
#define DOWN_THREHOLD 2000
#define RIGHT_THREHOLD 16000

// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    // This sets the conversion clock to 3MHz
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 up to ADC_MEM1. Each conversion will
    // thus use two channels.
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    // TODO: which one of GPIO_PRIMARY_MODULE_FUNCTION, or
    //                    GPIO_SECONDARY_MODULE_FUNCTION, or
    //                    GPIO_TERTIARY_MODULE_FUNCTION
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    ADC14_configureConversionMemory(ADC_MEM1,
                                   ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                   ADC_INPUT_A9,                 // joystick y
                                   ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);




}
Joystick Joystick_construct(){
    Joystick joystick;

    initADC();
    initJoyStick();
    startADC();

    joystick.leftState = notleft;
    joystick.rightState = notright;
    joystick.upState = notup;
    joystick.downState = notdown;

    return (joystick);

}


//void getSampleJoyStick(unsigned *X, unsigned *Y) {
//    // ADC runs in continuous mode, we just read the conversion buffers
//    *X = ADC14_getResult(ADC_MEM0);
//
//    // TODO: Read the Y channel
//    *Y = ADC14_getResult(ADC_MEM1);
//}

void Joystick_refresh(Joystick *joystick_p){

    // ADC runs in continuous mode, we just read the conversion buffers
    joystick_p->x = ADC14_getResult(ADC_MEM0);

    // TODO: Read the Y channel
    joystick_p->y = ADC14_getResult(ADC_MEM1);
}

bool Joystick_isPushedToleft(Joystick *joystick_p){

    return (joystick_p->x < LEFT_THRESHOLD);


}


bool Joystick_isTappedToleft(Joystick *joystick_p){
    joystick_p->isTappedtoLeft = false;

    if (joystick_p->leftState == notleft)
    {
        if (joystick_p->x < LEFT_THRESHOLD)
        {

            joystick_p->isTappedtoLeft = true;
            joystick_p->leftState = left;
        }

    }
    else
    {
        if (joystick_p->x > LEFT_THRESHOLD)
        {
            joystick_p->leftState = notleft;
        }
    }

    return (joystick_p->isTappedtoLeft);


}

bool Joystick_isPressedUp(Joystick *joystick_p)
{
    return (joystick_p->y > UP_THREHOLD);
}

bool Joystick_isTappedToUp(Joystick *joystick_p)
{
    joystick_p->isTappedtoUp = false;

    if(joystick_p->upState == notup)
    {
      if(joystick_p->y > UP_THREHOLD){
          joystick_p->isTappedtoUp = true;
          joystick_p->upState = up;

      }
    }
    else
    {
        if(joystick_p->y < UP_THREHOLD)
        {
            joystick_p->upState = notup;
        }
    }
    return (joystick_p->isTappedtoUp);
}

bool Joystick_isPressedDown(Joystick *joystick_p){
    return (joystick_p->y < DOWN_THREHOLD);
}

bool Joystick_isTappedToDown(Joystick *joystick_p){

    joystick_p->isTappedtoDown = false;

    if(joystick_p->downState == notdown){
        if(joystick_p->y < DOWN_THREHOLD){
            joystick_p->isTappedtoDown = true;
            joystick_p->downState = down;
        }
    }
    else {
        if(joystick_p->y > DOWN_THREHOLD){
            joystick_p->downState = notdown;
        }
    }

    return(joystick_p->isTappedtoDown);
}
bool Joystick_isPressedRight(Joystick *joystick_p){

    return(joystick_p->x > RIGHT_THREHOLD);

}

bool Joystick_isTappedToRight(Joystick *joystick_p){

    joystick_p->isTappedtoRight = false;

    if(joystick_p->rightState == notright){

        if(joystick_p->x < RIGHT_THREHOLD){

            joystick_p->isTappedtoRight = true;
            joystick_p->rightState = right;
        }
    }
    else {
        if(joystick_p->x > RIGHT_THREHOLD){
            joystick_p->rightState = notright;

        }
    }
    return(joystick_p->isTappedtoRight);
}
