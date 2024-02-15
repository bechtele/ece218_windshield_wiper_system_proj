//=====[Libraries]=============================================================

#include "system.h"
#include "engine.h"
#include "windshield_wiper.h"
#include "display.h"
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

InterruptIn engineButtonInterupt(BUTTON1);

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

static void displaySetup(); //Format display with mode and delay time selector feature
static void displayUpdate(); //Update the current mode (HI, LO, INT, OFF) and current delay (Short, Medium, Long)
static void engineButtonCallback();


//=====[Implementations of public functions]===================================

void systemInit(){
    engineInit();
    displayInit();
    displaySetup();
    wiperInit();

    engineButtonInterupt.rise(&engineButtonCallback);
}

void systemUpdate(){
    engineUpdate();
    displayUpdate();
    if(readEngine()){
        updateWipers();
    }
}


//=====[Implementations of private functions]==================================
static void displaySetup(){
    displayCharPositionWrite(0,0);
    displayStringWrite("Mode:");

    displayCharPositionWrite(0,1);
    displayStringWrite("Delay:");
}


static void displayUpdate(){
    switch(readMode()){
        case 0 : 
        displayCharPositionWrite(5,0);
        displayStringWrite("OFF "); break;
        case 1 : 
        displayCharPositionWrite(5,0);
        displayStringWrite("LOW "); break;
        case 2 :
        displayCharPositionWrite(5,0);
        displayStringWrite("INT "); break;
        case 3 :
        displayCharPositionWrite(5,0);
        displayStringWrite("HIGH"); break;
        default : 
        displayCharPositionWrite(5,0);
        displayStringWrite("!!! "); break;
    }

    switch(readDelay()){
        case 0 :
        displayCharPositionWrite(6,1);
        displayStringWrite("SHORT "); break;
        case 1 : 
        displayCharPositionWrite(6,1);
        displayStringWrite("MEDIUM"); break;
        case 2 :
        displayCharPositionWrite(6,1);
        displayStringWrite("LONG  "); break;
        default: 
        displayCharPositionWrite(6,1);
        displayStringWrite("!!!   "); break;
    }
}


static void engineButtonCallback()
{
    if(readEngine()){
        toggleEngine();
    }
}

