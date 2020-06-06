#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"

Thread thread, thread1, thread2, thread3, thread4, thread5, enablingthreads;
EventFlags my_event_flags, my_event_thread;

Serial pc(USBTX, USBRX);

volatile bool StartTest;
volatile int count = 0;
int flushcounter = 0;
int SysMode = 0;


int wait_len = 4000;

#define RUN_MODE 1
#define SER_MODE 2
#define OFF_MODE 3


void group1_thread(void)
{
    pc.printf("group1\n");
    while(1)
    { 
    pc.printf("relay1&2 on\n");
    motor_relay1 = 1;
    motor_relay2 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    pc.printf("relay1&2 off\n");
    motor_relay1 = 0;
    motor_relay2 = 0;
    my_event_flags.set(0x1);
    thread1.terminate();  
    }

    
}

void group2_thread(void)
{

    while(1)
    {
    pc.printf("group2\n");
    my_event_flags.wait_any(0x1);
    pc.printf("relay3&4 on\n");
    motor_relay3 = 1;
    motor_relay4 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    pc.printf("relay3&4 off\n");
    motor_relay4 = 0;
    my_event_flags.set(0x2);
    thread2.terminate();
    }

    
 
    
    
}

void group3_thread(void)
{
    pc.printf("group3\n");

    while(1)
    {
    my_event_flags.wait_any(0x2);
    pc.printf("relay5&6 on\n");
    motor_relay5 = 1;
    motor_relay6 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    pc.printf("relay5&6 off\n");
    motor_relay5 = 0;
    motor_relay6 = 0;
    my_event_flags.set(0x3);
    thread3.terminate();
    }

    
    
    
}

void group4_thread(void)
{
    

    while(1)
    {
    pc.printf("group4\n");
    my_event_flags.wait_any(0x3);
    pc.printf("relay7&8 on\n");
    motor_relay7 = 1;
    motor_relay8 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    pc.printf("relay7&8 off\n");
    motor_relay7 = 0;
    motor_relay8 = 0;
    my_event_flags.set(0x4);
    thread4.terminate();
    }
}


void group5_thread(void)
{

    while(1)
    {
    pc.printf("group5\n");    
    my_event_flags.wait_any(0x4);
    pc.printf("relay9&10 on\n");
    motor_relay9 = 1;
    motor_relay10 = 1;
    thread.wait(wait_len);
    //ThisThread::sleep_for(5000);
    pc.printf("relay9&10 off\n");
    motor_relay9 = 0;
    motor_relay10 = 0;
    my_event_flags.set(0x0);
    group1_thread();
    thread5.terminate();
    }    
}

void enabling_tasks_thread(void)
{
    while(1)
    {
        pc.printf("restarting threads\n");

        my_event_flags.wait_any(0x0);
        thread1.start(group1_thread);
        thread2.start(group2_thread);
        thread3.start(group3_thread);
        thread4.start(group4_thread);
        thread5.start(group5_thread);
    }
    
} 


void motor1_pause(void)
{


}

void motor1_resume(void)
{


}


void motor1_reset(void)
{

}

void motor2_pause(void)
{


}

void motor2_resume(void)
{


}


void motor2_reset(void)
{

}


void motor3_pause(void)
{


}

void motor3_resume(void)
{


}


void motor3_reset(void)
{

}


void motor4_pause(void)
{


}

void motor4_resume(void)
{


}


void motor4_reset(void)
{

}


void setup_motor_button(void)
{
    motor_button1.mode(PullUp);
    //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
    motor_button1.attach_deasserted(&motor1_pause); //if motor1 button is pressed and immediately released 
    motor_button1.attach_deasserted_held(&motor1_reset); //if motor1 button is released after 5 seconds
    motor_button1.setSampleFrequency(50000);//duration of button release


    motor_button2.mode(PullUp);
    //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
    motor_button2.attach_deasserted(&motor2_pause); //if motor2 button is pressed and immediately released 
    motor_button2.attach_deasserted_held(&motor2_reset); //if motor2 button is released after 5 seconds
    motor_button2.setSampleFrequency(50000);//duration of button release

    motor_button3.mode(PullUp);
    //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
    motor_button3.attach_deasserted(&motor3_pause); //if motor3 button is pressed and immediately released 
    motor_button3.attach_deasserted_held(&motor3_reset); //if motor3 button is released after 5 seconds
    motor_button3.setSampleFrequency(50000);//duration of button release


    motor_button4.mode(PullUp);
    //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
    motor_button4.attach_deasserted(&motor4_pause); //if motor4 button is pressed and immediately released 
    motor_button4.attach_deasserted_held(&motor4_reset); //if motor4 button is released after 5 seconds
    motor_button4.setSampleFrequency(50000);//duration of button release


}



void check_StartButton (void)
{
    if (startbutton == 1)
    {
       StartTest = true;
       // pc.printf("button pressed\n");

       // group1_thread();   

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
 

    thread1.start(group1_thread);
    thread2.start(group2_thread);
    thread3.start(group3_thread);
    thread4.start(group4_thread);
    thread5.start(group5_thread);
    enablingthreads.start(enabling_tasks_thread);

}



