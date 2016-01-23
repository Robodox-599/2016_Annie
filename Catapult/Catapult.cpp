#include "Catapult.h"


static const float TRIGGER_THRESHOLD = 0.25;

Catapult::Catapult()
{
	oi = nullptr; // TJF: added this to stop the compiler from complaining.
	armManip = new ManipArm();
	choochooTalon = new CANTalon(CATAPULT_MOTOR_CHANNEL);
	choochooTalon->ConfigFwdLimitSwitchNormallyOpen(true);
	// TODO: figure out how to implement pulse control thingy with the cantalon
	
	lastPulse = false;
	launchState = STATE_HOLD;
	autoLaunchState = STATE_COCKED;	
	
	init = true;
	autonInit1 = true;
	autonInit2 = true;
	lastPressed = true;
	
	timer = new Timer();
	timer->Start();
	initTime = 0;
	currentTime = 0;

	safety = 0;
	fire = 0;

	killSwitchA = true;
	killSwitchB = true;
}

// TJF: Added this entire constructor (see note in .h file)
Catapult::Catapult(OperatorInterface* the_oi)
{
	oi = the_oi;

	armManip = new ManipArm();
	choochooTalon = new CANTalon(CATAPULT_MOTOR_CHANNEL);
	choochooTalon->ConfigFwdLimitSwitchNormallyOpen(true);
	/*Configure the fwd limit switch to be normally open or normally closed.
	Talon will disable momentarilly if the Talon’s current setting is dissimilar
	to the caller’s requested setting.*/
	// TODO: figure out how to implement pulse control thingy with the cantalon

	lastPulse = false;
	launchState = STATE_HOLD;
	autoLaunchState = STATE_COCKED;

	init = true;
	autonInit1 = true;
	autonInit2 = true;
	lastPressed = true;

	timer = new Timer();
	timer->Start();
	initTime = 0;
	currentTime = 0;

	safety = 0;
	fire = 0;

	killSwitchA = true;
	killSwitchB = true;
}



Catapult::~Catapult()
{
	delete choochooTalon;
	delete timer;
	
	choochooTalon = NULL;
	timer = NULL;
}

void Catapult::launchBall()
{	
	// TJF: According to XBoxControlMapping.jpg the shoulders are axes, not buttons.
	// 		Just used an arbitrary limit to detect when they're pressed
	fire   = (oi->joyDrive->GetRawAxis(FIRE_BUTTON)   > TRIGGER_THRESHOLD); //fire = oi->joyDrive->GetRawButton(FIRE_BUTTON);
	safety = (oi->joyDrive->GetRawAxis(SAFETY_BUTTON) > TRIGGER_THRESHOLD); //safety = oi->joyDrive->GetRawButton(SAFETY_BUTTON);
	// TJF: Removed this logic for testing. Re-add when needed.
	oi->joyDrive->GetRawButton(KILL_SWITCH_A);
	killSwitchA = false;
	oi->joyDrive->GetRawButton(KILL_SWITCH_B);
	killSwitchB = false;

	switch(launchState)	
	{
	case STATE_OFF:	
		choochooTalon->Set(0);
		if(lastPressed && !killSwitchA && !killSwitchB)
		{
			lastPressed = false;
		}
		if(killSwitchA && killSwitchB && !lastPressed)
		{
			lastPressed = true;
			launchState = STATE_HOLD;
		}
		break;
	case STATE_HOLD:
		
		if(killSwitchA && killSwitchB)
		{
			launchState = STATE_OFF;
		}
		
		init = true;
		choochooTalon->Set(0);
		
		if(lastPressed && (fire == 0) && (safety == 0))
		{
			lastPressed = false;
		}

		if((fire == 1) && (safety == 1) && !lastPressed)
		{
			lastPressed = true;
			launchState = STATE_RESET;
		}
		break;
	case STATE_RESET:
		
		if(killSwitchA && killSwitchB) 
		{
			launchState = STATE_OFF;
		}

		if(init)
		{
			// TJF: Added Stop/Start calls to every timer read because I thought that was part of the reason this didn't work.
			//		It's probably not necessary so feel free to remove those later.
			timer->Stop();
			initTime = timer->Get();
			init = false;
			timer->Start();
		}

		timer->Stop();
		currentTime = timer->Get();
		timer->Start();
		
		if(currentTime < RESET_TIME + initTime)
		{
			choochooTalon->Set(-1.0);
		}
		else
		{
			// TJF: I think this logic is trying to stop the motor, right? I might be reading it wrong.
			choochooTalon->Set(0.0); //choochooTalon->Set(SLOW_SPEED);
		}
		
		if(choochooTalon->IsFwdLimitSwitchClosed() == 1) //Returns True if limit switch is closed. False if open
		{
			lastPulse = true;
		}
		if(choochooTalon->IsFwdLimitSwitchClosed() == 0 && lastPulse)
		{
			launchState = STATE_COCKED;
		}
		break;
	case STATE_COCKED:
		
		if(killSwitchA && killSwitchB) 
		{
			launchState = STATE_OFF;
		}
		init = true;
		choochooTalon->Set(0);
		
		if(lastPressed && (fire == 0) && (safety == 0))
		{
			lastPressed = false;
		}
		if((fire == 1) && (safety == 1) && !lastPressed && armManip->getArmPosition() == true)//true?
		{
			lastPressed = true;
			launchState = STATE_FIRE;
		}
		break;
	case STATE_FIRE:
		if(killSwitchA && killSwitchB) 
		{
			launchState = STATE_OFF;
		}	
		
		if(init)
		{
			timer->Stop();
			initTime = timer->Get();
			timer->Start();
			init = false;
		}

		timer->Stop();
		currentTime = timer->Get();
		timer->Start();
		
		if(currentTime < LAUNCH_TIME + initTime)
		{
			choochooTalon->Set(-1.0); //maybe change it to positive?
		}
		else
		{
			launchState = STATE_HOLD;
		}
		break;
	default:
		launchState = STATE_OFF;
	}
}

void Catapult::autoFirstLaunch()
{	
	if(autonInit1)
	{
		initTime = timer->Get();
		autonInit1 = false;
	}
	currentTime = timer->Get();
	
	if(currentTime < LAUNCH_TIME + initTime)
	{
		choochooTalon->Set(-1);
	}
	else
	{
		choochooTalon->Set(0);
	}
}

void Catapult::autoSecondLaunch()
{	
	if(autonInit2)
	{
		initTime = timer->Get();
		autonInit2 = false;
	}
	currentTime = timer->Get();
	
	if(currentTime < LAUNCH_TIME + initTime)
	{
		choochooTalon->Set(-1); 
	}
	else
	{
		choochooTalon->Set(0);
	}
}

void Catapult::autoReset()
{
	if(init)
	{
		initTime = timer->Get();
		init = false;
	}
	currentTime = timer->Get();
	
	if(currentTime < RESET_TIME + initTime)
	{
		choochooTalon->Set(-1); 
	}
	
	if(choochooTalon->IsFwdLimitSwitchClosed() == 1)
	{
		lastPulse = true;
	}
	if(choochooTalon->IsFwdLimitSwitchClosed() == 0 && lastPulse)
	{
		choochooTalon->Set(0);
	}
}

// TJF: Test only function, not necessary anymore but left for potential use.
/*void Catapult::Test_Motor(float velocity)
{
	choochooTalon->Set(velocity);
}*/
