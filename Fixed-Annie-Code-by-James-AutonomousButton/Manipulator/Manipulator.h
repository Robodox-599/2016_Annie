#ifndef MANIPULATOR_H_
#define MANIPULATOR_H_

#include "../Macros.h"
#include "../Operator Interface/OperatorInterface.h"
#include "ManipArm.h"

class Manipulator
{
public:
	Manipulator();
	~Manipulator();

	void intakeBall(bool intake, bool outtake, double speed);
	void toggleCompressor(bool start);
	bool compressorState();

	Compressor* comp599;

private:
	CANTalon *intakeRoller;

	int count;
	int step;
	bool compState;
};

#endif
