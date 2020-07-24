#include "mbed.h"
#include "PinDetect.h"
#include "testMotor.h"
#include "eeprom.h"
#include "TextLCD.h"

TestMotor::TestMotor(PinName motorRelay, PinName motorCounter, PinName motorPauser, EEPROM* EEPROM_Handle, uint32_t baseEEPROMAddress, TextLCD_I2C * TextLCD_I2C_Handle, uint32_t baseRowAddress, uint32_t baseColumnAddress): 
_motorRelayOut(new DigitalOut (motorRelay)), 
_pdMotorCounter(new PinDetect (motorCounter)), 
_PdMotorPauser(new PinDetect (motorPauser))
{
    _motorRelay = motorRelay;
    _motorCounter = motorCounter;
    _motorPauser = motorPauser;
    _EEPROM_Handle = EEPROM_Handle;
    _baseEEPROMAddress = baseEEPROMAddress;

    _TextLCD_I2C_Handle = TextLCD_I2C_Handle;
    _baseColumnAddress = baseColumnAddress;
    _baseRowAddress = baseRowAddress;

    _motorCurrentState = SER;
    _rotationCount = 0;
    _flushCount = 0;
    _runtimeInMonths = 0;
    _previousRotationCount = 0;
    LastSwitchTime = 0;

    _motorRelayOut->write(1);

    _pdMotorCounter->mode(PullUp);
    _pdMotorCounter->setAssertValue(0);
    _pdMotorCounter->attach_asserted(this, &TestMotor::_motorCounterPressed);
    _pdMotorCounter->attach_deasserted(this, &TestMotor::_motorCounterReleased);

    _PdMotorPauser->mode(PullUp);
    _PdMotorPauser->setAssertValue(0);
    _PdMotorPauser->attach_asserted(this, &TestMotor::_motorPauserPressed);
    _PdMotorPauser->attach_deasserted(this, &TestMotor::_motorPauserReleased);
    _PdMotorPauser->attach_asserted_held(this, &TestMotor::_motorPauserHeld); //if button is pressed for 2 seconds and released

    _PdMotorPauser->setSamplesTillHeld( 100 );

    _pdMotorCounter->setSampleFrequency(20000);
    _PdMotorPauser->setSampleFrequency();

}

void TestMotor::readEEPROMData(void)
{
    int8_t moderead = 0;

    _EEPROM_Handle->read(_baseEEPROMAddress, (int8_t&)moderead);
    _motorCurrentState = (motorState_t) moderead;

    _EEPROM_Handle->read(_baseEEPROMAddress + 1, (int32_t &)_rotationCount);
     _previousRotationCount = _rotationCount;
}

void TestMotor::printLCDdata(void)
{
    _TextLCD_I2C_Handle ->setAddress(_baseRowAddress, _baseColumnAddress);
    _TextLCD_I2C_Handle ->printf("%3d%c", (int8_t)_runtimeInMonths, getFaultState()? '*': ' ');
}

void TestMotor::startMotor(void)
{
    if (_motorCurrentState == SER)
    {
        _motorCheckCount = 0;   
        _motorCurrentState = RUN;
        _motorRelayOut->write(0);
    }
     _previousRotationCount = _rotationCount;
}

uint32_t TestMotor::stopMotor(void)
{
    if (_motorCurrentState == RUN)
    { 

        _motorCurrentState = SER;
        _motorRelayOut->write(1);
    }
    return _rotationCount - _previousRotationCount;
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
    _flushCount = ((float)_rotationCount) / 8.0;
    return _flushCount;
}

float TestMotor::getMonthCount(void)
{
    _runtimeInMonths = ((float)_rotationCount)/ 1456.0; //Actual number is 1456
    return _runtimeInMonths;

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

void TestMotor::setFaultState(bool faultyState)
{
    _faulty = faultyState;
    if(_faulty)
    {
        _motorCurrentState = STOP;     
        
    }
}

bool TestMotor::getFaultState(void)
{
    return _faulty;
}

void TestMotor::_motorCounterPressed (void)
{
    _rotationCount = _rotationCount + 1; 
    _motorCheckCount = _motorCheckCount + 1;

    if(_motorCheckCount == 4)
    {
        _motorCheckCount = 0;
        _motorCurrentState = SER;
        _motorRelayOut->write(1);
    }
    if(_motorCurrentState==RUN)
    {
        LastSwitchTime = faultTimer.read_ms();
    }
}

void TestMotor::_motorCounterReleased (void)
{
    
}

void TestMotor::_motorPauserPressed(void)
{

  if (_motorCurrentState == SER)
    {
        _motorCurrentState = STOP;
        _motorFaultCount = 0;
    } 
    else if (_motorCurrentState == STOP) 
    {
        _motorCurrentState = SER;
        _rotationCount = _previousRotationCount;
    }
}
    
void TestMotor::_motorPauserReleased(void)
{

}

void TestMotor::_motorPauserHeld(void)
{
    _motorCurrentState = SER;
    _rotationCount = 0; //Delete eeprom of current motor
    //_rotationCount = _previousRotationCount; //Just pause the motor
}