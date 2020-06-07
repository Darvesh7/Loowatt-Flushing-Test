#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"
#include "testMotor.h"
#include "eeprom.h"

#define EEPROM_ADDR 0x01
#define SDA PB_9
#define SCL PB_4
#define ONE_FLUSH 4

#define RUN_MODE 1
#define SER_MODE 2
#define OFF_MODE 3

TestMotor* testMotors[MAX_TEST_MOTORS];
EventQueue timedEvents;

int currentMotor = 0;
int evenMotor = 1;


EEPROM ep(SDA, SCL, EEPROM_ADDR, EEPROM::T24C512);


Serial pc(USBTX, USBRX);
bool gloreseteeprom = false;
int SysMode = 0;


void reset_eeprom(void)
{
    gloreseteeprom = 1;
}



void setup(void)
{
    pc.baud(115200);
    pc.printf("Starting...\r\n");

    int eeprom_size = ep.getSize();
    int flush_count = 0;
    int8_t initep = 0;
    int8_t rsysmode = 0;
    ep.read((uint32_t)(0), (int16_t &)initep);//read address 0 of eeprom
    pc.printf("initeo %d\n", initep);
    
    if (initep == 0) // if address 0 is empty, it is new eeprom
    {
    ep.write((uint32_t)0, 1); //if eeprom is new, write 1 to address 0. 
    ep.write((uint32_t)1, (int8_t)RUN_MODE);//Puts unit in RUN_MODE (Writing 1 to address 1)
    }
    else
    {
    ep.read((uint32_t)1, (int16_t &)rsysmode);//if eeprom is not new, read the last state
    ep.read((uint32_t)2, flush_count);//read the number of flushes
    }

    SysMode = rsysmode;

    switch(SysMode){
        case RUN_MODE:
            pc.printf("In RunningMode\n");


            break;
        case SER_MODE:
            pc.printf("In ServiceMode\n");

            break;
        case OFF_MODE:
            pc.printf("In offMode\n");

            break;
        default:

            break;
        }
    

    for (int i = 0; i < MAX_TEST_MOTORS; i++)
        testMotors[i] = new TestMotor(relays[i], countSwitches[i], buttons[i]);//add eeprom addr

}

void stopMotor(void)
{
    pc.printf("Stopping motor %d ", currentMotor);
    pc.printf("Stopping motor %d ", evenMotor);
    testMotors[currentMotor]->stopMotor();
    testMotors[evenMotor]->stopMotor();

    currentMotor = currentMotor+2;
    evenMotor = evenMotor+2;
    if(currentMotor == MAX_TEST_MOTORS)
    {
        currentMotor = 0;
        evenMotor = 1;
    }
    pc.printf("and starting motor %d ", currentMotor);
    pc.printf("and starting motor %d\r\n", evenMotor);
    testMotors[currentMotor]->startMotor();
    testMotors[evenMotor]->startMotor();
}

void serviceMotor(void)
{
    pc.printf("%d\r\n", testMotors[currentMotor]->getCount());//lcd
   

    pc.printf("%d\r\n", testMotors[evenMotor]->getCount());//lcd
}


  

int main()
{
    setup();

    timedEvents.call_every(4000, stopMotor);
    timedEvents.call_every(500, serviceMotor);
    testMotors[currentMotor]->startMotor();
    testMotors[evenMotor]->startMotor();
    timedEvents.dispatch();

    while(true)
    {
    sleep();
    if (gloreseteeprom)
    {
        ep.clear();
        gloreseteeprom = false;
    }
    //write stuff to eeprom here



    }
}

