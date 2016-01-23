#include "Drive.h"

Drive::Drive()
{
	shifter = new DoubleSolenoid(SHIFTER_CHANNEL, SHIFTER_B);
	shifter->Set(DoubleSolenoid::kReverse);
	
	frontLeftMotor = new CANTalon(DRIVE_FRONT_LEFT_MOTOR_CHANNEL);
	rearLeftMotor = new CANTalon(DRIVE_REAR_LEFT_MOTOR_CHANNEL);
	frontRightMotor = new CANTalon(DRIVE_FRONT_RIGHT_MOTOR_CHANNEL);
	rearRightMotor = new CANTalon(DRIVE_REAR_RIGHT_MOTOR_CHANNEL);

	navX = new AHRS(SPI::Port::kMXP);
	forwardSpeed = 0;
	turnSpeed = 0;
	avgEncVal = 0;
}

Drive::~Drive()
{
	delete shifter;
	delete frontLeftMotor;
	delete rearLeftMotor;
	delete frontRightMotor;
	delete rearRightMotor;
	
	shifter = NULL;
	frontLeftMotor = NULL;
	rearLeftMotor = NULL;
	frontRightMotor = NULL;
	rearRightMotor = NULL;
}

void Drive::shift(bool highButton, bool lowButton)
{	
	if(lowButton)
		shifter->Set(DoubleSolenoid::kForward);
	else if(highButton)
		shifter->Set(DoubleSolenoid::kReverse);
}

bool Drive::getShiftState()
{
	//iflow gear
	if(shifter->Get() == DoubleSolenoid::kForward)
		return true;
	return false;
}

float Drive::getAvgEncVal()
{
	avgEncVal = (rearLeftMotor->GetEncPosition() + rearRightMotor->GetEncPosition())/2;
	return avgEncVal;
}

void Drive::setForwardSpeed(float forward)
{
	if(forward >= DEADZONE || forward <= -DEADZONE)
	{
		forwardSpeed = forward;
	}
	else
	{
		forwardSpeed = 0;
	}
}

void Drive::setTurnSpeed(float turn)
{
	if(turn >= DEADZONE || turn <= -DEADZONE)
	{
		turnSpeed = turn;
	}
	else
	{
		turnSpeed = 0;
	}
}

// TJF: Swapped motor logic to reflect physical robot configuration instead of reversing everything.

void Drive::setLeftMotors(float velocity)
{
	frontLeftMotor->Set(velocity); //frontLeftMotor->Set(-velocity);
	rearLeftMotor->Set(velocity); //rearLeftMotor->Set(-velocity);
}

void Drive::setRightMotors(float velocity)
{
	frontRightMotor->Set(-velocity); //frontRightMotor->Set(velocity);
	rearRightMotor->Set(-velocity); //rearRightMotor->Set(velocity);
}

void Drive::drive(float xAxis, float yAxis)
{
	setForwardSpeed(xAxis);
	setTurnSpeed(yAxis);

	setLeftMotors(forwardSpeed - turnSpeed);
	setRightMotors(forwardSpeed + turnSpeed);
}



