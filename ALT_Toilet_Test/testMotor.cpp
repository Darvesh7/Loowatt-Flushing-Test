#include "mbed.h"
#include "PinDetect.h"
#include "testMotor.h"

TestMotor::TestMotor(PinName motorRelay, PinName motorCounter, PinName motorPauser) : 
_motorRelayOut(new DigitalOut (motorRelay)), 
_pdMotorCounter(new PinDetect (motorCounter)), 
_PdMotorPauser(new PinDetect (motorPauser))
{
    _motorRelay = motorRelay;
    _motorCounter = motorCounter;
    _motorPauser = motorPauser;
    _motorCurrentState = STOP;

    _motorRelayOut->write(1);

    _pdMotorCounter->mode(PullUp);
    _pdMotorCounter->setAssertValue(0);
    _pdMotorCounter->attach_asserted(this, &TestMotor::_motorCounterPressed);
    _pdMotorCounter->attach_deasserted(this, &TestMotor::_motorCounterReleased);

    _PdMotorPauser->mode(PullUp);
    _PdMotorPauser->attach_asserted(this, &TestMotor::_motorPauserPressed);
    _PdMotorPauser->attach_deasserted(this, &TestMotor::_motorPauserReleased);
    _PdMotorPauser->attach_asserted_held(this, &TestMotor::_motorPauserHeld);

    _pdMotorCounter->setSampleFrequency(20000);
    _PdMotorPauser->setSampleFrequency(20000);
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
}