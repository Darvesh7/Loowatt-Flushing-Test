#include "mbed.h"
const int MAX_TEST_MOTORS = 2;

//#define startbutton PA_0
//Relays to control DC MOTOR
//When relay is HIGH, DC MOTOR Moving, LOW DC MOTOR STOP.

const PinName relays[MAX_TEST_MOTORS] = {LED1, PA_15};
const PinName countSwitches[MAX_TEST_MOTORS] = {BUTTON1, PA_1};
const PinName buttons[MAX_TEST_MOTORS] = {PC_8, PC_6};

// #define relayM1 PA_13;
// #define relayM2 PA_14;
// #define relayM3 PA_15;
// #define relayM4 PB_7;
// #define relayM5 PC_14;
// #define relayM6 PC_15;
// #define relayM7 PH_0;
// #define relayM8 PH_1;
// #define relayM9 PC_2;
// #define relayM10 PC_3;


//A MICROSWITCH to count flushes, 4 times LOW = 1 whole flush, takes around 4 second for 1 whole flush
// #define coutswitchM1 PA_10;
// #define coutswitchM2 PB_3;
// #define coutswitchM3 PB_5;
// #define coutswitchM4 PB_10;
// #define coutswitchM5 PA_8;
// #define coutswitchM6 PA_9;
// #define coutswitchM7 PC_7;
// #define coutswitchM8 PB_6;
// #define coutswitchM9 PA_7;
// #define coutswitchM10 PA_6;


//Tactile Button to control motor manually is somethin is not working as expected
//One press motor in STOP MODE
//Two press motor in RUN MODE
//Press 3 seconds, MOTOR changes back to RUN MODE from ERROR MODE
//ERROR MODE - it just stop the motor from moving and display ERROR on the LCD
//Press 5 seconds, EEPROM data of this current motor is deleted

// #define buttonM1 PC_8;
// #define buttonM2 PC_6;
// #define buttonM3 PC_5;
// #define buttonM4 PA_12;
// #define buttonM5 PA_11;
// #define buttonM6 PB_12;
// #define buttonM7 PB_2;
// #define buttonM8 PB_1;
// #define buttonM9 PB_15;
// #define buttonM10 PC_14;