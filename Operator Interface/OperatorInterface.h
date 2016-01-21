#ifndef OPERATOR_INTERFACE_H_
#define OPERATOR_INTERFACE_H_

#include "WPILib.h"
#include "../Macros.h"

class OperatorInterface
{
	
public:
	OperatorInterface();	
	~OperatorInterface();

	//SmartDashboard* getDashboard();
	
	Joystick *joyDrive;

private:
	// TJF: SmartDashboard is a singleton, so you can't create your own instance of it
	//SmartDashboard* dashboard;
};
#endif
