#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"
#include "testMotor.h"

TestMotor* testMotors[MAX_TEST_MOTORS];
EventQueue timedEvents;

int currentMotor = 0;

Serial pc(USBTX, USBRX);



void setup(void)
{
    pc.baud(115200);
    pc.printf("Starting...\r\n");

    for (int i = 0; i < MAX_TEST_MOTORS; i++)
        testMotors[i] = new TestMotor(relays[i], countSwitches[i], buttons[i]), ;
}

void stopMotor(void)
{
    pc.printf("Stopping motor %d ", currentMotor);
    testMotors[currentMotor]->stopMotor();
    currentMotor = currentMotor+1;
    if(currentMotor == MAX_TEST_MOTORS)
    {
        currentMotor = 0;
    }
    pc.printf("and starting motor %d\r\n", currentMotor);
    testMotors[currentMotor]->startMotor();
}

void serviceMotor(void)
{
    pc.printf("%d\r\n", testMotors[currentMotor]->getCount());
}

int main()
{
    setup();

    timedEvents.call_every(4000, stopMotor);
    timedEvents.call_every(500, serviceMotor);
    testMotors[currentMotor]->startMotor();
    timedEvents.dispatch();

    while(true)
    {
          
    }
}


// Thread thread, thread1, thread2, thread3, thread4, thread5, enablingthreads;
// EventFlags my_event_flags, my_event_thread;
// Timer timer1,timer2,timer3,timer4,timer5;


// InterruptIn startbutton (PA_0); //to start test and can be use as emergency stop.
// DigitalOut  led2(PA_1);
// DigitalOut  led3(LED3);



// volatile bool startbuttonPressed = false;
// volatile bool startbuttonEnabled = true;
// Timeout startbuttonTimeout;
// volatile int counter = 0;

// volatile bool StartTest;
// volatile int count1 = 0; //Flush counter for motor 1
// volatile int count2 = 0; //Flush counter for motor 2
// volatile int count3 = 0; //Flush counter for motor 3
// volatile int count4 = 0;
// volatile int count5 = 0;
// volatile int count6 = 0;
// volatile int count7 = 0;
// volatile int count8 = 0;
// volatile int count9 = 0;
// volatile int count10 = 0;
// int flushcounter = 0; 
// int SysMode = 0;  //STATE of Motors


// int wait_len = 4000;

// #define RUN_MODE 1  //RUN STATE
// #define SER_MODE 2  //SERVICE STATE, BASICALLY PAUSE MODE
// #define OFF_MODE 3  //EEROR STATE


// void group1_thread(void)  //motor1 and motor 2 action
// {
//     pc.printf("group1\n");
//     while(1)
//     {
        
//     timer1.start();     
//     pc.printf("relay1&2 on\n");
//     motor_relay1 = 1; //motor1 high
//     motor_relay2 = 1; //motor2 high

//     thread.wait(wait_len);//after 4 seconds

//     pc.printf("relay1&2 off\n");
//     motor_relay1 = 0; //motor1 low
//     motor_relay2 = 0; //motor2 low
//     my_event_flags.set(0x1);//set flag
//     thread1.(); //terminate this thread
//     }
// }

// void group2_thread(void)
// {
//     while(1)
//     {
//     pc.printf("group2\n");
//     my_event_flags.wait_any(0x1); //wait for flag to start action
//     pc.printf("relay3&4 on\n");
//     motor_relay3 = 1;
//     motor_relay4 = 1;
//     thread.wait(wait_len);
//     //ThisThread::sleep_for(5000);
//     pc.printf("relay3&4 off\n");
//     motor_relay3 = 0;
//     motor_relay4 = 0;
//     my_event_flags.set(0x2);
//     thread2.terminate();
//     }
// }

// void group3_thread(void)
// {
//     pc.printf("group3\n");

//     while(1)
//     {
//     my_event_flags.wait_any(0x2);
//     pc.printf("relay5&6 on\n");
//     motor_relay5 = 1;
//     motor_relay6 = 1;
//     thread.wait(wait_len);
//     //ThisThread::sleep_for(5000);
//     pc.printf("relay5&6 off\n");
//     motor_relay5 = 0;
//     motor_relay6 = 0;
//     my_event_flags.set(0x3);
//     thread3.terminate();
//     }
// }

// void group4_thread(void)
// {
//     while(1)
//     {
//     pc.printf("group4\n");
//     my_event_flags.wait_any(0x3);
//     pc.printf("relay7&8 on\n");
//     motor_relay7 = 1;
//     motor_relay8 = 1;
//     thread.wait(wait_len);
//     //ThisThread::sleep_for(5000);
//     pc.printf("relay7&8 off\n");
//     motor_relay7 = 0;
//     motor_relay8 = 0;
//     my_event_flags.set(0x4);
//     thread4.terminate();
//     }
// }


// void group5_thread(void)
// {
//     while(1)
//     {
//     pc.printf("group5\n");    
//     my_event_flags.wait_any(0x4);
//     pc.printf("relay9&10 on\n");
//     motor_relay9 = 1;
//     motor_relay10 = 1;
//     thread.wait(wait_len);
//     //ThisThread::sleep_for(5000);
//     pc.printf("relay9&10 off\n");
//     motor_relay9 = 0;
//     motor_relay10 = 0;
//     my_event_flags.set(0x0);
//     group1_thread();
//     thread5.terminate();
//     }    
// }

// void enabling_tasks_thread(void)
// {
//     while(1)
//     {
//         pc.printf("restarting threads\n");

//         my_event_flags.wait_any(0x0); //After the 10th motor test has been done
//         //restart all the threads again.
//         thread1.start(group1_thread);
//         thread2.start(group2_thread);
//         thread3.start(group3_thread);
//         thread4.start(group4_thread);
//         thread5.start(group5_thread);
//     }
    
// } 


// void motor1_pause(void)
// {


// }

// void motor1_resume(void)
// {


// }


// void motor1_reset(void)
// {

// }

// void motor2_pause(void)
// {


// }

// void motor2_resume(void)
// {


// }


// void motor2_reset(void)
// {

// }


// void motor3_pause(void)
// {


// }

// void motor3_resume(void)
// {


// }


// void motor3_reset(void)
// {

// }


// void motor4_pause(void)
// {


// }

// void motor4_resume(void)
// {


// }


// void motor4_reset(void)
// {

// }


// void setup_motor_button(void)
// {
//     motor_button1.mode(PullUp);
//     //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
//     motor_button1.attach_deasserted(&motor1_pause); //if motor1 button is pressed and immediately released 
//     motor_button1.attach_deasserted_held(&motor1_reset); //if motor1 button is released after 5 seconds
//     motor_button1.setSampleFrequency(50000);//duration of button release


//     motor_button2.mode(PullUp);
//     //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
//     motor_button2.attach_deasserted(&motor2_pause); //if motor2 button is pressed and immediately released 
//     motor_button2.attach_deasserted_held(&motor2_reset); //if motor2 button is released after 5 seconds
//     motor_button2.setSampleFrequency(50000);//duration of button release

//     motor_button3.mode(PullUp);
//     //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
//     motor_button3.attach_deasserted(&motor3_pause); //if motor3 button is pressed and immediately released 
//     motor_button3.attach_deasserted_held(&motor3_reset); //if motor3 button is released after 5 seconds
//     motor_button3.setSampleFrequency(50000);//duration of button release


//     motor_button4.mode(PullUp);
//     //motor_button1.attach_asserted(&keyPressed); //if motor1 button is pressed
//     motor_button4.attach_deasserted(&motor4_pause); //if motor4 button is pressed and immediately released 
//     motor_button4.attach_deasserted_held(&motor4_reset); //if motor4 button is released after 5 seconds
//     motor_button4.setSampleFrequency(50000);//duration of button release
// }



// // ISR (Interrupt Service Routine) to enable button when bouncing is over
// void enableButton1(void)
// {
//     startbuttonEnabled = true;
// }
 
// // ISR (Interrupt Service Rountine) handling button pressed event
// void onStartButtonPressed(void)
// {
//     if (startbuttonEnabled) // Enabled when the button is not bouncing
//     {
//         startbuttonEnabled = false; // Disable the button while it's bouncing
//         startbuttonPressed = true;  // To be read in the main loop
//        startbuttonTimeout.attach(callback(enableButton1), 0.3);    // Debounce time 300 ms
//         counter++;      // Increment button's "press event" counter
//     }
// }








// void setup(void)
// {
    
//   switch (SysMode) {
//   case RUN_MODE:

//     break;

//   case SER_MODE:

//     break;

//   case OFF_MODE:

//     break;

//   default:

//     break;
//   }
// }

// void flush_counter(void)
// {
  

//     /*

//     if thread 1 was started
//     if countswitch 1 == high
//     count = count + 1
//     flush number of motor 1 = count

//     //check fault
//     //if time > than 5 seconds & count <=4  ....fault state
//     //if time < than 4 & count >= 4 .....fault state


//     if thread 2 was started

//     if thread 3 was started

//     if thread 4 was started

//     if thread 5 was started

    
//      */


// }



// // main() runs in its own thread in the OS
// int main()
// {
  
//     setup();

//     thread1.start(group1_thread);
//     thread2.start(group2_thread);
//     thread3.start(group3_thread);
//     thread4.start(group4_thread);
//     thread5.start(group5_thread);
//     enablingthreads.start(enabling_tasks_thread);
    

// }



