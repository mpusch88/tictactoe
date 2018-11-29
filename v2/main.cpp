///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// testing OpenGL Display List
// glGenLists(), glDeleteLists(), glNewList(), glEndList(), glCallList()
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-10-04
//
// UPDATED: 2016-08-25  (Y. Hu: Microsoft Visual Studio 2015 + Platform toolset in Visual Studio 2015; OpenHatics-V3.2.2; System configuration - Windows 7 + 16:9 screen ratio)
// UPDATED: 2018-08-02  (Y. Hu: Microsoft Visual Studio 2017 + Platform toolset in Visual Studio 2017; OpenHatics-V3.2.2; System configuration - Windows 7 + 16:9 screen ratio)
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Timer.h"
#include <GL/glut.h>
#include "Game.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;


typedef struct {
	double x, y, z;
} XYZ;

typedef struct {
	XYZ vp;					/* View position           */
	XYZ vd;					/* View direction vector   */
	XYZ vu;					/* View up direction       */
	double focalLength;		/* Focal Length along vd   */
	double aperture;		/* Camera aperture         */
	double eyeSep;			/* Eye separation - interpupilar distance    */
} CAMERA;

CAMERA camera;

#define PI      3.141592653589793238462643
#define DTOR    0.0174532925					// DEGREE TO RADIAN
#define RTOD    57.2957795						// RADIAN TO DEGREE

#define HAPTIC									// Enabling haptic device

#ifdef HAPTIC

	#include <HL/hl.h>
	#include <HDU/hduMatrix.h>
	#include <HDU/hduError.h>
	#include <HLU/hlu.h>


	/* Haptic device and rendering context handles. */
	static HHD ghHD = HD_INVALID_HANDLE;
	static HHLRC ghHLRC = 0;

	/* Shape id for shape we will render haptically. */
	HLuint gTeapotShapeId;
	HLuint gSquareId[9];
	#define CURSOR_SCALE_SIZE 60
	static double gCursorScale;
	static GLuint gCursorDisplayList = 0;
	#else
	#include <math.h>

#endif

// GLUT CALLBACK functions

void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);
void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void showInfo();
void showFPS();
void drawScene();
void drawMono();
void drawObject();
void drawEye(int eye);
void drawInStereo();
void drawEyeLookAt();
void drawSphere(float x, float y, float z, int id);
void drawX(float x, float y, float z, int id);
void normalise(XYZ *p);
XYZ crossProduct(XYZ p1, XYZ p2);
void color(int id);
void drawSquare(float x, float y, float z, int id);

#ifdef HAPTIC

	void exitHandler(void);
	void initHL();
	void drawSceneHaptics();
	void drawHapticCursor();
	void updateWorkspace();

#endif

// Global Variables

float diffuseColorRed[3] = { 0.929524f, 0.1f, 0.178823f };
float diffuseColorGreen[3] = { 0.3f, 0.53f, 0.54f };
float diffuseColors[9][3];
Timer timer;
Game game;
bool gameOver = false;
char moveList[9];
bool set[9];
bool square[9];
void *font = GLUT_BITMAP_8_BY_13;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX = 0;
float cameraAngleY = 0;
float cameraDistance;
int drawMode = 0;
bool stereoMethod;
float ratio = 1.777;
float foview = 60.;

// one camera - 2 frustums --> stereoscopy with each eye equal to one frustrum
float nNear = 1.;
float nFar = 15.;
float frustumTop = nNear*tan(foview*3.14159 / 360);
float frustumRight = frustumTop*ratio; //top

// frustum parameters for 2 cameras --> stereoscopy with each eye equal to one camera
float leftt;
float rightt;
float top;
float bottom;
float nearP = 1;
float farP = 40;

int colorIndex = 3;				// RGB
bool touched;
int stereo = 1;

// Haptic callback
#ifdef HAPTIC

	void HLCALLBACK touchShapeCallback(HLenum event, HLuint object, HLenum thread, HLcache *cache, void *userdata)
	{
		if (!set[object - 2] && !gameOver)
		{
			touched = !touched;
			game.updateBoard(object - 2);
			game.updatePlayer();

			if (square[object - 2])
			{
				square[object - 2] = false;

			}
			color(object);
		}

		cout << "object is" << object << endl;
	}

#endif

void color(int id) 
{
	if (!set[id - 2])
	{
		if (!game.getBoard().xWins() && !game.getBoard().oWins())
		{
			if (!touched)
			{
				for (int i = 0; i < colorIndex; i++)
				{
					diffuseColors[id - 2][i] = diffuseColorGreen[i];
				}
			}

			else
			{
				for (int i = 0; i < colorIndex; i++)
				{
					diffuseColors[id - 2][i] = diffuseColorRed[i];
				}
			}
			if (game.getPlayer()->getMark() == 'X') {
				moveList[id - 2] = 'x';

			}

			else
			{
				moveList[id - 2] = 'o';

			}

			set[id - 2] = true;
		}
	}
}

void drawX(float x, float y, float z, int id) 
{
	glPushMatrix();
	glTranslatef(x, y, z);

	glLineWidth(255);
	glColor3fv(diffuseColors[id - 2]);

	glBegin(GL_POLYGON);
	glVertex3d(0.0, 0.2,1.0);
	glVertex3d(-0.8, 1.0, 1.0);
	glVertex3d(-1.0, 0.8, 1.0);
	glVertex3d(-0.2, 0.0, 1.0);
	glVertex3d(-1.0, -0.8, 1.0);
	glVertex3d(-0.8, -1.0, 1.0);
	glVertex3d(0.0, -0.2, 1.0);
	glVertex3d(0.8, -1.0, 1.0);
	glVertex3d(1.0, -0.8, 1.0);
	glVertex3d(0.2, 0.0, 1.0);
	glVertex3d(1.0, 0.8, 1.0);
	glVertex3d(0.8, 1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.0, 0.2, -1.0);
	glVertex3d(0.8, 1.0, -1.0);
	glVertex3d(1.0, 0.8, -1.0);
	glVertex3d(0.2, 0.0, -1.0);
	glVertex3d(1.0, -0.8, -1.0);
	glVertex3d(0.8, -1.0, -1.0);
	glVertex3d(0.0, -0.2, -1.0);
	glVertex3d(-0.8, -1.0, -1.0);
	glVertex3d(-1.0, -0.8, -1.0);
	glVertex3d(-0.2, 0.0, -1.0);
	glVertex3d(-1.0, 0.8, -1.0);
	glVertex3d(-0.8, 1.0, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.0, 0.2, 1.0);
	glVertex3d(0.0, 0.2, -1.0);
	glVertex3d(-0.8, 1.0, -1.0);
	glVertex3d(-0.8, 1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.0, 0.2, 1.0);
	glVertex3d(0.8, 1.0, 1.0);
	glVertex3d(0.8, 1.0, -1.0);
	glVertex3d(0.0, 0.2, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.2, 0.0, 1.0);
	glVertex3d(0.2, 0.0, -1.0);
	glVertex3d(1.0, 0.8, -1.0);
	glVertex3d(1.0, 0.8, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(-0.2, 0.0, 1.0);
	glVertex3d(-1.0, 0.8, 1.0);
	glVertex3d(-1.0, 0.8, -1.0);
	glVertex3d(-0.2, 0.0, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.0, -0.2, 1.0);
	glVertex3d(-0.8, -1.0, 1.0);
	glVertex3d(-0.8, -1.0, -1.0);
	glVertex3d(0.0, -0.2, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.0, -0.2, 1.0);
	glVertex3d(0.0, -0.2, -1.0);
	glVertex3d(0.8, -1.0, -1.0);
	glVertex3d(0.8, -1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.2, -0.0, 1.0);
	glVertex3d(1.0, -0.8, 1.0);
	glVertex3d(1.0, -0.8, -1.0);
	glVertex3d(0.2, -0.0, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(-0.2, -0.0, 1.0);
	glVertex3d(-0.2, -0.0, -1.0);
	glVertex3d(-1.0, -0.8, -1.0);
	glVertex3d(-1.0, -0.8, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.8, 1.0, 1.0);
	glVertex3d(1.0, 0.8, 1.0);
	glVertex3d(1.0, 0.8, -1.0);
	glVertex3d(0.8, 1.0, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(-0.8, 1.0, 1.0);
	glVertex3d(-0.8, 1.0, -1.0);
	glVertex3d(-1.0, 0.8, -1.0);
	glVertex3d(-1.0, 0.8, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(-0.8, -1.0, 1.0);
	glVertex3d(-1.0, -0.8, 1.0);
	glVertex3d(-1.0, -0.8, -1.0);
	glVertex3d(-0.8, -1.0, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3d(0.8, -1.0, 1.0);
	glVertex3d(0.8, -1.0, -1.0);
	glVertex3d(1.0, -0.8, -1.0);
	glVertex3d(1.0, -0.8, 1.0);
	glEnd();

	glPopMatrix();
}

void drawSphere(float x, float y, float z, int id) 
{
	glPushMatrix();
	glTranslatef(x, y, z);

	glColor3fv(diffuseColors[id - 2]);
	glutSolidSphere(1, 20, 16);
	glPopMatrix();
	glFlush();
}

void drawSquare(float x, float y, float z, int id) 
{
	glPushMatrix();

	glTranslatef(x, y, z);
	glColor3fv(diffuseColors[id - 2]);

	glBegin(GL_QUADS);

	glNormal3d(0, 1, 0);						// Top
	glVertex3d(-2.0 / 2, 2.0 / 2, 1.0);
	glVertex3d(2.0 / 2, 2.0 / 2, 1.0);
	glVertex3d(2.0 / 2, 2.0 / 2, -1.0);
	glVertex3d(-2.0 / 2, 2.0 / 2, -1.0);

	glNormal3d(0, 0, 1);						// Back
	glVertex3d(-2.0 / 2, 2.0 / 2, 1.0);
	glVertex3d(-2.0 / 2, -2.0 / 2, 1.0);
	glVertex3d(2.0 / 2, -2.0 / 2, 1.0);
	glVertex3d(2.0 / 2, 2.0 / 2, 1.0);

	glNormal3d(0, 0, -1);						// Front
	glVertex3d(-2.0 / 2, 2.0 / 2, -1);
	glVertex3d(2.0 / 2, 2.0 / 2, -1);
	glVertex3d(2.0 / 2, -2.0 / 2, -1);
	glVertex3d(-2.0 / 2, -2.0 / 2, -1);

	glNormal3d(1, 0, 0);						// Left
	glVertex3d(2.0 / 2, 2.0 / 2, -1);
	glVertex3d(2.0 / 2, 2.0 / 2, 1.0);
	glVertex3d(2.0 / 2, -2.0 / 2, 1.0);
	glVertex3d(2.0 / 2, -2.0 / 2, -1);

	glNormal3d(-1, 0, 0);						// Right
	glVertex3d(-2.0 / 2, 2.0 / 2, 1.0);
	glVertex3d(-2.0 / 2, 2.0 / 2, -1);
	glVertex3d(-2.0 / 2, -2.0 / 2, -1);
	glVertex3d(-2.0 / 2, -2.0 / 2, 1.0);

	glNormal3d(0, -1, 0);						// Bottom
	glVertex3d(-2.0 / 2, -2.0 / 2, 1.0);
	glVertex3d(-2.0 / 2, -2.0 / 2, -1);
	glVertex3d(2.0 / 2, -2.0 / 2, -1);
	glVertex3d(2.0 / 2, -2.0 / 2, 1.0);

	glEnd();
	glPopMatrix();
}

//************************************************************************************

int main(int argc, char **argv)
{
	game = Game();
	mouseLeftDown = mouseRightDown = false;
	stereoMethod = false;

	for (int i = 0; i < 9; i++)
	{
		set[i] = false;
		square[i] = true;

		moveList[i] = 'c';

		for (int j = 0; j < colorIndex; j++)
		{
			diffuseColors[i][j] = 1.0;
		}
	}

	// Parameters for 3d stereoscopy
	camera.aperture = foview;						// Field of view
	camera.focalLength = 14;						// Focal length
	camera.eyeSep = camera.focalLength / 60;		// Separation of the eyes

	// Position of the camera
	camera.vp.x = 0;
	camera.vp.y = 0;
	camera.vp.z = 12;

	// View direction vector
	camera.vd.x = 0;
	camera.vd.y = 0;
	camera.vd.z = -1;

	// View up vector
	camera.vu.x = 0;
	camera.vu.y = 1;
	camera.vu.z = 0;

	initGLUT(argc, argv);
	initGL();

#ifdef HAPTIC

	initHL();										// Initialize haptic device

#endif

	glutMainLoop();						/* Start GLUT event-processing loop */
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
	glutInit(&argc, argv);

	if (stereo == 1)
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_STEREO);		// display stereo mode

	else
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);					// display mono mode

	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);

	int handle = glutCreateWindow(argv[0]);				// param is the title of window

	glutFullScreen();
	glutDisplayFunc(displayCB);
	glutIdleFunc(idleCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);

	return handle;
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
	glShadeModel(GL_SMOOTH);						// shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);			// 4-byte pixel alignment

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);                   // Background colour
	glClearStencil(0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	initLights();
	setCamera(0, 0, 10, 0, 0, 0);
}



///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);		// Lighting and color mask
	glDisable(GL_LIGHTING);								// Disable for proper text color

	glColor4fv(color);						// set text color
	glRasterPos2i(x, y);					// place text position

	while (*str)
	{
		glutBitmapCharacter(font, *str);
		++str;
	}

	glEnable(GL_LIGHTING);
	glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char *str, float pos[3], float color[4], void *font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);		// lighting and color mask
	glDisable(GL_LIGHTING);								// Disable for proper text color

	glColor4fv(color);						// set text color
	glRasterPos3fv(pos);					// place text position

	while (*str)
	{
		glutBitmapCharacter(font, *str);
		++str;
	}

	glEnable(GL_LIGHTING);
	glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
	mouseLeftDown = mouseRightDown = false;
	stereoMethod = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
	// Setting up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { .2f, .2f, .2f, 1.0f };		// ambient light
	GLfloat lightKd[] = { .7f, .7f, .7f, 1.0f };		// diffuse light
	GLfloat lightKs[] = { 1, 1, 1, 1 };					// specular light

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// Positioning light
	float lightPos[4] = { 0, 0, 20, 1 };				// positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);								// MUST enable each light source after configuration
}

///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0);	// eye(x,y,z), focal(x,y,z), up(x,y,z)
}

///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
	// Backing up current model-view matrix
	glPushMatrix();                     // save current modelview matrix
	glLoadIdentity();                   // reset modelview matrix

										// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);		// switch to projection matrix
	glPushMatrix();						// save current projection matrix
	glLoadIdentity();					// reset projection matrix
	gluOrtho2D(0, 400, 0, 300);			// set to orthogonal projection

	float color[4] = { 1, 1, 1, 1 };

	stringstream ss;
	ss << std::fixed << std::setprecision(3);

	if (game.getStatus())
	{
		gameOver = true;
		if (game.getWinner() != NULL) 
		{
			if (game.getWinner()->getMark() == 'O') 
			{
				ss << "Green Wins!!!!" << ends;
				drawString(ss.str().c_str(), 1, 166, color, font);
				ss.str("");

				ss << "Press 'r' to restart!" << ends;
				drawString(ss.str().c_str(), 1, 155, color, font);
				ss.str("");
			}

			else 
			{
				ss << "Red Wins!!!!" << ends;
				drawString(ss.str().c_str(), 1, 166, color, font);
				ss.str("");

				ss << "Press 'r' to restart!" << ends;
				drawString(ss.str().c_str(), 1, 155, color, font);
				ss.str("");
			}
		}

		else 
		{
			ss << " DRAW!!!!" << ends;
			drawString(ss.str().c_str(), 1, 166, color, font);
			ss.str("");

			ss << "Press 'r' to restart!" << ends;
			drawString(ss.str().c_str(), 1, 155, color, font);
			ss.str("");
		}
	}

	else
	{
		if (game.getPlayer()->getMark() == 'O')
		{
			ss << "Current player: Green" << ends;
			drawString(ss.str().c_str(), 1, 286, color, font);
			ss.str("");
		}

		else
		{
			ss << "Current player: Red" << ends;
			drawString(ss.str().c_str(), 1, 286, color, font);
			ss.str("");
		}
	}

	ss << "Press SPACE key to toggle stereo mode." << ends;
	drawString(ss.str().c_str(), 1, 1, color, font);

	// unset floating format
	ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

	glPopMatrix();                   // restore to previous projection matrix
	glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
	glPopMatrix();                   // restore to previous modelview matrix
}

///////////////////////////////////////////////////////////////////////////////
// display frame rates
///////////////////////////////////////////////////////////////////////////////
void showFPS()
{
	static Timer timer;
	static int count = 0;
	static stringstream ss;
	double elapsedTime;

	glPushMatrix();                     // save current modelview matrix
	glLoadIdentity();                   // reset modelview matrix
	glMatrixMode(GL_PROJECTION);        // switch to projection matrix
	glPushMatrix();                     // save current projection matrix
	glLoadIdentity();                   // reset projection matrix
	gluOrtho2D(0, 400, 0, 300);         // set to orthogonal projection

	float color[4] = { 1, 1, 0, 1 };
	elapsedTime = timer.getElapsedTime();

	if (elapsedTime < 1.0)
	{
		++count;
	}

	else
	{
		ss.str("");
		ss << std::fixed << std::setprecision(1);
		ss << (count / elapsedTime) << " FPS" << ends; // update fps string
		ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		count = 0;
		timer.start();
	}

	drawString(ss.str().c_str(), 315, 286, color, font);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//=============================================================================
// CALLBACKS
//=============================================================================
void displayCB()
{
	if (stereo == 1)
		drawInStereo();

	else
		drawMono();

#ifdef HAPTIC

	drawSceneHaptics();

#endif

	glutSwapBuffers();
}


void drawObject() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();

	// Transforming camera
	glTranslatef(0, 0, cameraDistance);		// press down right button of the mouse
	glRotatef(cameraAngleX, 1, 0, 0);		// pitch - press down left button of the mouse
	glRotatef(cameraAngleY, 0, 1, 0);		// heading - press down left button of the mouse

	timer.start();

	int x, y, z;

	for (int i = 0; i < 9; i++)
	{
		int num = i;
		x = 0;
		y = 0;
		z = 0;

		if (square[i])
		{
			if (i == 0) {
				x -= 3;
				y += 3;
			}else if (i == 1) {
				y += 3;
			}else if (i == 2) {
				x += 3;
				y += 3;
			}else if (i == 3) {
				x -= 3;
			}else if (i == 5) {
					x += 3;
			}else if (i == 6) {
				x -= 3;
				y -= 3;
			}else if (i == 7) {
				y -= 3;
			}else if (i == 8) {
				x = 3;
				y = -3;
			}

			drawSquare(x, y, z, i + 2);
		}

		else
		{
			if (i == 0) {
				x -= 3;
				y += 3;
			}
			else if (i == 1) {
				y += 3;
			}
			else if (i == 2) {
				x += 3;
				y += 3;
			}
			else if (i == 3) {
				x -= 3;
			}
			else if (i == 5) {
				x += 3;
			}
			else if (i == 6) {
				x -= 3;
				y -= 3;
			}
			else if (i == 7) {
				y -= 3;
			}
			else if (i == 8) {
				x = 3;
				y = -3;
			}

			if (moveList[i] == 'x') 
			{
				drawX(x, y, z, i + 2);
			}

			else if (moveList[i] == 'o')
			{
				drawSphere(x, y, z, i + 2);
			}
		}
	}

	glPopMatrix();
	timer.stop();
}

void reshapeCB(int w, int h)
{
	// Setting viewport to entire window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	float aspectRatio = (float)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(w) / h, 1.0f, 1000.0f);		// FOV, AspectRatio, NearClip, FarClip
	glMatrixMode(GL_MODELVIEW);

#ifdef HAPTIC

	updateWorkspace();

#endif
}

void timerCB(int millisec)
{
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}

void idleCB()
{
#ifdef HAPTIC

	HLerror error;

	while (HL_ERROR(error = hlGetError()))
	{
		fprintf(stderr, "HL Error: %s\n", error.errorCode);

		if (error.errorCode == HL_DEVICE_ERROR)
		{
			hduPrintError(stderr, &error.errorInfo,
				"Error during haptic rendering\n");
		}
	}

#endif

	glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: // ESCAPE
			exit(0);
			break;

		case 'd': // switch rendering modes (fill -> wire -> point)
		case 'D':
			drawMode = ++drawMode % 3;
			if (drawMode == 0)        // fill mode
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
			}
			else if (drawMode == 1)  // wireframe mode
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
			}

			else                    // point mode
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
			}
			break;

		case ' ': // switch rendering modes (fill -> wire -> point)
			stereoMethod = !stereoMethod;
			break;
		case 'r':
		case 'R':
			game.reset();
			gameOver = false;
			cout << "R: P1 '" << game.getPlayer()->getMark() << "' and opponent '" << game.getPlayer()->getOpponent()->getMark() << "'\n";


			for (int i = 0; i < 9; i++) {
				set[i] = false;
				square[i] = true;
				moveList[i] = 'c';
				for (int j = 0; j < colorIndex; j++) {
					diffuseColors[i][j] = 1.0;
				}
			}
			break;

		default:
			;
	}

	glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}

void mouseMotionCB(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}

	if (mouseRightDown)
	{
		cameraDistance += (y - mouseY) * 0.2f;
		mouseY = y;
	}

	glutPostRedisplay();
}

// haptic code begin

#ifdef HAPTIC

	/*******************************************************************************
	Initialize the HDAPI.  This involves initing a device configuration, enabling
	forces, and scheduling a haptic thread callback for servicing the device.
	*******************************************************************************/
	void initHL()
	{
		HDErrorInfo error;
		ghHD = hdInitDevice(HD_DEFAULT_DEVICE);

		if (HD_DEVICE_ERROR(error = hdGetError()))
		{
			hduPrintError(stderr, &error, "Failed to initialize haptic device");
			fprintf(stderr, "Press any key to exit");
			getchar();
			exit(-1);
		}

		ghHLRC = hlCreateContext(ghHD);
		hlMakeCurrent(ghHLRC);
		hlEnable(HL_HAPTIC_CAMERA_VIEW);

		// Generating IDs for each shape.
		for (int i = 0; i < 9; i++) 
		{
			gSquareId[i] = hlGenShapes(1);
			hlAddEventCallback(HL_EVENT_TOUCH, gSquareId[i], HL_CLIENT_THREAD, &touchShapeCallback, NULL);
			cout << "Initialized cube " << gSquareId[i] << endl;
		}
	}

	/*******************************************************************************
	This handler is called when the application is exiting.  Deallocates any state
	and cleans up.
	*******************************************************************************/
	void exitHandler()
	{
		// Deallocate shape IDs
		for (int i = 0; i < 9; i++)
		{
			hlDeleteShapes(gSquareId[i], 1);
		}

		// Free up the haptic rendering context.
		hlMakeCurrent(NULL);

		if (ghHLRC != NULL)
		{
			hlDeleteContext(ghHLRC);
		}

		// Free up the haptic device.
		if (ghHD != HD_INVALID_HANDLE)
		{
			hdDisableDevice(ghHD);
		}
	}

	/*******************************************************************************
	Use the current OpenGL viewing transforms to initialize a transform for the
	haptic device workspace so that it's properly mapped to world coordinates.
	*******************************************************************************/
	void updateWorkspace()
	{
		GLdouble modelview[16];
		GLdouble projection[16];
		GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		hlMatrixMode(HL_TOUCHWORKSPACE);
		HLdouble minPoint[3], maxPoint[3];

		minPoint[0] = -10;
		minPoint[1] = -10;
		minPoint[2] = -10;
		maxPoint[0] = 10;
		maxPoint[1] = 10;
		maxPoint[2] = 10;

		hluFitWorkspaceBox(modelview, minPoint, maxPoint);

		// Compute cursor scale.
		gCursorScale = hluScreenToModelScale(modelview, projection, viewport);
		gCursorScale *= CURSOR_SCALE_SIZE;
	}

	/*******************************************************************************
	The main routine for rendering scene haptics.
	*******************************************************************************/
	void drawSceneHaptics()
	{
		// Start haptic frame.  (Must do this before rendering any haptic shapes.)
		hlBeginFrame();

		// Set material properties for the shapes to be drawn.
		hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, 0.7f);
		hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, 0.1f);
		hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, 0.2f);
		hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, 0.3f);

		for (int i = 0; i < 9; i++)
		{
			int x = 0;
			int y = 0;
			int z = 0;

			hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gSquareId[i]);

			// Use OpenGL commands to create geometry.
			glPushMatrix();

			// Transform camera
			glTranslatef(0, 0, cameraDistance);
			glRotatef(cameraAngleX, 1, 0, 0);			// Pitch
			glRotatef(cameraAngleY, 0, 1, 0);			// Heading

			if (square[i])
			{
				if (i == 0) {
					x -= 3;
					y += 3;
				}
				else if (i == 1) {
					y += 3;
				}
				else if (i == 2) {
					x += 3;
					y += 3;
				}
				else if (i == 3) {
					x -= 3;
				}
				else if (i == 5) {
					x += 3;
				}
				else if (i == 6) {
					x -= 3;
					y -= 3;
				}
				else if (i == 7) {
					y -= 3;
				}
				else if (i == 8) {
					x = 3;
					y = -3;
				}

				drawSquare(x, y, z, i + 2);
			}

			else
			{
				if (i == 0) {
					x -= 3;
					y += 3;
				}
				else if (i == 1) {
					y += 3;
				}
				else if (i == 2) {
					x += 3;
					y += 3;
				}
				else if (i == 3) {
					x -= 3;
				}
				else if (i == 5) {
					x += 3;
				}
				else if (i == 6) {
					x -= 3;
					y -= 3;
				}
				else if (i == 7) {
					y -= 3;
				}
				else if (i == 8) {
					x = 3;
					y = -3;
				}

				if (moveList[i] == 'x')
				{
					drawX(x, y, z, i + 2);
				}

				else if (moveList[i] == 'o')
				{
					drawSphere(x, y, z, i + 2);
				}
			}
			glPopMatrix();
			hlEndShape();
		}

		// End the haptic frame.
		hlEndFrame();

		// Call any event callbacks that have been triggered.
		hlCheckEvents();
	}

	/*******************************************************************************
	Draws a 3D cursor for the haptic device using the current local transform,
	the workspace to world transform and the screen coordinate scale.
	*******************************************************************************/
	void drawHapticCursor()
	{
		static const double kCursorRadius = 0.5;
		static const double kCursorHeight = 1.5;
		static const int kCursorTess = 15;
		HLdouble proxyxform[16];
		GLUquadricObj *qobj = 0;

		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
		glPushMatrix();

		if (!gCursorDisplayList) 
		{
			gCursorDisplayList = glGenLists(1);
			glNewList(gCursorDisplayList, GL_COMPILE);
			qobj = gluNewQuadric();

			gluCylinder(qobj, 0.0, kCursorRadius, kCursorHeight, kCursorTess, kCursorTess);
			glTranslated(0.0, 0.0, kCursorHeight);
			gluCylinder(qobj, kCursorRadius, 0.0, kCursorHeight / 5.0, kCursorTess, kCursorTess);

			gluDeleteQuadric(qobj);
			glEndList();
		}

		// Get the proxy transform in world coordinates for haptic device.
		hlGetDoublev(HL_PROXY_TRANSFORM, proxyxform);
		glMultMatrixd(proxyxform);

		// Apply the local cursor scale factor.
		glScaled(gCursorScale, gCursorScale, gCursorScale);
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.0, 0.5, 1.0);
		glCallList(gCursorDisplayList);
		glPopMatrix();
		glPopAttrib();
	}

#endif

// haptic code finished

// Draw 3d stereoscopic view
void drawInStereo() 
{
	// one camera - 2 frustums --> stereoscopy with each eye equal to one frustrum
	if (stereoMethod == 1) 
	{
		drawEye(1); // draw left eye
		drawEye(2); // draw right eye
	}

	else 
	{
		// frustum parameters for 2 cameras --> stereoscopy with each eye equal to one camera
		drawEyeLookAt();
	}
}

// With 3d stereoscopic view
void drawEye(int eye) 
{
	float dx0 = -0.070 / 4.0;
	float dx1 = 0.40 / 4.0;

	glDrawBuffer(GL_BACK_LEFT);

	if (eye == 1) 
	{
		glDrawBuffer(GL_BACK_RIGHT);
		dx0 = -dx0;
		dx1 = -dx1;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-frustumRight - dx0, frustumRight - dx0, -frustumTop, frustumTop, nNear, nFar);
	glTranslatef(0 + dx1, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	drawScene();
	glFlush();
}

// Without stereoscopic view
void drawMono() 
{
	float radians = DTOR * camera.aperture / 2;
	float wd2 = nearP * tan(radians);
	float ndfl = nearP / camera.focalLength;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	leftt = -ratio * wd2;
	rightt = ratio * wd2;
	top = wd2;
	bottom = -wd2;

	glFrustum(leftt, rightt, bottom, top, nearP, farP);
	glMatrixMode(GL_MODELVIEW);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.vp.x, camera.vp.y, camera.vp.z,
						   camera.vp.x + camera.vd.x,
						   camera.vp.y + camera.vd.y,
						   camera.vp.z + camera.vd.z,
						   camera.vu.x, camera.vu.y, camera.vu.z);
	drawScene();
}

// With stereoscopic view
void drawEyeLookAt() 
{
	float radians = DTOR * camera.aperture / 2;
	float wd2 = nearP * tan(radians);
	float ndfl = nearP / camera.focalLength;

	XYZ r = crossProduct(camera.vd, camera.vu);
	normalise(&r);

	r.x *= camera.eyeSep / 2.0;
	r.y *= camera.eyeSep / 2.0;
	r.z *= camera.eyeSep / 2.0;

	// draw right eye - camera 1

	glDrawBuffer(GL_BACK_RIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	leftt = -ratio * wd2 - 0.5 * camera.eyeSep * ndfl;
	rightt = ratio * wd2 - 0.5 * camera.eyeSep * ndfl;
	top = wd2;
	bottom = -wd2;

	glFrustum(leftt, rightt, bottom, top, nearP, farP);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.vp.x + r.x, camera.vp.y + r.y, camera.vp.z + r.z,
								 camera.vp.x + r.x + camera.vd.x,
								 camera.vp.y + r.y + camera.vd.y,
								 camera.vp.z + r.z + camera.vd.z,
								 camera.vu.x, camera.vu.y, camera.vu.z);
	drawScene();
	glFlush();

	// draw left eye - camera 2

	glDrawBuffer(GL_BACK_LEFT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	leftt = -ratio * wd2 + 0.5 * camera.eyeSep * ndfl;
	rightt = ratio * wd2 + 0.5 * camera.eyeSep * ndfl;
	top = wd2;
	bottom = -wd2;

	glFrustum(leftt, rightt, bottom, top, nearP, farP);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.vp.x - r.x, camera.vp.y - r.y, camera.vp.z - r.z,
								 camera.vp.x - r.x + camera.vd.x,
								 camera.vp.y - r.y + camera.vd.y,
							 	 camera.vp.z - r.z + camera.vd.z,
								 camera.vu.x, camera.vu.y, camera.vu.z);
	drawScene();
	glFlush();
}

void drawScene()
{
	drawObject();

#ifdef HAPTIC

	drawHapticCursor();

#endif

	showInfo();
	showFPS();
}

void normalise(XYZ *p)
{
	double length;
	length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);

	if (length != 0)
	{
		p->x /= length;
		p->y /= length;
		p->z /= length;
	}

	else
	{
		p->x = 0;
		p->y = 0;
		p->z = 0;
	}
}

XYZ crossProduct(XYZ p1, XYZ p2)
{
	XYZ p3;

	p3.x = p1.y*p2.z - p1.z*p2.y;
	p3.y = p1.z*p2.x - p1.x*p2.z;
	p3.z = p1.x*p2.y - p1.y*p2.x;

	return p3;
}
