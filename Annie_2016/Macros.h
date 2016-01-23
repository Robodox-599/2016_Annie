#ifndef ANESTHESIOLOGIST_MACROS_H_
#define ANESTHESIOLOGIST_MACROS_H_
#include "WPILib.h"
/*
	XBOX CONTROLS
	
	forward/backward			left joystick 
	turn 						right joystick
	intake button 				A
	outtake button 				Y
	up intake 					LB
	down intake 				RB
	compressor 					B
	shift gears 				BR

*/
	
// buttons
/*
 * ****** READ THIS v *******
 * Things James Fixed (Search code for "TJF" to find all):
 * All the button assignments were off by 1 (old values left commented out for comparison)
 * Up/down intake buttons were swapped
 * ****** READ THIS ^ *******
 */

//http://first.wpi.edu/FRC/roborio/stable/docs/cpp/classJoystick.html
#define INTAKE_BUTTON							(1)//(0)
#define OUTTAKE_BUTTON							(4)//(3)
#define UP_INTAKE_BUTTON						(6)//(4)
#define DOWN_INTAKE_BUTTON						(5)//(5)
#define COMPRESSOR_BUTTON						(2)//(1)
//#define SHIFT_GEARS_BUTTON						(2)
#define FIRE_BUTTON								(3)//(2)
#define SAFETY_BUTTON							(4)//(3)
#define KILL_SWITCH_A							(6)
#define KILL_SWITCH_B							(7)

#define TURN_X_AXIS								(4)
#define FORWARD_Y_AXIS							(1)

	//Slots
#define PNEUMATICS_24V_SLOT						(1)

	//Talons
// TJF: Talon device IDs were 0, 1, 2, 3, 3, 5 (made one of them ID 4 by using http://roborio-599.local)
// 	Changed left/right motor assignments to match physical robot
#define DRIVE_FRONT_LEFT_MOTOR_CHANNEL          (4)//(0)
#define DRIVE_REAR_LEFT_MOTOR_CHANNEL           (5)//(1)
#define DRIVE_FRONT_RIGHT_MOTOR_CHANNEL         (0)//(4)
#define DRIVE_REAR_RIGHT_MOTOR_CHANNEL          (1)//(5)
#define CATAPULT_MOTOR_CHANNEL          		(3)
#define INTAKE_ROLLER_CHANNEL					(2)
#define SHIFTER_B								(6)

	//Sensor Channels
#define INTAKE_SWITCH_CHANNEL					(2)
#define PULSE_SWITCH_CHANNEL					(3)

	//Pneumatics
//#define SHIFTER_SOLENOID_CHANNEL_A				(1)
//#define SHIFTER_SOLENOID_CHANNEL_B				(2)
#define SHIFTER_CHANNEL							(7)
#define INTAKE_ARM_SOLENOID_CHANNEL_A			(4)
#define INTAKE_ARM_SOLENOID_CHANNEL_B			(5)
#define COMPRESSOR_CHANNEL						(0)			//TODO: get real value

	//Choo-Choo Launcher 
#define STATE_OFF								(0)
#define STATE_HOLD								(1)
#define STATE_RESET								(2)
#define STATE_COCKED							(3)
#define STATE_FIRE								(4)
#define LAUNCH_TIME 							(.17)
#define RESET_TIME								(1.0)
#define SLOW_SPEED								(-1.0)
														
	//RPM
#define TIME_COMPARISON							(.25)
#define MINUTE_CONVERSION						(240)
#define TICKS_PER_ROTATION						(250)

	//Misc
#define SYNC_STATE_OFF          		        (0)
#define REDUCTION							    (.5)
#define DEADZONE								(0.3) //(.15) the joystick axis can be go up to 0.17 at its resting state. Update 2016/01/14: It was 0.2 the left joystick was extremely sensitive
#define TICKS_DEADZONE							(10)
#define INCHES_PER_TICK                			(0.075398)   // circumference of wheel / total 
#define PI										(3.14159265358979323846264338327950288)
#define INCHES_PER_VOLT							(1/.0098)// 1 / (9.8mV / 1000)

#endif
