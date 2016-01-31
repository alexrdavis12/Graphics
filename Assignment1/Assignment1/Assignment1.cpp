/* 
	Alexandria Davis
	CSCE 441 - Spring 2016
*/

#include <gl\glut.h>
void init(void)
{
	glClearColor(0.0, 0.0,0, 0.0); // Set display-window color to white.
	glMatrixMode(GL_PROJECTION); // Set projection parameters.
	gluOrtho2D(0.0, 600.0, 0.0, 300.0); // Coordinates in window go from 0,0 to 600,300
}

void hexagon(void)
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window.
	glColor3f(0,1,0); // Green
	glBegin(GL_LINES);//GL_LINE_LOOP

	//Top Left line
	glVertex2i(250, 150); 
	glVertex2i(275, 193.3); 

	//Top Line
	glColor3f(1, 1, 1); // White
	glVertex2i(275, 193.3);
	glVertex2i(325, 193.3);

	//Top Right line
	glColor3f(1, .5, 0); //orange
	glVertex2i(325, 193.3);
	glVertex2i(350, 150);

	//Bottom Right Line
	glColor3f(.36, .25, .20); // Brown
	glVertex2i(350, 150);
	glVertex2i(325, 106.7);

	//Bottom Line
	glColor3f(.53, .12, .47); //Purple
	glVertex2i(325, 106.7);
	glVertex2i(275, 106.7); // Specify line-segment geometry.

	//Bottom Left Line
	glColor3f(1, 1, 0); // Yellow
	glVertex2i(275, 106.7);
	glVertex2i(250, 150);

	
	glEnd();
	glFlush(); // Process all OpenGL routines as quickly as possible.
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv); // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode.
	glutInitWindowPosition(50, 100); // Set top-left display-window position.
	glutInitWindowSize(600, 300); // Set display-window width and height. 2:1 ratio
	
	glutCreateWindow("Alexandria Davis - Assignment 1"); // Create display window.
	init(); // Execute initialization procedure.
	glutDisplayFunc(hexagon); // Send graphics to display window.
	glutMainLoop(); // Display everything and wait.
}
