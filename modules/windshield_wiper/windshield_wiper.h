//=====[#include guards - begin]===============================================

#ifndef windshield_wiper_H_
#define windshield_wiper_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

int readMode(); //returns current mode 
int readDelay(); //returns current delay
void updateWipers();
void wiperInit();
void servoWrite();



//=====[#include guards - end]=================================================

#endif // _MODULE_TEMPLATE_H_