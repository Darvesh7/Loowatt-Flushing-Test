#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"
#include "testMotor.h"
#include "eeprom.h"

#define EEPROM_ADDR 0x00
#define SDA PB_9
#define SCL PB_8
#define ONE_FLUSH 4

#define RUN_MODE = 1;
#define SER_MODE 2
#define OFF_MODE 3



TestMotor* testMotors[MAX_TEST_MOTORS];
EventQueue timedEvents;

int currentMotor = 0;
int evenMotor = 1;

EEPROM* ep; 

Serial pc(USBTX, USBRX);
bool gloreseteeprom = false;
int SysMode = 0;

int data;

void reset_eeprom(void)
{
    gloreseteeprom = 1;
}

void setup(void)
{
    pc.baud(115200);
    pc.printf("Starting...\r\n");

    ep = new EEPROM(SDA, SCL, EEPROM_ADDR, EEPROM::T24C512);

    for (int i = 0; i < MAX_TEST_MOTORS; i++)
    testMotors[i] = new TestMotor(relays[i], countSwitches[i], buttons[i], ep, (uint32_t)i*5 + 1);//add eeprom addr

    uint8_t initep = 0xFF;
    uint8_t testdata = 0xFF; 

    ep->read((uint32_t)0, (int8_t&)initep);//read address 0 of eeprom
    pc.printf("%d", ep->getError());
    pc.printf("initep %X\n", initep);
    
    if (initep != 0x55) // if address 0 is empty, it is new eeprom
    {
        ep->write((uint32_t)0, 0x55); //if eeprom is new, write 1 to address 0.
        ep->read((uint32_t)0, (int8_t &)testdata);

        pc.printf("initeo %X\n", initep);

        for (int i = 0; i < MAX_TEST_MOTORS; i++)
        {
            testMotors[i]->writeEEPROMData();   
        }

    }
    else
    {
        for (int i = 0; i < MAX_TEST_MOTORS; i++)
        {
            testMotors[i]->readEEPROMData();   
        }
    } 

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

    testMotors[currentMotor]->writeEEPROMData();
    testMotors[evenMotor]->writeEEPROMData();

    testMotors[currentMotor]->startMotor();
    testMotors[evenMotor]->startMotor();
}

void serviceMotor(void)
{
    pc.printf("%.2f\r\n", testMotors[currentMotor]->getFlushCount());//lcd
    pc.printf("%.2f\r\n", testMotors[evenMotor]->getFlushCount());//lcd
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
   
    }
}

