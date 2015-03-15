
//=============================================================================
// gameApp.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates how to use directx input 
// b. demonstrates how to use directx fonts
// c. demonstrates how to initialize direct3D
// d. provide a framework for creating games
// e. demonstrates how to create an egnine frame and provide a skeleton function of one.
// f. demonstarte the usage of a camera
// g. demonstrates the creation of a surface and of vertex and index buffers
// h. demonstartes how to set the differnt matrices. 
//
// Description:
//--------------
// A simple application that demonstrates how to use directx to surface and how to use the camea.  
// It uses a framework for a general purpose simple game engine
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


#include "gameApp.h"
#include "House.h"
#include <time.h>
#include <winuser.h>

/********************************************************************************/


// DEFINE
#define row 50
#define colu 50
#define length 60	
#define width 60
//#define numberofhouse  5
#define FRAMES_PER_SECOND 30

#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0

// event types for processing game state
#define GAME_UPDATE_FRAME_STATE_EVENT 1
#define GAME_COLLISION_EVENT 2
#define CameraChange 3
#define GAME_WINNING_EVENT 4
#define GAME_LOSING_EVENT 5
#define BOUNDARY 6
/*******************************************************************************/
// class static variables
camera *gameApp::cam = NULL;
camera *gameApp::cam1 = NULL;
camera *gameApp::cam3 = NULL;
 camera *gameApp::overCam=NULL;
meshSurface *gameApp::drawSurface = NULL;
Chaser *gameApp::chaserYellowKia = NULL;
Prey   *gameApp::preyRedKia = NULL;
Chaser    *gameApp::chaserYellowKia1 = NULL;
gameApp * gameApp::myApp;
House   *gameApp::house1 = NULL;
House   *gameApp::house2 = NULL;
House   *gameApp::house3 = NULL;
House   *gameApp::house4 = NULL;
House   *gameApp::house5 = NULL;
House   *gameApp::house6 = NULL;
Client	*gameApp::client = NULL;
House   *gameApp::targethouse = NULL;
//Client *gameApp::cl=NULL;
std::vector<gameObject *> gameApp::gameStaticEntities;	// these entities do not change with time
std::vector<gameObject *> gameApp::gameDynamicEntities; // these entities' attributes change with time

char *winning = "YouWin";
char* message ;

/******************************************************************/
/*
Purpose: constructor of the game application


Descripton: 

Return:


*/

gameApp::gameApp(void) 
{

	myApp = this;
}





/******************************************************************/
/*
Purpose: destructor of the game application


Descripton: 

Return:
1 - if failed
0 - if successful

*/

gameApp::~gameApp(void)
{
}





/******************************************************************/
/*
Purpose: initializes the graphics device

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::initGraphics(int argc, char** argv, int winWidth, int winHeight, int winPosx, int winPosy)
{
	int rc = 0;

	GLenum err;

	//  initialize GLUT and pass it the command variables
	glutInit(&argc, argv);
	
	//glutInitDisplayMode(GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Set the window
	// instruct openGL where to put the window on the screen
	glutInitWindowPosition(winPosx, winPosy);

	// instuct openGL what window size ot use
	glutInitWindowSize(winWidth, winHeight);		// no magic numbers

	// careate the fist window and obtain a handle to it 
	wId = glutCreateWindow("My First Window");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set the colour buffer to black   
	glEnable(GL_DEPTH_TEST);				// Enable depth ubffer testing 
	glClearDepth(1.0f);						// Set depth buffer 
	glDepthFunc(GL_LEQUAL);					// depth test operation (this is the default)
	glShadeModel(GL_SMOOTH);				//  smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // highest quality perspective corrections

	// set the call back functions
	glutDisplayFunc(renderFrame);
	glutReshapeFunc(reshapeFun);
	glutKeyboardFunc(keyboardFun);
	glutSpecialFunc(specialKeyboardFun);
	glutTimerFunc(10, timerFun, 1);



	// initialize GLEW
	err = glewInit();
	//GLenum err = glewInit();
	if (err != GLEW_OK)  fprintf(stderr, " Error initializing GLEW! \n");
	else fprintf(stderr, "Initializing GLEW succeeded!\n");


	return 0;


	return(0);
}


/******************************************************************/
/*
Purpose: executes the message loop

Descripton: 
This function is a virtual function and can be replaced by a similar function in the derived class.



Return:
1 - if failed
0 - if successful

*/
int gameApp::communicate(){

	SOCKET src=client->runrun();
	while (1) {
		client->clientAction(message, src);
		Sleep(300);
	}
	return 0;

}

int gameApp::gameLoop(void)
{
	
	
	// enter glut main loop
	glutMainLoop();
	
	return 0;
}


/******************************************************************/
/*
Purpose: updates the game state

Descripton: 

Return:
1 - if failed
0 - if successful

*/




int gameApp::updateGameState(long event)
{
	//cam1 = new camera();

	static int frameCount = 0;
	static int dir = 1;
	unsigned int i;
	int rc;

	switch (event) {
	case GAME_UPDATE_FRAME_STATE_EVENT:
		
		frameCount++;	// increament frame count

		// this section should have the game logic with respect to time.
		// Then it calls each object with updateStateFunction

		for (i = 0; i < gameDynamicEntities.size(); i++) {
			
			rc = gameDynamicEntities.at(i)->updateState(frameCount);
			cam1->setCamera(Vector3f(preyRedKia->mPosition.x+preyRedKia->lookAtVector.x, preyRedKia->mPosition.y+1, preyRedKia->mPosition.z+preyRedKia->lookAtVector.z), preyRedKia->mPosition + preyRedKia->lookAtVector * 4, Vector3f(0, 1, 0));
			cam3->setCamera(Vector3f(preyRedKia->mPosition.x + preyRedKia->lookAtVector.x*(-2), preyRedKia->mPosition.y + 2, preyRedKia->mPosition.z + preyRedKia->lookAtVector.z*(-2)), preyRedKia->mPosition + preyRedKia->lookAtVector * 4, Vector3f(0, 1, 0));
			message = client->PackData(preyRedKia->mPosition, preyRedKia->lookAtVector, 0.2, preyRedKia->getSpeed());

			
			if (rc == GAME_OBJECT_DELETE_ENTITY) {
				// swap current object with the last one
				gameObject *temp = gameDynamicEntities.at(i);
				gameDynamicEntities.at(i) = gameDynamicEntities.back();			
				delete temp;
				gameDynamicEntities.pop_back();
			}
		}
		break;
	case GAME_COLLISION_EVENT:
		if (collisiondetection(preyRedKia->mPosition, house1->mPosition)){
			preyRedKia->moveForward((float)-.3);
		}
		if (collisiondetection(preyRedKia->mPosition, house2->mPosition)){
			preyRedKia->moveForward((float)-.3);
		}
		if (collisiondetection(preyRedKia->mPosition, house3->mPosition)){
			preyRedKia->moveForward((float)-.3);
		}
		if (collisiondetection(preyRedKia->mPosition, house4->mPosition)){
			preyRedKia->moveForward((float)-.3);
		}	
		if (collisiondetection(preyRedKia->mPosition, house5->mPosition)){
			preyRedKia->moveForward((float)-.3);
		}	
		if (collisiondetection(preyRedKia->mPosition, house6->mPosition)){
			preyRedKia->moveForward((float)-.3);
		}
		break;
	case GAME_WINNING_EVENT:
		if (collisiondetection(preyRedKia->mPosition, targethouse->mPosition)){
		//output(0, 0, 0, (float)1, (float)0, (float)0, "YOUWIN");
			MessageBox(NULL, "YouWin", "Great", MB_OK | MB_ICONEXCLAMATION);
			preyRedKia->setPositionOrientation(Vector3f(5, 0, 5), Vector3f(1, 0, 1), Vector3f(0, 1, 0));
			chaserYellowKia->setPositionOrientation(Vector3f(-5, 0, 5), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
		}
		
		break;
	case GAME_LOSING_EVENT:
		if (collisiondetection(preyRedKia->mPosition, chaserYellowKia->mPosition)){
			MessageBox(NULL, "YouLose", "Sorry", MB_OK | MB_ICONEXCLAMATION);
			preyRedKia->setPositionOrientation(Vector3f(5, 0, 5), Vector3f(1, 0, 1), Vector3f(0, 1, 0));
			chaserYellowKia->setPositionOrientation(Vector3f(-15, 0, -5), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
			chaserYellowKia1->setPositionOrientation(Vector3f(-20, 0, 10), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
			preyRedKia->moveForward(-2.0);
		}
		else if
		(collisiondetection(preyRedKia->mPosition, chaserYellowKia1->mPosition)){
			MessageBox(NULL, "Sorry", "YouLose", MB_OK | MB_ICONEXCLAMATION);
			preyRedKia->setPositionOrientation(Vector3f(5, 0, 5), Vector3f(1, 0, 1), Vector3f(0, 1, 0));
			chaserYellowKia->setPositionOrientation(Vector3f(-15, 0, -5), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
			chaserYellowKia1->setPositionOrientation(Vector3f(-20, 0, 10), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
			preyRedKia->moveForward(-2.0);
		}

		break;
	case BOUNDARY:
		if (boundary(preyRedKia->mPosition)){
			preyRedKia->moveForward(-5.0);
		}
		
		if (boundary(chaserYellowKia->mPosition)){
			chaserYellowKia->moveForward(-10.0);
		}
		if (boundary(chaserYellowKia1->mPosition)){
			chaserYellowKia1->moveForward(-10.0);
		}
		break;
	default:
		printf("another event occured \n");
		break;
	}



	
	return 0;
}


/******************************************************************/
/*
Purpose: renders one frame

Descripton: 

Return:
1 - if failed
0 - if successful

*/



void gameApp::renderFrame(void)
{
	static int count = 0;
	static int dir = 1;
	unsigned int i;

	count++;
	glClearColor(0, 0, 0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the colour and depth buffers

	for (i = 0; i < gameDynamicEntities.size(); i++) {
		gameObject *g = gameDynamicEntities.at(i);
		gameDynamicEntities.at(i)->render(NULL, gameApp::cam);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		gameStaticEntities.at(i)->render(NULL, gameApp::cam);
	}


	// swap the buffers
	glutSwapBuffers();
}


/******************************************************************/
/*
Purpose: initialize the game

Descripton: 

Return:
1 - if failed
0 - if successful

*/

int gameApp::initGame(void)
{
		// create game objects
	//Car *truck;
			// may want to set them as static for collision
	/*int h1;
	srand(time(NULL));
	h1 = rand() % numberofhouse+1;
	int h2;
	h2=numberofhouse-h1;*/

	
	house1 = new House();
	house2 = new House();
	house3 = new House();
	house4 = new House();
	house5 = new House();
	house6 = new House();
	
	targethouse = new House();
	//truck = new Car();
	preyRedKia = new Prey();
	chaserYellowKia = new Chaser(preyRedKia);
	chaserYellowKia1 = new Chaser(preyRedKia);
	drawSurface = new meshSurface();

	drawSurface->createShaderProg("general.vert", "general.frag");
	//truck->createShaderProg("general.vert", "general.frag"); // load shader
	preyRedKia->createShaderProg("general.vert", "general.frag");
	chaserYellowKia->createShaderProg("general.vert", "general.frag");
	chaserYellowKia1->createShaderProg("general.vert", "general.frag");
	house1->createShaderProg("general.vert", "general.frag");
	house2->createShaderProg("general.vert", "general.frag");
	targethouse->createShaderProg("generalRed.vert", "generalRed.frag");
	house3->createShaderProg("general.vert", "general.frag");
	house4->createShaderProg("general.vert", "general.frag");
	house5->createShaderProg("general.vert", "general.frag");
	house6->createShaderProg("general.vert", "general.frag");
	// set truck object 
	// load the gemoetry
	//truck->loadModelOBJ("truck\\L200_OBJ_DOS.obj",&truck->mVtxBuf, &truck->mNumVtx,&truck->mIndBuf, &truck->mNumInd);
	//truck->loadTexture("truck\\truck_color_clean_256.jpg");	// get the texture

	//// set the initial position attributes - align the object wth the z-axis if needed
	//truck->setScale((float) 0.05, (float) 0.05, (float) 0.05);
	//truck->setPositionOrientation(Vector3f(0,0,0), Vector3f(0,0,1),Vector3f(0,1,0));

	// set the prey object
	// load the gemoetry
	preyRedKia->loadModelOBJ("kia\\kia_rio.obj",&preyRedKia->mVtxBuf, &preyRedKia->mNumVtx,&preyRedKia->mIndBuf, &preyRedKia->mNumInd);
	// load the textures
	preyRedKia->loadTexture("kia\\rio_red.bmp");
	// set the initial position attributes - align the object wth the z-axis if needed
	preyRedKia->setScale((float) 0.2, (float) 0.2, (float) 0.2);
	preyRedKia->setPositionOrientation(Vector3f(5.04,0.2,5.0), Vector3f(1,0,1),Vector3f(0,1,0));
	preyRedKia->mYaw=90;		// orient it so that it faces that +zaxis

	// set the chaser object
	// load the gemoetry model
	chaserYellowKia->loadModelOBJ("kia\\kia_rio.obj",&chaserYellowKia->mVtxBuf, &chaserYellowKia->mNumVtx,&chaserYellowKia->mIndBuf, &chaserYellowKia->mNumInd);
	// load the textures
	chaserYellowKia->loadTexture("kia\\rio_yellow.bmp");
	// set the initial position attributes - align the object wth the z-axis if needed
	chaserYellowKia->setScale((float) 0.2, (float) 0.2, (float) 0.2);
	chaserYellowKia->mYaw=90;		// orient it so that it faces that +zaxis
	chaserYellowKia->setPositionOrientation(Vector3f(-15,0,-5), Vector3f(0,0,1),Vector3f(0,1,0));






	chaserYellowKia1->loadModelOBJ("kia\\kia_rio.obj", &chaserYellowKia1->mVtxBuf, &chaserYellowKia1->mNumVtx, &chaserYellowKia1->mIndBuf, &chaserYellowKia1->mNumInd);
	// load the textures
	chaserYellowKia1->loadTexture("kia\\rio_yellow.bmp");
	// set the initial position attributes - align the object wth the z-axis if needed
	chaserYellowKia1->setScale((float) 0.2, (float) 0.2, (float) 0.2);
	chaserYellowKia1->mYaw = 90;		// orient it so that it faces that +zaxis
	chaserYellowKia1->setPositionOrientation(Vector3f(-20, 0, 10), Vector3f(0, 0, 1), Vector3f(0, 1, 0));

	




	targethouse->loadModelOBJ("targethouse\\target.obj", &targethouse->mVtxBuf, &targethouse->mNumVtx, &targethouse->mIndBuf, &targethouse->mNumInd);	
	// load the textures
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	targethouse->setScale((float) 0.015, (float)  0.015, (float) 0.02);
	targethouse->mYaw = 180;
	targethouse->setPositionOrientation(Vector3f(20, 0, -20), Vector3f(1, 0, 1), Vector3f(0, 1, 0));


	
		// set the house object
		house1->loadModelOBJ("house_obj\\house_obj.obj", &house1->mVtxBuf, &house1->mNumVtx, &house1->mIndBuf, &house1->mNumInd);
		// load the textures
		house1->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		house1->setScale(0.006, 0.006, 0.006);
		house1->setPositionOrientation(Vector3f(10, 0, 6.0), Vector3f(1, 0, 1), Vector3f(0, 1, 0));



		house3->loadModelOBJ("house_obj\\house_obj.obj", &house3->mVtxBuf, &house3->mNumVtx, &house3->mIndBuf, &house3->mNumInd);
		// load the textures
		house3->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		house3->setScale(0.006, 0.006, 0.006);
		house3->setPositionOrientation(Vector3f(20, 0, 15), Vector3f(1, 0, 1), Vector3f(0, 1, 0));




		house2->loadModelOBJ("house_obj\\house_obj.obj", &house2->mVtxBuf, &house2->mNumVtx, &house2->mIndBuf, &house2->mNumInd);
		// load the textures
		house2->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		house2->setScale(0.006, 0.006, 0.006);
		house2->setPositionOrientation(Vector3f(-12, 0, -1), Vector3f(-1, 0, 1), Vector3f(0, 1, 0));

	
		house4->loadModelOBJ("house_obj\\house_obj.obj", &house4->mVtxBuf, &house4->mNumVtx, &house4->mIndBuf, &house4->mNumInd);
		// load the textures
		house4->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		house4->setScale(0.006, 0.006, 0.006);
		house4->setPositionOrientation(Vector3f(-2, 0, -9.6), Vector3f(-1, 0, 1), Vector3f(0, 1, 0));

		house5->loadModelOBJ("house_obj\\house_obj.obj", &house5->mVtxBuf, &house5->mNumVtx, &house5->mIndBuf, &house5->mNumInd);
		// load the textures
		house5->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		house5->setScale(0.006, 0.006, 0.006);
		house5->setPositionOrientation(Vector3f(-20, 0, -12.6), Vector3f(-1, 0, 1), Vector3f(0, 1, 0));


		house6->loadModelOBJ("house_obj\\house_obj.obj", &house6->mVtxBuf, &house6->mNumVtx, &house6->mIndBuf, &house6->mNumInd);
		// load the textures
		house6->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		house6->setScale(0.006, 0.006, 0.006);
		house6->setPositionOrientation(Vector3f(-15, 0,10.6), Vector3f(-1, 0, 1), Vector3f(0, 1, 0));

	// set the surface
	drawSurface->createSurface(row, colu, width, length);
	drawSurface->loadTexture("surface\\grass_texture_256.tga");

	// add the objects to the list of game objects
	gameDynamicEntities.push_back(chaserYellowKia);
	gameDynamicEntities.push_back(chaserYellowKia1);
	gameDynamicEntities.push_back(preyRedKia);
	//gameDynamicEntities.push_back(truck);

	// add the objecst to the list of game objects
	gameStaticEntities.push_back(drawSurface);
	
	gameStaticEntities.push_back(house1);
	gameStaticEntities.push_back(house2);
	gameStaticEntities.push_back(house3);
	gameStaticEntities.push_back(house4);
	gameStaticEntities.push_back(targethouse);
	gameStaticEntities.push_back(house5);
	gameStaticEntities.push_back(house6);
	// set the global camera
	cam = new camera();
	cam1 = new camera();
	cam3 = new camera();
	overCam = new camera();
	cam->setCamera(Vector3f(0,20,40), Vector3f(0,0,0), Vector3f(0,1,0));
	cam->setPerspectiveView(60, 1, (float) 0.2, 1000);
	//
	overCam = cam;
	cam1->setPerspectiveView(60, 1, (float) 0.2, 1000);
	//scam3->setCamera(Vector3f(0, 20, 40), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
	cam3->setPerspectiveView(60, 1, (float) 0.2, 1000);


	
	
	
	return 0;
}



/****************************************************************************/



void gameApp::keyboardFun(unsigned char key, int x, int y) 
{
	switch (key) {
		case 033:
		case 'Q':
			cam->setCamera(Vector3f(0, 20, 40), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
			break;
		case 'c':
			cam = cam1;
			break;
		case 'v':
			cam = cam3;
			break;
		case 'q':
			exit(1);
			break;
		case 'w': 
			cam->moveForward(NORMAL_SPEED); 
			break;
		case 'W': 
			cam->moveForward(MAX_SPEED); 
			break;
		case 's':
			cam->moveForward(-NORMAL_SPEED); 
			break; 
		case 'S':
			cam->moveForward(-MAX_SPEED); 
			break;
		case 'a': 
			cam->yaw((float) .2);
			break;
		case 'A': 
			cam->yaw(1);
			break;
		case 'D':
			cam->yaw(-1);
			break;
		case 'd': 
			cam->yaw((float) -.2);
			break;
		case 'z': 
			cam->zoomIn();
			break;
		case 'Z': 
			cam->zoomOut();
			break;
		case 'r': 
//			angle += 1.0;
			break;
		case 'R': 
//			angle -= 1.0;
			break;
		case 't': 
//			refractFlag = (refractFlag == 0) ? 1 : 0;
			break;
		case'i':
			preyRedKia->moveForward((float).5);
			break;
		case'k':
			preyRedKia->moveForward((float)-.5);
			break;
		case'j':
			preyRedKia->yaw((float)10);
			break;
		case'l':
			preyRedKia->yaw((float)-10);
			break;
		case'u':
			cam = overCam;//->setCamera(cam->position_old, cam->lookAtVector_old, cam->upVector_old);
			break;
	
	}
}



void gameApp::specialKeyboardFun(int key, int x, int y) 
{
	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		case GLUT_KEY_LEFT:
			cam->roll((float) .2);
			break;
		case GLUT_KEY_UP:
			cam->pitch((float) .2);
			break;
		case GLUT_KEY_RIGHT:
			cam->roll((float) -.2);
			break;
		case GLUT_KEY_DOWN:
			cam->pitch((float) -.2);
			break;
	}
}



/************************************************************************************************/
/*
This functions in invoked when ther
e is a change to the window by the user
 Here a new setting of the rendering paramters are set. In most cases it consists of two things:
a. changeing the view port dimensions
b. changing the aspect ratio

input 
w, h - the width and height of the new window in the pixel coordinates

*/

void gameApp::reshapeFun(int w, int h)
{

	static GLfloat aspect;			// aspect ratio of height and width

	// change the view port
	glViewport (0 , 0, (GLsizei) w, (GLsizei) h);

	// Compute aspect ratio of the new window
	aspect = (float)w / (float)h;


	// Set the state to accept projection information
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
	cam->setPerspectiveView(cam->fieldOfView,aspect,cam->nearPlane,cam->farPlane);

}
 


/***************************************************************************************************/

void gameApp::timerFun(int timerEvent)
{
	static int frameNumber = 0;
	frameNumber++;


//	renderSceneFun();
	glutPostRedisplay();
	// get next event from Queue  (future implementation) 
	// the queue should have return an event and the event timer
	myApp->updateGameState((long) GAME_UPDATE_FRAME_STATE_EVENT);
	myApp->updateGameState((long)GAME_COLLISION_EVENT);
	myApp->updateGameState((long)GAME_WINNING_EVENT);
	myApp->updateGameState((long)GAME_LOSING_EVENT);
	myApp->updateGameState((long)BOUNDARY);
	glutTimerFunc(5, timerFun, frameNumber);
}

bool gameApp::collisiondetection(Vector3f ob1, Vector3f ob2){
	if (pow((ob1.x - ob2.x), 2) + pow((ob1.z - ob2.z), 2) <= 15)
		return true;
	else
		return false;
}

bool gameApp::boundary(Vector3f ob1)
{
	if (abs(ob1.x) >= (float)30 || abs(ob1.z)>=(float)30)
		return true;
	else
		return false;

}

void gameApp::output(int x, int y, int z,float r, float g, float b, char *string){
	
	
	glColor3f(r, g, b);
	glRasterPos3f(x,y,z);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);
	}


}


