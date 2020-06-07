#ifndef MBED_TESTMOTOR_H
#define MBED_TESTMOTOR_H

#include "mbed.h"
#include "PinDetect.h"


typedef enum
{
    RUN,
    SER,
    STOP
} motorState_t;//use anytime

class TestMotor
{
    public:

    TestMotor(PinName motorRelay, PinName motorCounter, PinName motorPauser);

    void startMotor(void);
    void stopMotor(void);
    
    motorState_t getState(void);
    void setState(motorState_t state);

    uint32_t getCount(void);

    private:
    uint32_t _rotationCount = 0;
//counting month

    PinName _motorRelay;
    PinName _motorCounter;
    PinName _motorPauser;

    DigitalOut* _motorRelayOut;
    PinDetect* _pdMotorCounter;
    PinDetect* _PdMotorPauser;//pause motor

    motorState_t _motorCurrentState;
    // pin detect used callbacks
    void _motorCounterPressed (void);
    void _motorCounterReleased (void);

    void _motorPauserPressed(void);
    void _motorPauserReleased(void);
    void _motorPauserHeld(void);
};

#endif //