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
	
	void shift(bool highButton, bool lowButton);
	bool getShiftState();
	
	void setForwardSpeed(float forward);
	
	float getAvgEncVal();

	void setTurnSpeed(float turn);
	
	void setLeftMotors(float velocity);
	void setRightMotors(float velocity);

	void drive(float xAxis, float yAxis, int POV);

	// TJF: Test only function
	void TestSpeedController(int device_id, float velocity);

	float getGyroDashboard();

	float linearizeDrive(float driveInput);

	void setReferenceAngle(int angle);

	AHRS* navX;

	CANTalon *frontLeftMotor;
	CANTalon *rearLeftMotor;
	CANTalon *frontRightMotor;
	CANTalon *rearRightMotor;

	float avgEncVal; //average encoder value is used to give approximate accurate drive during autonomous

	int status;//purely for debugging purposes take out after done

	int referenceAngle;

private:

	/*CANTalon *frontLeftMotor;
	CANTalon *rearLeftMotor;
	CANTalon *frontRightMotor;
	CANTalon *rearRightMotor;*/

	//moved drive motors in order to make it easier to print out values to smart dash board

	DoubleSolenoid *shifter;

	bool autoTurn;

	float forwardSpeed;
	float turnSpeed;
};
#endif
