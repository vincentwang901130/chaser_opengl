#include "Prey.h"




/**************************************************************************/


Prey::Prey(void)
{
}




/**************************************************************************/


Prey::~Prey(void)
{
}




/**************************************************************************/

// this is the object behaviour.  Here the logic should be  

int Prey::updateState(int time) 	// update the object state

{
	//static int dir = 1;

 //// temporarily add motion to objects.  Should be moved to the "behaviour" of objects
	//moveForward((float) 0.06*dir);
	//if (time  % 200 == 0) {
	//	dir *= -1;		// change direction
	//	yaw(-10);  // rotate around the up vector of object by 10 degrees
	//}

	
	return 0;
}




/**************************************************************************/
// responds to a key stroke
int Prey::processKeys(unsigned char key)
{

	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Prey::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}
