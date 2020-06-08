#include "mbed.h"
#include <PinDetect.h>
#include "pins.h"
#include "testMotor.h"
#include "eeprom.h"
#include "TextLCD.h"

#define EEPROM_ADDR 0x00
#define SDA PB_9
#define SCL PB_8
#define LCD_ADDR 0x4E

#define RUN_MODE 1
#define SER_MODE 2
#define OFF_MODE 3

InterruptIn StartButton(BUTTON1);

I2C i2c_lcd(SDA, SCL);
//TextLCD_I2C lcd(&i2c_lcd, LCD_ADDR, TextLCD::LCD20x4);

TestMotor* testMotors[MAX_TEST_MOTORS];
EventQueue timedEvents, StartQueue;

int oddMotor = 0;
int evenMotor = 1;


EEPROM* ep; 


Serial pc(USBTX, USBRX);

Thread EEROR;


void setup(void)
{
    pc.baud(115200);
    pc.printf("Starting...\r\n");

    ep = new EEPROM(SDA, SCL, EEPROM_ADDR, EEPROM::T24C512);
    lcd = new TextLCD_I2C (&i2c_lcd, LCD_ADDR, TextLCD::LCD20x4);

    int y = 1;
    int x = 0;

    for (int i = 0; i < MAX_TEST_MOTORS; i++)                                                           //Column         //Row in inverted
    testMotors[i] = new TestMotor(relays[i], countSwitches[i], buttons[i], ep, (uint32_t)i*5 + 1, lcd, lcdBaseCol[i], lcdBaseRow[i]);//add eeprom addr

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
            testMotors[i]->printLCDdata(); 
        }
    } 

}

void stopMotor(void)
{
    uint32_t rotationsMadeOddMotor = 0;
    uint32_t rotationsMadeEvenMotor = 0;

    bool FaultOddMotor = false;
    bool FaultEvenMotor = false;

    rotationsMadeOddMotor = testMotors[oddMotor]->stopMotor();
    rotationsMadeEvenMotor = testMotors[evenMotor]->stopMotor();

    FaultOddMotor = rotationsMadeOddMotor < 4;
    FaultEvenMotor = rotationsMadeEvenMotor < 4;

    testMotors[oddMotor]->setFaultState(FaultOddMotor);
    testMotors[evenMotor]->setFaultState(FaultEvenMotor);

    pc.printf("Stop motor %d %s ", oddMotor, testMotors[oddMotor]->getFaultState()? " Faulty ": " OK ");
    pc.printf("Stop motor %d %s \r\n", evenMotor, testMotors[evenMotor]->getFaultState()? " Faulty ": " OK ");


    testMotors[oddMotor]->printLCDdata(); 
    testMotors[evenMotor]->printLCDdata(); 
           
    oddMotor = oddMotor+2;
    evenMotor = evenMotor+2;
    if(oddMotor == MAX_TEST_MOTORS)
    {
        oddMotor = 0;
        evenMotor = 1;
    }
    pc.printf("Starting motor %d ", oddMotor);
    pc.printf("Starting motor %d\r\n", evenMotor);

    testMotors[oddMotor]->writeEEPROMData();
    testMotors[evenMotor]->writeEEPROMData();

    testMotors[oddMotor]->startMotor();
    testMotors[evenMotor]->startMotor();
}

void serviceMotor(void)
{
    pc.printf("%.2f\r\n", testMotors[oddMotor]->getFlushCount());//lcd
    pc.printf("%.2f\r\n", testMotors[evenMotor]->getFlushCount());//lcd
}

void startTest(void) {

timedEvents.dispatch();
}

int main()
{
    setup();
    lcd->setBacklight(TextLCD::LightOn);
    //setupLCD();
    timedEvents.call_every(4000, stopMotor);
    timedEvents.call_every(500, serviceMotor);
    testMotors[oddMotor]->startMotor();
    testMotors[evenMotor]->startMotor();

    Thread eventThread;
    eventThread.start(callback(&StartQueue, &EventQueue::dispatch_forever));

    StartButton.fall(StartQueue.event(&startTest));
 
    wait(osWaitForever);    
    
    

    while(true)
    {
   
    }
}

