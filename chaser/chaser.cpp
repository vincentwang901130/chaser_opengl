

//=============================================================================
// Chaser.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates the creation of a surface and of vertex and index buffers
// b. demonstartes how to rpesent a surface 
//
//
// Description:
//--------------
//
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================



#include "Chaser.h"
#include "Prey.h"



/**************************************************************************/


Chaser::Chaser(void)
{
}
Chaser::Chaser(Prey* p){
	this->p = p;
}




/**************************************************************************/


Chaser::~Chaser(void)
{
}




/**************************************************************************/

// this is the object behaviour.  Here the logic should be  

int Chaser::updateState(int time) 	// update the object state

{
	if (saw(p)){
		chase(p);
	}
	else{
		moveAround(time);

	}
	
	return 0;
}

void Chaser::chase(Prey* p){
	Vector3f v1 = Vector3f(p->mPosition.x - this->mPosition.x, p->mPosition.y - this->mPosition.y, p->mPosition.z - this->mPosition.z);// p->mPosition - this->mPosition;
	Vector3f v2= this->lookAtVector + this->mPosition;
	float theta = Vector3f::cross_value_value(v1, v2);
	if (theta < 0)
		yaw(0.3);//left
	else
		yaw(-0.3);//right

	moveForward(0.07);
}

bool Chaser::saw(Prey* p){
	if (Vector3f(abs(p->mPosition.x) - abs(this->mPosition.x), abs(p->mPosition.y) - abs(this->mPosition.y),abs(p->mPosition.z) - abs(this->mPosition.z)).length() < 450){
		
		return true;
	}
	else
		return false;
}

void Chaser::moveAround(int time){
	static int dir = 1;

	// temporarily add motion to objects.  Should be moved to the "behaviour" of objects
	moveForward((float) 0.06*dir);
	if (time % 300 == 0) {
		dir *= -1;		// change direction
	}

	if (time % 1 == 0) {
		yaw(0.1);  // rotate around the up vector of object by 10 degrees
	}

}
/*************************************************************************************/

/**************************************************************************/
// responds to a key stroke
int Chaser::processKeys(unsigned char key)
{

	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Chaser::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}

