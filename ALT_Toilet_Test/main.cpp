#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"

Thread thread, thread1, thread2, thread3, thread4, thread5;


volatile bool StartTest = false;
volatile int count = 0;
int flushcounter = 0;
int SysMode = 0;


int wait_len = 4000;

#define RUN_MODE 1
#define SER_MODE 2
#define OFF_MODE 3


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






void setup(void)
{
  switch (SysMode) {
  case RUN_MODE:

    break;

  case SER_MODE:

    break;

  case OFF_MODE:

    break;

  default:

    break;
  }
}

void flush_counter(void)
{
  

    /*

    if thread 1 was started
    if countswitch 1 == high
    count = count + 1
    flush number of motor 1 = count

    //check fault
    //if time > than 5 seconds & count <=4  ....fault state
    //if time < than 4 & count >= 4 .....fault state


    if thread 2 was started

    if thread 3 was started

    if thread 4 was started

    if thread 5 was started

    
     */


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

    thread.signal_set(0x00);
}



