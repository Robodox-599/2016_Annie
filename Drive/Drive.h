#ifndef DRIVE_H_
#define DRIVE_H_
#include "../Macros.h"
#include "../Operator Interface/OperatorInterface.h"
#include "AHRS.h"

class Drive
{
public:
	Drive();
	~Drive();
	
	//void shift(bool highButton, bool lowButton);
	//bool getShiftState();
	
	float setLinVelocity(float linVal);
	
	float getAvgEncVal();

	float setTurnSpeed(float turn, bool turboButton);	
	
	void setLeftMotors(float velocity);
	void setRightMotors(float velocity);

	void drive(float joyY, float joyX);

	// TJF: Test only function
	void TestSpeedController(int device_id, float velocity);

	float getGyroDashboard();
	AHRS* navX;

	CANTalon *frontLeftMotor;
	CANTalon *rearLeftMotor;
	CANTalon *frontRightMotor;
	CANTalon *rearRightMotor;

	float avgEncVal; //average encoder value is used to give approximate accurate drive during autonomous

private:

	/*CANTalon *frontLeftMotor;
	CANTalon *rearLeftMotor;
	CANTalon *frontRightMotor;
	CANTalon *rearRightMotor;*/

	//moved drive motors in order to make it easier to print out values to smart dash board

	//DoubleSolenoid *shifter;
	float leftCmd;
	float rightCmd;
};
#endif
