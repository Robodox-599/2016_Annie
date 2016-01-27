#include "Macros.h"
#include "Drive/Drive.h"
#include "Manipulator/Manipulator.h"
#include "Manipulator/ManipArm.h"
#include "Catapult/Catapult.h"
#include "Operator Interface/OperatorInterface.h"
#include "Linear Drive/LinearDrive.h"

/*
 *Systems Check 1/14/16 all thing working
 */

class Annie: public IterativeRobot
{
private:
	Drive *drive;
	Manipulator *manip;
	ManipArm* manipArm;
	Catapult *catapult;
	OperatorInterface *oi;
	Command *autonomousCommand; //an object required to make autonomous button in the smart dashboard
	SendableChooser *chooser; //all the naming just follows the tutorial: http://wpilib.screenstepslive.com/s/4485/m/26401/l/255419-choosing-an-autonomous-program-from-smartdashboard
	LinearDrive *linearDrive;
	AHRS *navX;

	void RobotInit()
	{
		manipArm = new ManipArm();
		drive = new Drive();
		manip = new Manipulator();
		// TJF: new catapult constructor needs pointer to an OperatorInterface
		//catapult = new Catapult();
		oi = new OperatorInterface();
		catapult = new Catapult(oi);
		chooser = new SendableChooser();
		navX = new AHRS(SPI::Port::kMXP);
		linearDrive = new LinearDrive(navX);

	}

	void AutonomousInit()
	{
		autonomousCommand = (Command *) chooser->GetSelected(); //Sends which autonomous was chosen
		autonomousCommand->Start();
	}

	void AutonomousPeriodic()
	{
		Scheduler::GetInstance()->Run(); //runs scheduled autonomous from auto init
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		manip->toggleCompressor(oi->joyDrive->GetRawButton(COMPRESSOR_BUTTON));

		//drive
		drive->drive(oi->joyDrive->GetRawAxis(FORWARD_Y_AXIS), oi->joyDrive->GetRawAxis(TURN_X_AXIS));
		drive->shift(oi->joyDrive->GetRawButton(8), oi->joyDrive->GetRawButton(9));

		//manipulator
		// TJF: Replaced "magic numbers" with named constants
		manipArm->moveArm(oi->joyDrive->GetRawButton(UP_INTAKE_BUTTON), oi->joyDrive->GetRawButton(DOWN_INTAKE_BUTTON)); //manipArm->moveArm(oi->joyDrive->GetRawButton(6), oi->joyDrive->GetRawButton(7));
		manip->intakeBall(oi->joyDrive->GetRawButton(INTAKE_BUTTON), oi->joyDrive->GetRawButton(OUTTAKE_BUTTON), (oi->joyDrive->GetThrottle()+1)/2);

		// TJF: removed only because it doesn't work yet
		catapult->launchBall();
		printSmartDashboard(); // 01/18/2016 moved this function because it needed to be updated constantly instead of initializing it only once.

	}

	void printSmartDashboard()
	{
		SmartDashboard::PutNumber("Drive Y-Value", oi->joyDrive->GetRawAxis(FORWARD_Y_AXIS)); //oi->getDashboard()->PutNumber("Drive Y-Value", oi->joyDrive->GetRawAxis(FORWARD_Y_AXIS));
		SmartDashboard::PutNumber("Drive X-Value", oi->joyDrive->GetRawAxis(TURN_X_AXIS));//oi->getDashboard()->PutNumber("Drive X-Value", oi->joyDrive->GetRawAxis(TURN_X_AXIS));

		SmartDashboard::PutBoolean("Compressor On?", manip->compressorState()); //oi->getDashboard()->PutBoolean("Compressor On?", manip->compressorState());
		SmartDashboard::PutNumber("Gyro Value", drive->navX->GetAngle());

		//chooser->AddDefault("Auto 1", /*new Autonomous1()*/); //need many autonomous as we need
		//chooser->AddDefault("Auto 2", /*new Autonomous2()*/);
		SmartDashboard::PutData("Autonomous Modes", chooser); //not displaying because there are no input
		SmartDashboard::PutNumber("Calculated Rotate", linearDrive->CalculateRotValue(0, 1)); //added CalculateRotValue to Smart Dashboard to test for value while driving (1/25/16)
		SmartDashboard::PutBoolean("Test", true); //oi->getDashboard()->PutBoolean("Compressor On?", manip->compressorState());

	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Annie);
