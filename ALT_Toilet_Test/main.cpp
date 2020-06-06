#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"

Thread thread, thread1, thread2, thread3, thread4, thread5;


volatile bool StartTest = false;
int flushcounter = 0;


int wait_len = 4000;


void group1_thread(void)
{
    motor_relay1 = 1;
    motor_relay2 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    motor_relay1 = 0;
    motor_relay2 = 0;

    thread1.signal_set(0x01);

    thread1.terminate();
    
}

void group2_thread(void)
{
    thread2.signal_wait(0x01);

    while(1)
    {
    motor_relay3 = 1;
    motor_relay4 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    motor_relay3 = 0;
    motor_relay4 = 0;
    }

    thread2.signal_set(0x02);
    thread2.terminate();
    
    
}

void group3_thread(void)
{
    thread3.signal_wait(0x02);

    while(1)
    {
    motor_relay5 = 1;
    motor_relay6 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    motor_relay5 = 0;
    motor_relay6 = 0;
    }

    thread3.signal_wait(0x03);
    thread3.terminate();
    
}

void group4_thread(void)
{
    thread3.signal_wait(0x03);

    while(1)
    {
    motor_relay7 = 1;
    motor_relay8 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    motor_relay7 = 0;
    motor_relay8 = 0;
    }

    thread4.signal_wait(0x04);
    thread4.terminate();
}


void group5_thread(void)
{
    thread5.signal_wait(0x04);

    while(1)
    {
    motor_relay9 = 1;
    motor_relay10 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    motor_relay9 = 0;
    motor_relay10 = 0;
    }

    thread1.start(group1_thread);
    thread5.terminate();
}



void check_StartButton (void)
{
    if (startbutton == 1)
    {
        StartTest = true;

        group1_thread();   

    }
}

void check_StopButton (void)
{
   

}






void main_action(void)
{




}



// main() runs in its own thread in the OS
int main()
{
    buttons_setup();
    startbutton.rise(&check_StartButton);

    thread1.start(group1_thread);
    thread2.start(group2_thread);
    thread3.start(group3_thread);
    thread4.start(group4_thread);
    thread5.start(group5_thread);

    thread2.signal_set(0x00);
}



