//=====[Libraries]=============================================================

#include "windshield_wiper.h"
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration of private defines]========================================

#define MODE_HI 0.75 //Settings
#define MODE_INT 0.5
#define MODE_LO 0.25

#define DELAY_SHORT 0.33 //Settings
#define DELAY_LONG 0.66

#define PERIOD 0.02 //Motor
#define DUTY_MAX 0.110
#define DUTY_MIN 0.08

#define INCREMENT 50
#define DELAY_TIME_MS 500

//=====[Declaration of private data types]=====================================

typedef enum {
   REST,
   CYCLE,
   WAIT
} wiperState_t;

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

AnalogIn modeDial(A0);
AnalogIn delayDial(A1);
PwmOut servo(PF_9);

int wiperMode = 0; // 0-OFF | 1-LOW | 2-INT | 3-HIGH
int wiperDelay = 0; //0-SHORT | 1-MEDIUM | 2-LONG

float cycleLength = (DUTY_MAX-DUTY_MIN)/INCREMENT;
int dlayTime = DELAY_TIME_MS/INCREMENT;

wiperState_t wiperState = REST;

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

static void updateMode();
static void updateDelay();
static void cycleWipers();
static void sweep();

//=====[Implementations of public functions]===================================
void updateWipers(){
    cycleWipers();
}

int readMode(){
    return wiperMode;
}

int readDelay(){
    return wiperDelay;
}

void wiperInit(){
    servo.period(PERIOD);
    servo.write(DUTY_MAX);
}

void servoWrite(float dutyCycle){
    servo.write(dutyCycle);
}
//=====[Implementations of private functions]==================================

void updateMode(){
    float readP = modeDial.read();
    if(readP < MODE_LO){ 
        wiperMode = 0; //OFF
    }
    
    else if(readP >= MODE_LO && readP < MODE_INT){
        wiperMode = 1; //LOW
    }

    else if(readP >= MODE_INT && readP < MODE_HI){
        wiperMode = 2; //INT
    }

    else if(readP >= MODE_HI){
        wiperMode = 3; //HIGH 
    }
       
    
}

void updateDelay(){
    float readP = delayDial.read();
    if(readP < DELAY_SHORT){
        wiperDelay = 0; //SHORT
    }

    else if(readP >= DELAY_SHORT && readP < DELAY_LONG){
        wiperDelay = 1; //MEDIUM
    }

    else{
        wiperDelay = 2; //LONG
    }
}

static void cycleWipers(){

   switch(wiperState){
        case REST :
            updateMode();
            updateDelay();
           
            switch(wiperMode){
                case 0 : //uartUsb.write( "REST-OFF\r\n", 10);
                break; //Do nothing, remain in the rest state

                case 1 : //LOW Mode
                case 2 : //INT Mode
                case 3 : //HIGH Mode
                
                wiperState = CYCLE; 
                break;
           }
        break;

        case CYCLE :
            switch(wiperMode){
                case 1 : //LOW Mode
                case 2 : //INT Mode
                    //TO MAX
                    
                    sweep();

                    switch(wiperMode){
                        case 1 : wiperState = REST; 
                        break;  
                        case 2 : wiperState = WAIT; 
                        break;
                        
                    }

                break;
                case 3 : //HIGH Mode
                    
                    servo.write(DUTY_MIN);
                    delay(350);
                    servo.write(DUTY_MAX);
                    delay(350);
                    wiperState = REST;
                break;
            }
        break;
    
        case WAIT :
            switch(wiperDelay){
                case 0 :  
                delay(2000); break;
                case 1 : 
                delay(5000); break;
                case 2 :  
                delay(7000); break;
            }
            wiperState = REST;
        break;
    }
}

static void sweep(){
    float temp;

    for(int i = 0; i < INCREMENT; i++){
        temp = DUTY_MAX - i*cycleLength;
        servo.write(temp);
        delay(dlayTime);
    }

    for(int i = 0; i < INCREMENT; i++){
        temp = DUTY_MIN + i*cycleLength;
        servo.write(temp);
        delay(dlayTime);
    }
}