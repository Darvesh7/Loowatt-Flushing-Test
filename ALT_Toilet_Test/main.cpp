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
DigitalOut LED(LED2);

Timer faultTimer;

I2C i2c_lcd(SDA, SCL);
//TextLCD_I2C lcd(&i2c_lcd, LCD_ADDR, TextLCD::LCD20x4);

TestMotor* testMotors[MAX_TEST_MOTORS];
EventQueue timedEvents, StartQueue, PauseQueue;


int oddMotor = 0;
int evenMotor = 1;

EEPROM* ep; 

Serial pc(USBTX, USBRX);


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

float getAverageRotationLapse (uint32_t LastSwitchTime, uint32_t rotationMade)
{
    float AverageTimePerRotation = 999.99;
    if (rotationMade > 0)
    {
        AverageTimePerRotation = ((float)LastSwitchTime/(float)rotationMade);
    }

    return AverageTimePerRotation;
}


void CheckMotor(void)
{
    uint32_t rotationsMadeOddMotor = 0;
    uint32_t rotationsMadeEvenMotor = 0;
    float averageDurationOfRotationOddMotor = 0.0;
    float averageDurationOfRotationEvenMotor = 0.0;

    bool FaultOddMotor = false;
    bool FaultEvenMotor = false;

    rotationsMadeOddMotor = testMotors[oddMotor]->stopMotor();
    rotationsMadeEvenMotor = testMotors[evenMotor]->stopMotor();

    averageDurationOfRotationOddMotor = getAverageRotationLapse(testMotors[oddMotor]->LastSwitchTime, rotationsMadeOddMotor);
    averageDurationOfRotationEvenMotor = getAverageRotationLapse(testMotors[evenMotor]->LastSwitchTime, rotationsMadeEvenMotor);

    FaultOddMotor = rotationsMadeOddMotor < 4 || averageDurationOfRotationOddMotor < 495.0;
    FaultEvenMotor = rotationsMadeEvenMotor < 4 || averageDurationOfRotationEvenMotor < 495.0;

    testMotors[oddMotor]->setFaultState(FaultOddMotor);
    testMotors[evenMotor]->setFaultState(FaultEvenMotor);

    pc.printf("Stop motor %d %s %.2f ms", oddMotor, testMotors[oddMotor]->getFaultState()? " Faulty ": " OK ", averageDurationOfRotationOddMotor);
    pc.printf("Stop motor %d %s %.2f ms\r\n", evenMotor, testMotors[evenMotor]->getFaultState()? " Faulty ": " OK ", averageDurationOfRotationOddMotor);

    testMotors[oddMotor]->printLCDdata(); 
    testMotors[evenMotor]->printLCDdata(); 
           
    oddMotor = oddMotor + 2;
    evenMotor = evenMotor + 2;
    if(oddMotor == MAX_TEST_MOTORS)
    {
        oddMotor = 0;
        evenMotor = 1;
    }
    pc.printf("Starting motor %d ", oddMotor);
    pc.printf("Starting motor %d\r\n", evenMotor);

    testMotors[oddMotor]->writeEEPROMData();
    testMotors[evenMotor]->writeEEPROMData();

    faultTimer.reset();

    testMotors[oddMotor]->startMotor();
    testMotors[evenMotor]->startMotor();
}

void serviceMotor(void)
{
    pc.printf("%.2f\r\n", testMotors[oddMotor]->getMonthCount());//lcd
    pc.printf("%.2f\r\n", testMotors[evenMotor]->getMonthCount());//lcd
}

void startTest(void) 
{
    timedEvents.call_every(3600, CheckMotor); //this 4 seconds is variable.
    timedEvents.call_every(500, serviceMotor);
    testMotors[oddMotor]->startMotor();
    testMotors[evenMotor]->startMotor();
    faultTimer.start();

    timedEvents.dispatch();
}

int main()
{
    setup();
    lcd->setBacklight(TextLCD::LightOn);
  
    //setupLCD();
    Thread eventThread;
    eventThread.start(callback(&StartQueue, &EventQueue::dispatch_forever));
    StartButton.fall(StartQueue.event(&startTest));
 

    while(true)
    {
        
   
    }
}

