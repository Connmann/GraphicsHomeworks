/*
 *  3D Objects
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  m/M        Cycle through different sets of objects
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int axes=1;       //  Display axes
int mode=0;       //  What to display

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void wheel(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int d,
                 int s)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glColor3f(0.5,0.5,0.5);
   //Wheel
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, 0, 0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glColor3f(0.5,0.5,0.5);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), -0.05);
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glVertex3d(Sin(0)*Cos(-90) , Cos(0)*Cos(-90), -0.05);
   glVertex3d(Sin(0)*Cos(-90) , Cos(0)*Cos(-90), 0.05);
   glEnd();

   //Rim
   d = d * 0.7;
   glColor3f(0.8,0.8,0.8);
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, 0, -0.055);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), -0.055);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, 0, 0.055);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.055);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();

}

static void bumper(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int m)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Bumper
   glBegin(GL_POLYGON);
   glVertex3f(0,0.2,0.4);
   glVertex3f(0,0.2,-0.4);
   glVertex3f(0.1,0.2,-0.35);
   glVertex3f(0.1,0.2,0.35);
   glEnd();

   glBegin(GL_POLYGON);
   glVertex3f(0,0,0.4);
   glVertex3f(0,0,-0.4);
   glVertex3f(0.1,0,-0.35);
   glVertex3f(0.1,0,0.35);
   glEnd();

   glBegin(GL_QUAD_STRIP);
   glVertex3f(0, 0, 0.4);
   glVertex3f(0.1, 0, 0.35);
   glVertex3f(0, 0.2, 0.4);
   glVertex3f(0.1, 0.2, 0.35);

   glVertex3f(0.1, 0, 0.35);
   glVertex3f(0.1, 0, -0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0.1, 0.2, -0.35);
      
   glVertex3f(0, 0, -0.4);
   glVertex3f(0.1, 0, -0.35);
   glVertex3f(0.0, 0.2, -0.4);
   glVertex3f(0.1, 0.2, -0.35);
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0, 0.25, 0.35);
   glVertex3f(0, 0.25, -0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glVertex3f(0, 0.25, 0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0, 0.2, 0.4);

   glVertex3f(0, 0.25, -0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0, 0.2, -0.4);
   glEnd();

   //Lights (taillights or headlights)
   if(m == 1) {
      glColor3f(1, 1, 0.8);
   } else {
      glColor3f(.8, 0, 0);
   }

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0.11, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glVertex3d(0.11, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0.11, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glVertex3d(0.11, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   if (m == 1) {
      glColor3f(0.1,0.1,0.1);
      //Grill
      glBegin(GL_QUADS);
      glVertex3f(0.11, 0.05, 0.18);
      glVertex3f(0.11, 0.15, 0.18);
      glVertex3f(0.11, 0.15, -0.18);
      glVertex3f(0.11, 0.05, -0.18);
      glEnd();
   }

   //Undo transformations
   glPopMatrix();  
}

static void car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 float cr, float cb, float cg)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glColor3f(cr, cb, cg);
  
   //Lower Body
   cube(0,0.1,0, 0.8,0.1,0.4, 0);
   //Cabin
   cube(-0.1,0.3,0, 0.3,0.1,0.35, 0);

   wheel(0.6,0,0.4, dx,dy,dz, 0, 8, 10);
   wheel(-0.6,0,-0.4, dx,dy,dz, 0, 8, 10);
   wheel(0.6,0,-0.4, dx,dy,dz, 0, 8, 10);
   wheel(-0.6,0,0.4, dx,dy,dz, 0, 8, 10);

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);
   glColor3f(cr, cb, cg);

   //Hood
   glBegin(GL_QUAD_STRIP);
   glVertex3f(0.8, 0.2, 0.4);
   glVertex3f(-0.8, 0.2, 0.4);
   glVertex3f(0.8, 0.25, 0.35);
   glVertex3f(-0.8, 0.25, 0.35);

   glVertex3f(0.8, 0.25, 0.35);
   glVertex3f(0.4, 0.25, 0.35);
   glVertex3f(0.8, 0.25, -0.35);
   glVertex3f(0.4, 0.25, -0.35);
   
   glVertex3f(0.8, 0.2, -0.4);
   glVertex3f(-0.8, 0.2, -0.4);
   glVertex3f(0.8, 0.25, -0.35);
   glVertex3f(-0.8, 0.25, -0.35);
   glEnd();

   //Windshield
   glColor3f(0.8, 0.8, 1);
   glBegin(GL_QUADS);
   glVertex3f(0.4,0.25,-0.35);
   glVertex3f(0.4,0.25,0.35);
   glVertex3f(0.2,0.4,0.35);
   glVertex3f(0.2,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glVertex3f(0.4,0.25,0.35);
   glVertex3f(0.2,0.25,0.35);
   glVertex3f(0.2,0.4,0.35);

   glVertex3f(0.4,0.25,-0.35);
   glVertex3f(0.2,0.25,-0.35);
   glVertex3f(0.2,0.4,-0.35);
   glEnd();

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);
   glColor3f(cr, cb, cg);

   //Trunk
   glBegin(GL_QUADS);
   glVertex3f(-0.8, 0.25, 0.35);
   glVertex3f(-0.8, 0.25, -0.35);
   glVertex3f(-0.6, 0.25, -0.35);
   glVertex3f(-0.6, 0.25, 0.35);
   glEnd();

   //Rear Window
   glColor3f(0.8, 0.8, 1);
   glBegin(GL_QUADS);
   glVertex3f(-0.6,0.25,-0.35);
   glVertex3f(-0.6,0.25,0.35);
   glVertex3f(-0.4,0.4,0.35);
   glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glVertex3f(-0.6,0.25,0.35);
   glVertex3f(-0.4,0.25,0.35);
   glVertex3f(-0.4,0.4,0.35);
   
   glVertex3f(-0.6,0.25,-0.35);
   glVertex3f(-0.4,0.25,-0.35);
   glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glColor3f(0.1,0.1,0.1);
   //Spoiler
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0);

   glBegin(GL_QUAD_STRIP);
   glVertex3f(-0.7,0.31,-0.35);
   glVertex3f(-0.8,0.31,-0.35);
   glVertex3f(-0.7,0.31,0.35);
   glVertex3f(-0.8,0.31,0.35);
  
   glVertex3f(-0.7,0.31,-0.35);
   glVertex3f(-0.8,0.33,-0.35);
   glVertex3f(-0.7,0.31,0.35);
   glVertex3f(-0.8,0.33,0.35);

   glVertex3f(-0.8,0.31,-0.35);
   glVertex3f(-0.8,0.33,-0.35);
   glVertex3f(-0.8,0.31,0.35);
   glVertex3f(-0.8,0.33,0.35);
   glEnd();

   glColor3f(cr, cb, cg);

   glBegin(GL_TRIANGLES);
   glVertex3f(-0.68,0.31,-0.35);
   glVertex3f(-0.82,0.31,-0.35);
   glVertex3f(-0.82,0.35,-0.35);

   glVertex3f(-0.68,0.31,0.35);
   glVertex3f(-0.82,0.31,0.35);
   glVertex3f(-0.82,0.35,0.35);
   glEnd();

   //Undo transformations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);

   //Red car
   car(-1,0,-0.8, 0.5,0.5,0.5, 0, 1,0,0);
   //Blue car
   car(-1,0,0.8, 1,1,1, 0, 0,0,0.8);
   //Green car
   car(1,0,0.8, 1,1,1, 180, 0,0.5,0);
   //Teal car
   car(1.5,0,-1, 1,1,1, 220, 0,0.8,0.8);

   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,25);
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm')
      mode = (mode+1)%7;
   else if (ch == 'M')
      mode = (mode+6)%7;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   const double dim=2.5;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Objects");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
