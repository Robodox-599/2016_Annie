/*
 * Autonomous.h
 *
 *  Created on: Jan 21, 2016
 *      Author: Admin
 */

#ifndef AUTONOMOUS_AUTONOMOUS_H_
#define AUTONOMOUS_AUTONOMOUS_H_
#include "../Drive/Drive.h"

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



#endif /* AUTONOMOUS_AUTONOMOUS_H_ */
