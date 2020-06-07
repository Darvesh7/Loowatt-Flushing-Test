#include "mbed.h"
#include "PinDetect.h"
#include "testMotor.h"
#include "eeprom.h"

TestMotor::TestMotor(PinName motorRelay, PinName motorCounter, PinName motorPauser, EEPROM* EEPROM_Handle, uint32_t baseEEPROMAddress): 
_motorRelayOut(new DigitalOut (motorRelay)), 
_pdMotorCounter(new PinDetect (motorCounter)), 
_PdMotorPauser(new PinDetect (motorPauser))
{
    _motorRelay = motorRelay;
    _motorCounter = motorCounter;
    _motorPauser = motorPauser;
    _EEPROM_Handle = EEPROM_Handle;
    _baseEEPROMAddress = baseEEPROMAddress;

    _motorCurrentState = STOP;
    _rotationCount = 0;

    _motorRelayOut->write(1);

    _pdMotorCounter->mode(PullUp);
    _pdMotorCounter->setAssertValue(0);
    _pdMotorCounter->attach_asserted(this, &TestMotor::_motorCounterPressed);
    _pdMotorCounter->attach_deasserted(this, &TestMotor::_motorCounterReleased);

    _PdMotorPauser->mode(PullUp);
    _PdMotorPauser->setAssertValue(0);
    _PdMotorPauser->attach_asserted(this, &TestMotor::_motorPauserPressed);
    _PdMotorPauser->attach_deasserted(this, &TestMotor::_motorPauserReleased);
    _PdMotorPauser->attach_deasserted_held(this, &TestMotor::_motorPauserHeld);

    _pdMotorCounter->setSampleFrequency(20000);
    _PdMotorPauser->setSampleFrequency(20000);
}

void TestMotor::readEEPROMData(void)
{
    int8_t moderead = 0;

    _EEPROM_Handle->read(_baseEEPROMAddress, (int8_t&)moderead);
    _motorCurrentState = (motorState_t) moderead;

    _EEPROM_Handle->read(_baseEEPROMAddress + 1, (int32_t &)_rotationCount);
}

void TestMotor::startMotor(void)
{
    _motorCurrentState = RUN;
    _motorRelayOut->write(0);
    
}
void TestMotor::stopMotor(void)
{
    _motorCurrentState = STOP;
    _motorRelayOut->write(1);
}
    
motorState_t TestMotor::getState(void)
{
    return _motorCurrentState;
}

uint32_t TestMotor::getCount(void)
{
    return _rotationCount;
}

float TestMotor::getFlushCount(void)
{
    _flushCount = ((float)_rotationCount) / 4.0;
    return _flushCount;
}

void TestMotor::writeEEPROMData(void)
{
    _EEPROM_Handle->write(_baseEEPROMAddress, (int8_t)_motorCurrentState);
    _EEPROM_Handle->write(_baseEEPROMAddress + 1, _rotationCount);
}

void TestMotor::clearEEPROMData(void)
{
    _EEPROM_Handle->write(_baseEEPROMAddress, (int8_t)0);
    _EEPROM_Handle->write(_baseEEPROMAddress + 1, (int32_t)0);
}

void TestMotor::_motorCounterPressed (void)
{
    _rotationCount = _rotationCount + 1;
}
    
void TestMotor::_motorCounterReleased (void)
{
    
}

void TestMotor::_motorPauserPressed(void)
{
    _motorCurrentState = SER;
}
    
void TestMotor::_motorPauserReleased(void)
{

}

void TestMotor::_motorPauserHeld(void)
{
    _motorCurrentState = STOP;
    _flushCount = 0;

    writeEEPROMData();
}