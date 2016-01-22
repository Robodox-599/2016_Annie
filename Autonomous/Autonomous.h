#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_
#include "../Macros.h"

class Autonomous{

public:

	Autonomous();
	Autonomous(bool goStraight); //made second constructor for second set of auto
	~Autonomous();

	void chooseAuto();
	void goStraightAutonomous(); //this autonomous was made in order to test out navx, encoder, and smartdashboard chooser
	void randomAutonomous(); //just an arbitrary autonomous to see if smartdashboard chooser work

	Drive *drive;
private:

	bool autonomousIsOn;
	int autoType;
};
