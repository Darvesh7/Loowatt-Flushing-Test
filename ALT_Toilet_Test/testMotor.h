#ifndef MBED_TESTMOTOR_H
#define MBED_TESTMOTOR_H

#include "mbed.h"
#include "PinDetect.h"
#include "eeprom.h"



typedef enum
{
    RUN,
    SER,
    STOP
} motorState_t;

class TestMotor
{
    public:

    TestMotor(PinName motorRelay, PinName motorCounter, PinName motorPauser, uint32_t address);

    void startMotor(void);
    void stopMotor(void);
    
    motorState_t getState(void);
    void setState(motorState_t state);

    uint32_t getCount(void);

    void readEEPROM(void);
    void writeEEPROM(void);
    void clearEEPROM(void);

    private:
    uint32_t _rotationCount = 0;

    PinName _motorRelay;
    PinName _motorCounter;
    PinName _motorPauser;
    uint32_t _address;

    DigitalOut* _motorRelayOut;
    PinDetect* _pdMotorCounter;
    PinDetect* _PdMotorPauser;
    EEPROM* _pdMotorData;
  

    motorState_t _motorCurrentState;
    // pin detect used callbacks
    void _motorCounterPressed (void);
    void _motorCounterReleased (void);

    void _motorPauserPressed(void);
    void _motorPauserReleased(void);
    void _motorPauserHeld(void);
};

#endif //