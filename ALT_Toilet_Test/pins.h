#include "mbed.h"
#include "TextLCD.h"


TextLCD_I2C* lcd;

const int MAX_TEST_MOTORS = 10;

PinName relays[MAX_TEST_MOTORS] = {PA_13, PA_14, PA_15, PB_7, PA_0 , PA_1 , PC_2 , PC_3, PA_4, PB_0}; //PH_0, PH_1};
PinName countSwitches[MAX_TEST_MOTORS] = {PA_10, PB_3, PB_5, PB_4, PB_10 , PA_8 , PA_9, PC_7, PA_7, PA_6};//, PC_1, PC_0 };
PinName buttons[MAX_TEST_MOTORS] = {PC_8, PC_6, PC_5, PA_12 , PA_11 , PB_12 , PB_2 , PB_1, PB_15, PB_14};// PB_13, PC_4};


uint32_t lcdBaseCol[MAX_TEST_MOTORS] = {0, 5, 10, 15, 0, 5, 10, 15, 0, 5};// 10, 15};
uint32_t lcdBaseRow[MAX_TEST_MOTORS] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2};// 2, 2};

/* 
    PauseButton.fall(callback(pressed));  //callback function
    t.start(callback(&PauseQueue, &EventQueue::dispatch_forever));//one button to play or pause
    int id = PauseQueue.call_every(2000,handler, 5); //call the function every 2 seconds
    PauseQueue.dispatch(10000); //function starts after 10 seconds
    
    wait(osWaitForever); 
*/   
    
    

void setupLCD(void)  //column & row 
{
    lcd->setBacklight(TextLCD::LightOn);
    lcd->setAddress(0, 0);  
    lcd->printf("1.");
    lcd->setAddress(0, 1);  
    lcd->printf("2.");
    lcd->setAddress(0, 2);  
    lcd->printf("3.");
    lcd->setAddress(0, 3);  
    lcd->printf("4.");

    lcd->setAddress(7, 0);  
    lcd->printf("5.");
    lcd->setAddress(7, 1);  
    lcd->printf("6.");
    lcd->setAddress(7, 2);  
    lcd->printf("7.");
    lcd->setAddress(7, 3);  
    lcd->printf("8.");

    lcd->setAddress(14, 0);  
    lcd->printf("9.");
    lcd->setAddress(14, 1);  
    lcd->printf("10.");

}
