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

	autoTurn = false;

	referenceAngle = 0;

	forwardSpeed = 0;
	turnSpeed = 0;
	avgEncVal = 0;

	status = 0;
	navX->ZeroYaw();
	gyroValue = navX->GetYaw();
	//1/27/2016- 4:11 pm First test of this system. Robot moved on it's own, but PID was set in motion.

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

void Drive::setReferenceAngle(int angle)
{
	if(autoTurn == false && (angle == 270 || angle == 180 || angle == 90 || angle == 0))
	{
		autoTurn = true;
		referenceAngle = angle;
	}
	/*else if(angle != -1)//try taking this out
	{
		referenceAngle = 0;//took this out (2/2/2016) still need ot test if this works - Carleton
	}*/
}

float abs(float num)
{
	if(num < 0)
	{
		return -num;
	}
	return num;
}

//TODO: add a function for calculating for edges cases in gyro and compensating for them
void Drive::edgeCase()
{
	if(gyroValue < 0)
	{
		gyroValue += 360;
	}
}

float Drive::shortestPath()
{
	if(abs(referenceAngle - navX->GetYaw()) < abs(referenceAngle - gyroValue))
	{
		return referenceAngle - navX->GetYaw();
	}

	return referenceAngle - gyroValue;
}

void Drive::setTurnSpeed(float turn)
{
	if((turn >= DEADZONE && autoTurn == false) || (turn <= -DEADZONE && autoTurn == false)) //changed this from : turn >= or turn <= or autoTurn
	{
		turnSpeed = turn;

		referenceAngle = 0;
		navX->ZeroYaw();
	}
	else if(referenceAngle - gyroValue <= -1 || referenceAngle - gyroValue >= 1)//added the equal signs
	{
		turnSpeed = KP * shortestPath();
	}
	else if(referenceAngle - gyroValue > -1 && referenceAngle - gyroValue < 1)
	{
		turnSpeed = 0;//since joystick is checked above, we can set turnSpeed to 0 since we know that nothing can happen with joystick and there is no error
		autoTurn = false;//did this because some how autoTurn was set to false
	}
	//took out the else statement it had nothing in it
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
	gyroValue = navX->GetYaw();
	edgeCase();
	setReferenceAngle(POV);
	setForwardSpeed(xAxis);
	setTurnSpeed(yAxis);

	setLeftMotors(linearizeDrive(forwardSpeed - turnSpeed));
	setRightMotors(linearizeDrive(forwardSpeed + turnSpeed));
}

