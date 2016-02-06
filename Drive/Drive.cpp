#include "Drive.h"
#include "Math.h"

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

	error360 = 0;
	error180 = 0;

	navX->ZeroYaw();
	gyroValue = navX->GetYaw();
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
	/*if(autoTurn == false && (angle == 180 || angle == 90 || angle == 0))
	{
		autoTurn = true;
		referenceAngle = angle;
	}

	else if(autoTurn == false && angle == 270)
	{
		autoTurn = true;
		referenceAngle = -90;//trying this out
	}*/

	//if(autoTurn == false)
	//{
		switch(angle)
		{
			case 270 :
				navX->ZeroYaw();
				referenceAngle = -90;
				autoTurn = true;
				break;
			case 180 :
			case 90 :
			case 0 :
				navX->ZeroYaw();
				referenceAngle = angle;
				autoTurn = true;
				break;
			default :
				break;
		}
	//}

}

/*float Drive::abs(float num)
{
	if(num < 0)
	{
		return -num;
	}
	return num;
}*/

void Drive::edgeCase()
{
	if(gyroValue < 0)
	{
		gyroValue += 360;
	}
}

float Drive::shortestPath()
{
	if(std::abs(error180) < std::abs(error360))//using cMath absolute value function
	{
		return error180;
	}

	return error360;
}

void Drive::setTurnSpeed(float turn)
{

	if(/*autoTurn == false &&*/ turn >= DEADZONE || turn <= -DEADZONE)
	{
		turnSpeed = turn;

		autoTurn = false;

		referenceAngle = 0;
		navX->ZeroYaw();
	}
	else if((error360 <= -.5 || error360 >= .5) && (error180 <= -.5 || error180 >= .5))//added the equal signs
	{
		turnSpeed = KP * shortestPath();
	}
	else if(gyroValue == referenceAngle || navX->GetYaw() == referenceAngle) //if((referenceAngle - gyroValue > -1 && referenceAngle - gyroValue < 1) || (referenceAngle - navX->GetYaw() > -1 && referenceAngle - navX->GetYaw() < 1))
	{
		turnSpeed = 0;
		autoTurn = false;
		navX->ZeroYaw();
		gyroValue = 0;
		referenceAngle = 0;
		//Testing relative turns for Auto Turning 2/6/16
	}
}

void Drive::setLeftMotors(float velocity)
{
	frontLeftMotor->Set(velocity);
	rearLeftMotor->Set(velocity);
}

void Drive::setRightMotors(float velocity)
{
	frontRightMotor->Set(-velocity);
	rearRightMotor->Set(-velocity);
}

float Drive::linearizeDrive(float driveInput)
{
	return driveInput * SLOPE_ADJUSTMENT;
}

void Drive::drive(float xAxis, float yAxis, int POV)
{
	gyroValue = navX->GetYaw();
	edgeCase();
	setReferenceAngle(POV);
	setForwardSpeed(xAxis);

	error360 = referenceAngle - gyroValue;
	error180 = referenceAngle - navX->GetYaw();

	setTurnSpeed(yAxis);

	setLeftMotors(linearizeDrive(forwardSpeed - turnSpeed));
	setRightMotors(linearizeDrive(forwardSpeed + turnSpeed));
}

float Drive::returnMotorInputRight()
{
	return linearizeDrive(forwardSpeed + turnSpeed);
}

float Drive::returnMotorInputLeft()
{
	return linearizeDrive(forwardSpeed - turnSpeed);
}
