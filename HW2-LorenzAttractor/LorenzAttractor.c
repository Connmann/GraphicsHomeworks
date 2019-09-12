/*
 *  Hello World Triangle Version 5
 */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
double th=0;  //Rotation angle (Y axis)
double xh=0;  //Tilt angle (X axis)
double zh=0;  //  Spin angle


/*
 * Convenience function for text
 */
#define LEN 8192  //  Maximum amount of text
void Print(const char* format , ...)
{
   char    buf[LEN]; // Text storage
   char*   ch=buf;   // Text pointer
   //  Create text to be display
   va_list args;
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display text string
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Function to print any errors encountered
 */
void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 * This function is called by GLUT to display the scene
 */
void display()
{
   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //  Reset transformations
   glLoadIdentity();
   
   glColor3f(1.0,1.0,1.0);

   //X Axis
   glBegin(GL_LINE_STRIP);
   glVertex3f(0.0, 0.0, 0.0);
   glVertex3f(400.0, 0.0, 0.0);
   glVertex3f(0.0, 0.0, 0.0);
   glVertex3f(0.0, 400.0, 0.0);
   glVertex3f(0.0, 0.0, 0.0);
   glVertex3f(0.0, 0.0, 400.0);
   glEnd();

   glTranslatef(0.0, 0.0, -100.0);
   //Rotate around Y axis
   glRotatef(th,0.0,1.0,0.0);
   //Rotate around X axis
   glRotatef(xh,1.0,0.0,0.0);

   // //Y Axis
   // glBegin(GL_LINE_STRIP);
   // glVertex3f(0.0, 0.0, 0.0);
   // glVertex3f(0.0, 400.0, 0.0);
   // glEnd();

   // glTranslatef(0.0, 0.0, -400.0);
   // //Rotate around Y axis
   // glRotatef(th,0.0,1.0,0.0);
   // //Rotate around X axis
   // glRotatef(xh,1.0,0.0,0.0);

   // //Z Axis
   // glBegin(GL_LINE_STRIP);
   // glVertex3f(0.0, 0.0, 0.0);
   // glVertex3f(0.0, 0.0, 400.0);
   // glEnd();

   // glTranslatef(0.0, 0.0, -400.0);
   // //Rotate around Y axis
   // glRotatef(th,0.0,1.0,0.0);
   // //Rotate around X axis
   // glRotatef(xh,1.0,0.0,0.0);
  

   glColor3f(1.0,0.0,0.0);

   //Draw Lorenz Attractor

   /*  Lorenz Parameters  */
   double s  = 10;
   double b  = 2.6666;
   double r  = 28;

   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;

   //printf("%5d %8.3f %8.3f %8.3f\n",0,x,y,z);
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   glBegin(GL_LINE_STRIP);
   for (i=0;i<50000;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      // printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
      glVertex3f(x, y, z);
   }
   glEnd();

   glTranslatef(0.0, 0.0, -400.0);
   //Rotate around Y axis
   glRotatef(th,0.0,1.0,0.0);
   //Rotate around X axis
   glRotatef(xh,1.0,0.0,0.0);


   //  Sanity check
   ErrCheck("display");
   //  Flush and swap buffer
   glFlush();
   glutSwapBuffers();
}

/*
 * This function is called by GLUT when special keys are pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow - increase rotation by 5 degree
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //Up arrow - Increase tilt by 5 degrees
   else if (key == GLUT_KEY_UP)
      xh += 5;
   //Down arrow - Decrease tilt by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      xh -= 5;


   //  Request display update
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height)
{
   //  Calculate width to height ratio
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set viewport as entire window
   glViewport(0,0, width,height);
   //  Select projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Set projection to identity
   glLoadIdentity();

   gluPerspective(90,w2h,-1.0,1.0);


   //  Select model view matrix
   glMatrixMode(GL_MODELVIEW);
   //  Set model view to identity
   glLoadIdentity();

   gluLookAt(0.0,30.0,0.0, 0.0,0.0,-100.0, 0.0,1.0,0.0);

}

/*
 * This function is called by GLUT when idle
 */
void idle()
{
   //  Get elapsed (wall) time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   //  Calculate spin angle 90 degrees/second
   zh = fmod(90*t,360);
   //  Request display update
   glutPostRedisplay();
}

/*
 * GLUT based Hello World
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered true color window with Z-buffer
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //  Create window
   glutCreateWindow("Connor Guerin - Lorenz Attractor");
   //  Register display, reshape, idle and key callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutSpecialFunc(special);
   //  Enable Z-buffer depth test
   glEnable(GL_DEPTH_TEST);
   //  Pass control to GLUT for events
   glutMainLoop();
   //  Return to OS
   return 0;
}
