#include "Autonomous.h"

//set autoType to autoNum so when Autonomous is initialized the parameter will tell us which autonomous to run
Autonomous::Autonomous(int autoNum)
{
	autonomousIsOn = false;
	autoType = autoNum;
	drive = new Drive();

	chooseAuto();
}

/*Autonomous::Autonomous(bool goStraight)
{
	autonomousIsOn = false;
	autoType = 1;
	drive = new Drive();
}*/

Autonomous::~Autonomous()
{

}

void Autonomous::goStraightAutonomous()
{
	autonomousIsOn = true; //TODO: does this require while loop?

	if(drive->avgEncVal < 5000) //drives for 4-5 wheel rotation
	{
		drive -> drive(0.5, 0.5);
	}
	else
	{
		drive -> drive(0,0);
	}
}

void Autonomous::randomAutonomous()
{
	autonomousIsOn = true;

	if(drive->avgEncVal < 5000) //turns for 4-5 wheel rotation
	{
		drive -> drive(0.5, -0.5);
	}
	else
	{
		drive -> drive(0,0);
	}
}

void Autonomous::chooseAuto()
{
	if(autoType == 0)
	{
		void goStraightAutonomus();
	}
	if(autoType == 1)
	{
		void randomAutonomous();
	}
}
