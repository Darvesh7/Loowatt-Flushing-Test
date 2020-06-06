#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"

Thread thread, thread1, thread2, thread3, thread4, thread5;


bool StartTest = false;
int flushcounter = 0;

void button_callbacks(void)
{




}



void group1_thread(void)
{
    motor_relay1 = 1;
    motor_relay2 = 1;




    
}

void group2_thread(void)
{
    
}

void group3_thread(void)
{
    
}

void group4_thread(void)
{
    
}


void group5_thread(void)
{
    
}



void check_StartButton (void)
{
    if (startbutton == 1)
    {
        StartTest = true;

        

    }
}







void main_action(void)
{




}



// main() runs in its own thread in the OS
int main()
{
    buttons_setup();
    
    thread1.start(group1_thread);
    thread2.start(group2_thread);
    thread3.start(group3_thread);
    thread4.start(group4_thread);
    thread5.start(group5_thread);
}



