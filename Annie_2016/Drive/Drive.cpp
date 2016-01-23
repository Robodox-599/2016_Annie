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
	leftCmd = 0;
	rightCmd = 0;
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

float Drive::setLinVelocity(float linVal)
{
	if(linVal > DEADZONE)
		return linVal;
	else if(linVal < -DEADZONE)
		return linVal;
	else 
		return 0; //NEUTRAL
}

float Drive::setTurnSpeed(float turn, bool turboButton)
{
	if((turn > DEADZONE && !turboButton) || (turn < -DEADZONE && !turboButton)) 
		return turn * REDUCTION;
	if(turn < DEADZONE && turn > -DEADZONE) 
		return 0; //NEUTRAL
	if((turn > DEADZONE && turboButton) || (turn < -DEADZONE && turboButton)) 
		return turn;
	//Needed a default return value even though all possibilities are covered
	return 0;
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

void Drive::drive(float joyY, float joyX)
{
	leftCmd  = setLinVelocity(joyY - joyX); //leftCmd = setLinVelocity(joyY +  joyX);
	rightCmd = setLinVelocity(joyY + joyX); //rightCmd = setLinVelocity(joyY - joyX);
	
	setLeftMotors(leftCmd);
	setRightMotors(rightCmd);
}

// TJF: Test only function, not needed anymore.
void Drive::TestSpeedController(int device_id, float velocity)
{
	CANTalon* uut;

	switch(device_id)
	{
	case DRIVE_FRONT_LEFT_MOTOR_CHANNEL:
		uut = frontLeftMotor;
		break;
	case DRIVE_REAR_LEFT_MOTOR_CHANNEL:
		uut = frontLeftMotor;
		break;
	case DRIVE_FRONT_RIGHT_MOTOR_CHANNEL:
		uut = frontLeftMotor;
		break;
	case DRIVE_REAR_RIGHT_MOTOR_CHANNEL:
		uut = frontLeftMotor;
		break;
	default:
		return;
	}

	uut->Set(velocity);
}
