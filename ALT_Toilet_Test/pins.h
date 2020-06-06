#include "mbed.h"
#include <PinDetect.h>



InterruptIn startbutton (PA_0); //to start test and can be use as emergency stop.

//Relays to control DC MOTOR
//When relay is HIGH, DC MOTOR Moving, LOW DC MOTOR STOP.
DigitalOut motor_relay1 (PA_0);
DigitalOut motor_relay2 (PA_0);
DigitalOut motor_relay3 (PA_0);
DigitalOut motor_relay4 (PA_0);
DigitalOut motor_relay5 (PA_0);
DigitalOut motor_relay6 (PA_0);
DigitalOut motor_relay7 (PA_0);
DigitalOut motor_relay8 (PA_0);
DigitalOut motor_relay9 (PA_0);
DigitalOut motor_relay10 (PA_0);

//A MICROSWITCH to count flushes, 4 times LOW = 1 whole flush, takes 4 second for 1 whole flush
InterruptIn countswitch1 (PA_0);
InterruptIn countswitch2 (PA_0);
InterruptIn countswitch3 (PA_0);
InterruptIn countswitch4 (PA_0);
InterruptIn countswitch5 (PA_0);
InterruptIn countswitch6 (PA_0);
InterruptIn countswitch7 (PA_0);
InterruptIn countswitch8 (PA_0);
InterruptIn countswitch9 (PA_0);
InterruptIn countswitch10 (PA_0);


//Tactile Button to control motor manually is somethin is not working as expected
//One press motor in STOP MODE
//Two press motor in RUN MODE
//Press 3 seconds, MOTOR changes back to RUN MODE from ERROR MODE
//ERROR MODE - it just stop the motor from moving and display ERROR on the LCD
//Press 5 seconds, EEPROM data of this current motor is deleted

PinDetect motor_button1(PB_3);
PinDetect motor_button2(PB_3);
PinDetect motor_button3(PB_3);
PinDetect motor_button4(PB_3);
PinDetect motor_button5(PB_3);
PinDetect motor_button6(PB_3);
PinDetect motor_button7(PB_3);
PinDetect motor_button8(PB_3);
PinDetect motor_button9(PB_3);
PinDetect motor_button10(PB_3);


void buttons_setup(void)
{
    startbutton.mode(PullUp);
    
    countswitch1.mode(PullUp); 
    countswitch2.mode(PullUp);
    countswitch3.mode(PullUp);
    countswitch4.mode(PullUp);
    countswitch5.mode(PullUp);
    countswitch6.mode(PullUp);
    countswitch7.mode(PullUp);
    countswitch8.mode(PullUp);
    countswitch9.mode(PullUp);
    countswitch10.mode(PullUp);

    wait_ms(100);
}

