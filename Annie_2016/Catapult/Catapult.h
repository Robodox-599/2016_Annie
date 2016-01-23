#ifndef CATAPULT_H_
#define CATAPULT_H_
#include "../Macros.h"
#include "../Operator Interface/OperatorInterface.h"
#include "../Manipulator/ManipArm.h"
	
class Catapult
{
public:
	Catapult();
	// TJF: This class needed a pointer to the OperatorInterface.
	//		Default constructor SHOULD NOT BE USED unless you re-work everything else
	Catapult(OperatorInterface *the_oi);
	~Catapult();
	
	void launchBall();
	
	void autoFirstLaunch();
	void autoSecondLaunch();
	void autoReset();

	// TJF: Just a test function, not needed for actual robot operation
	void Test_Motor(float velocity);
		
	CANTalon *choochooTalon;
	int launchState;
	int autoLaunchState;
	Timer *timer;
	
private:
	ManipArm* armManip;
	OperatorInterface* oi;
	
	double initTime;
	double currentTime;
	bool init;
	bool autonInit1;
	bool autonInit2;
	bool lastPulse;
	bool lastPressed;
	// TJF: These needed to be booleans since they were used as true/false flags
	bool fire; //float fire;
	bool safety; //float safety;
	bool killSwitchB;
	bool killSwitchA;
	
};
#endif
