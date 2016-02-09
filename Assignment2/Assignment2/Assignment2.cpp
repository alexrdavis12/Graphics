/*
Alexandria Davis
CSCE 441 - Spring 2016
*/

#include <stdlib.h>
#include <gl\glut.h>
#include <stddef.h>
#include <iostream>

static GLfloat spin = 0.0;
static int CURRENT_LAYOUT = GL_POINTS;
static int MAX = 50;
static int MIN = -50;
static double curr_y=NULL;
static double curr_x = NULL;
static int start_x = NULL;
static int start_y = NULL;
static GLfloat MAXSPEED = 1.0;
static double MAX_COLOR = 1.0;
static GLfloat speed = .5;
static GLfloat defspeed = .5;
bool clicked_left = false;
bool clicked_right = false;

static double red = 0.5;
static double green = 0.5;
static double blue = 0.5;

static bool middleclick = false;
static int mlocx = NULL;
static int mlocy = NULL;
static double xdiff = 0;
static double ydiff = 0;

static int p1x = 0;
static int p1y = -20;
static int p2x = -19;
static int p2y = -6;
static int p3x = -12;
static int p3y = 16;
static int p4x = 12;
static int p4y = 16;
static int p5x = 19;
static int p5y = -6;


void init(void)
{
	glClearColor(red, green,blue, 0.0);
	glShadeModel(GL_FLAT);
	glPointSize(5);

}

void display(void)
{
	glClearColor(red, green, blue, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(spin, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(CURRENT_LAYOUT);
		glVertex2d(p1x, p1y); // Vertices of the pentagon
		glVertex2d(p2x, p2y);
		glVertex2d(p3x, p3y);
		glVertex2d(p4x, p4y);
		glVertex2d(p5x, p5y);
	glEnd();
	//glRectf(-25.0, -25.0, 25.0, 25.0);
	glPopMatrix();
	glutSwapBuffers();
}

void spinDisplayL(void)
{
	spin = spin - speed;
	if (speed < 0)
		spin = spin + 360.0;
}

void spinDisplayR(void)
{
	spin=spin + speed;
	if (spin > 360.0)
		spin = spin - 360.0;
}

void adjustSpeedandColor(void)
{
	speed = ((curr_y + 50.0) / 100.0)*MAXSPEED;
	// if (speed == 0)
		// speed = 360;
	double val = ((double)curr_x + 50.0) / 100.0; //percentage of screen determines intensity
	if (val > 0) {
		if (red > 0)
			red = MAX_COLOR*val;
		if (green > 0)
			green = MAX_COLOR*val;
		if (blue > 0)
			blue = MAX_COLOR*val;
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1': //Display Points
		 CURRENT_LAYOUT = GL_POINTS;
		 glutPostRedisplay();
		 break;
	case '2': // Display Outline
		CURRENT_LAYOUT = GL_LINE_LOOP;
		glutPostRedisplay();
		 break;
	case '3': // Display Polygon
		CURRENT_LAYOUT = GL_POLYGON;
		glutPostRedisplay();
		 break;
	case 'c': // Background to cyan
		red = 0; green = .5; blue = .5; //start at 50% intensity
		glutPostRedisplay(); 
		break;
	case 'm': // Background to magenta
		red = .5; green = 0; blue = .5;//start at 50% intensity
		glutPostRedisplay();
		break;
	case 'y': // Background to yellow
		red = .5; green = .5; blue = 0;//start at 50% intensity
		glutPostRedisplay();
		 break;
	case 'w': // Background to white
		//glClearColor(1.0, 1.0, 1.0, 1.0);
		red = .5 ; green = .5; blue = .5 ;//start at 50% intensity
		glutPostRedisplay();
		break;
	}
	glutPostRedisplay();
}
void idleFunc(void) { // Allows me to have things running constantly in the background
	if (middleclick)
	{
		ydiff = mlocy - curr_y;
		xdiff = mlocx - curr_x;
		mlocy = curr_y;
		mlocx = curr_x;
		p1x += xdiff; p2x += xdiff; p3x += xdiff; p4x += xdiff; p5x += xdiff;
		p1y += ydiff; p2y += ydiff; p3y += ydiff; p4y += ydiff; p5y += ydiff;
		glutPostRedisplay();
	}
	if (clicked_left)
	{
		adjustSpeedandColor();
		spinDisplayL();
		glutPostRedisplay();
	}
	else if (clicked_right)
	{
		adjustSpeedandColor();
		spinDisplayR();
		glutPostRedisplay();
	}
	
}

void trackLocs(int x, int y)
{
	if (x <= 250 && x >= 0 && y >= 0 && y <= 250) {
		curr_x = (x / 2.5) - 50; //convert the
		curr_y = -1*((y / 2.5) - 50);
	}
	std::cout << "X: " << curr_x << " Y: " << curr_y;
}

void mouse(int button, int state, int x, int y)
{
	std::cout << "Inside mouse handler";
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			clicked_left = true;
			 // when  clicked store the location no adjustments to speed needed
			start_y = y;
			start_x = x;
			//curr_x = x; curr_y = y;
			//adjustSpeedandColor(); 
		}
		else if (state == GLUT_UP) {
			
			clicked_left = false; 
			speed = defspeed; // put the speed back to default so that any changes are forgotten
			start_x = NULL; // the current position should also be forgotten
			start_y = NULL;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			 // when first clicked store the location no adjustments to speed needed
				start_y = y;
				start_x = x;
			
				clicked_right = true;
		}
		else if (state == GLUT_UP) {
			clicked_right = false; //stop spinning
			speed = defspeed; // put the speed back to default so that any changes are forgotten
			start_x = NULL; // the current position should also be forgotten
			start_y = NULL;
			
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
		{
			mlocx = (x/2.5)-50;
			mlocy = (y/2.5)-50;
			middleclick = true;
		}
		else if (state == GLUT_UP)
		{
			middleclick = false;
			mlocx = 0;
			mlocy = 0;
		}
		break;
	default:
		break;
	}
}

/*
*  Request double buffer display mode.
*  Register mouse input callback functions
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(250,250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Alexandria Davis - Assignment 2"); //Title Requirement
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(trackLocs);	 // used to do the tracking needed to change speed and intensity
	glutIdleFunc(idleFunc);
	glutMainLoop();
	
	return 0;
}