/*
Alexandria Davis
CSCE 441 - Spring 2016
*/

#include <stdlib.h>
#include <gl\glut.h>
#include <stddef.h>
#include <iostream>
//#include<fstream.h>
#include<math.h>
#include "edges.h"

/******************************************************************
Notes:
Image size is 400 by 400 by default.  You may adjust this if
you want to.
You can assume the window will NOT be resized.
Call clearFramebuffer to clear the entire framebuffer.
Call setFramebuffer to set a pixel.  This should be the only
routine you use to set the color (other than clearing the
entire framebuffer).  drawit() will cause the current
framebuffer to be displayed.
As is, your scan conversion should probably be called from
within the display function.  There is a very short sample
of code there now.
You may add code to any of the subroutines here,  You probably
want to leave the drawit, clearFramebuffer, and
setFramebuffer commands alone, though.
*****************************************************************/

#define ImageW 400
#define ImageH 400

float framebuffer[ImageH][ImageW][3];
PolygonManager pm;
bool definingNewPolygon = true;
bool clippingMode = false;
bool leftclicked = false;
ClippingWindow cwindow;
struct color {
	float r, g, b;		// Color (R,G,B values)
};

// Draws the scene
void drawit(void)
{
	glDrawPixels(ImageW, ImageH, GL_RGB, GL_FLOAT, framebuffer);
	glFlush();
}

// Clears framebuffer to black
void clearFramebuffer()
{
	int i, j;

	for (i = 0; i<ImageH; i++) {
		for (j = 0; j<ImageW; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer 
void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y;
	if (R <= 1.0)
		if (R >= 0.0)
			framebuffer[y][x][0] = R;
		else
			framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (G <= 1.0)
		if (G >= 0.0)
			framebuffer[y][x][1] = G;
		else
			framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (B <= 1.0)
		if (B >= 0.0)
			framebuffer[y][x][2] = B;
		else
			framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
}

void display(void)
{
	//The next two lines of code are for demonstration only.
	//They show how to draw a line from (0,0) to (100,100)
	//int i;
	//for (i = 0; i <= 100; i++) setFramebuffer(i, i, 1.0, 1.0, 1.0);

	drawit();
}

void init(void)
{
	clearFramebuffer();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				if (!clippingMode) {
					//std::cout << "begin adding new point\n";
					if (definingNewPolygon)
					{
						//std::cout << "starting definition of new polygon";
						if (pm.newPolygon(Point(x, y)))
							definingNewPolygon = false;
						else
							std::cout << "You can only define 10 polygons";
					}
					else if (!pm.addPoint(Point(x, y)))
					{
						std::cout << "You can only define 10 verticies";
					}
				}
				else { // clipping mode has been enabled
					clearFramebuffer();
					pm.redrawPolygons();
					leftclicked = true;
					cwindow = ClippingWindow(x, y);
				}
			}
			else if (state == GLUT_UP)
			{
				if (clippingMode) {
					leftclicked = false;
					clearFramebuffer();
					pm.redrawPolygons();
					cwindow.updateDraw(x, y);
					cwindow.finishDraw(x, y);
					//pm.clipPolygons(cwindow);  // commented out so that it would not segfault... was unable to fix before the deadline
				}
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				//std::cout << "ending the current polygon\n";
				if (!definingNewPolygon) // cannot end a polygon that has not been started
				{
					
					pm.endPolygon(Point(x, y));
					definingNewPolygon = true;
				}
				else
					std::cout << "you must start a polygon before it can be ended\n";
			}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		clippingMode = !clippingMode; //use the c key to toggle clipping mode
		break;
	default:
		break;
	}
}

void trackLocs(int x, int y)
{
	if (leftclicked) {
		clearFramebuffer();
		pm.redrawPolygons();
		cwindow.updateDraw(x, y);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ImageW, ImageH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Alexandria Davis - Homework 3");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(trackLocs);
	glutIdleFunc(drawit);
	glutMainLoop();
	return 0;
}