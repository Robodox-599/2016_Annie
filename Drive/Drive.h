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
	
	void drive(float xAxis, float yAxis, int POV);

	void shift(bool highButton, bool lowButton);
	bool getShiftState();

	float getAvgEncVal();
	
	void setForwardSpeed(float forward);
	void setTurnSpeed(float turn);
	
	void setLeftMotors(float velocity);
	void setRightMotors(float velocity);

	float getGyroDashboard();

	float linearizeDrive(float driveInput);
	void setReferenceAngle(int angle);
	void edgeCase();
	float shortestPath();

	float returnMotorInputRight();
	float returnMotorInputLeft();
	//float abs(float num);

	AHRS* navX;

	CANTalon *frontLeftMotor;
	CANTalon *rearLeftMotor;
	CANTalon *frontRightMotor;
	CANTalon *rearRightMotor;

	float avgEncVal; //average encoder value is used to give approximate accurate drive during autonomous

	bool autoTurn;

	float error360;
	float error180;

	int referenceAngle;

private:

	/*CANTalon *frontLeftMotor;
	CANTalon *rearLeftMotor;
	CANTalon *frontRightMotor;
	CANTalon *rearRightMotor;*/

	//moved drive motors in order to make it easier to print out values to smart dash board

	DoubleSolenoid *shifter;

	float forwardSpeed;
	float turnSpeed;

	float gyroValue;
};
#endif
