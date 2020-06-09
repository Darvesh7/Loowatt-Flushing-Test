#ifndef MBED_TESTMOTOR_H
#define MBED_TESTMOTOR_H

#include "mbed.h"
#include "PinDetect.h"
#include "eeprom.h"
#include "TextLCD.h"



typedef enum
{
    STOP,
    RUN,
    SER
} motorState_t;

class TestMotor
{
    public:

    TestMotor(PinName motorRelay, PinName motorCounter, PinName motorPauser, EEPROM* EEPROM_Handle, uint32_t baseEEPROMAddress, TextLCD_I2C * TextLCD_I2C_Handle, uint32_t baseRowAddress, uint32_t baseColumnAddress);

    void startMotor(void);
    uint32_t stopMotor(void); //return rotations

    motorState_t getState(void);
    void setState(motorState_t state);

    uint32_t getCount(void);
    float getFlushCount(void);
    float getMonthCount(void);

    void writeEEPROMData(void);
    void clearEEPROMData(void);
    void readEEPROMData(void);

    void printLCDdata(void);

    void setFaultState(bool faultyState);
    bool getFaultState(void);

    private:
    int32_t _rotationCount;
    int32_t _previousRotationCount;
    float _flushCount;
    float _runtimeInMonths;

    PinName _motorRelay;
    PinName _motorCounter;
    PinName _motorPauser;

    DigitalOut* _motorRelayOut;
    PinDetect* _pdMotorCounter;
    PinDetect* _PdMotorPauser;
    EEPROM * _EEPROM_Handle;
    TextLCD_I2C* _TextLCD_I2C_Handle;

    bool _faulty;

    uint32_t _baseEEPROMAddress;

    uint32_t _baseRowAddress;
    uint32_t _baseColumnAddress;

    motorState_t _motorCurrentState;
    // pin detect used callbacks
    void _motorCounterPressed (void);
    void _motorCounterReleased (void);

    void _motorPauserPressed(void);
    void _motorPauserReleased(void);
    void _motorPauserHeld(void);
};

#endif //