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

	//1/27/2016- 4:11 pm First test of this system. Robot moved on it's own, but PID was set in motion.
	navX->ZeroYaw();
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

float Drive::setReferenceAngle(int angle)
{
	if(navX->GetYaw() <= 0.5 && navX->GetYaw() >= -0.5)
	{
		navX->ZeroYaw();
	}

	if(angle == -1)
	{
		return 0;
	}
	else if(angle == 270)
	{
		return -90;
	}
	else if(angle == 90 || angle == 180)
	{
		return angle;
	}
	else
	{
		return 0;
	}
}

void Drive::setTurnSpeed(float turn, int POV)
{
	if(turn >= DEADZONE || turn <= -DEADZONE)
	{
		turnSpeed = turn;

		navX->ZeroYaw();
	}
	else if(navX->GetYaw() <= 0 || navX->GetYaw() >= 0)
	{
		turnSpeed = KP * (setReferenceAngle(POV) - navX->GetYaw());
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

float Drive::linearizeDrive(float driveInput)
{
	/*if(driveInput > SPEEDLIMIT || driveInput < SPEEDLIMIT)
	{
		return SPEEDLIMIT;
	}
	return driveInput;*/

	return ((driveInput * SLOPE_ADJUSTMENT) /*- SLOPE_ADJUSTMENT*/);
}

void Drive::drive(float xAxis, float yAxis, int POV)
{
	setForwardSpeed(xAxis);
	setTurnSpeed(yAxis, POV);

	setLeftMotors(linearizeDrive(forwardSpeed - turnSpeed));
	setRightMotors(linearizeDrive(forwardSpeed + turnSpeed));
}

