1.  [5 points]  Send an email cc’d to both the instructor and TA.  Include the following information in the message:  Your name, classification (junior/senior/grad student), major, and one or two interesting facts about yourself.  The email address sent from should be the one you want to use to receive information about the class.
[1 point each – name, classification, major]
[2 points – one or two facts]
 
2.     [25 points]  The purpose of this assignment is mainly to familiarize you with the basic process for creating a window using GLUT, and some very basic OpenGL commands.  Implement the program found on page 46 of Hearn and Baker (you should be able to copy it exactly).  You may work with others as much as you want in order to get this working (but not the remainder).  Once you have it displaying a red line on a white background, experiment with the following:
·        Changing the values passed to the glClearColor, glColor3f, glVertex2i, gluOrtho2D, glutInitWindowSize, glutInitWindowPosition, and glutCreateWindow routines.
·        Adding or removing additional glVertex2i commands.
Note: you should not change the values for the last value in glClearColor (keep it 0.0).  Be careful when changing values for gluOrtho2D and glutInitWindowSize.  You should probably only try changing one of them at a time until you get a feel for exactly what each one does, and how they work together.  We will be coming back to how they work together later in the class – for now I want you to get a more intuitive feel for it.
 
When you have experimented with this for a while, create a program to do the following (you can modify the program from the book):
·        Create a rectangular window with a width to height ratio of 2:1, titled “<your name> - Assignment 1”
·        In the window draw a hexagon (outline only, not filled).  The sides of the hexagon should be white, green, purple, orange, brown, and yellow, not necessarily in that order.  The hexagon should be approximately in the middle of the window, and should be regular (sides same length, and angles the same).  The background should be black.