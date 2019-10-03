/*
 *  Homework 4 - Projections
 *  Connor Guerin
 *
 *  Key bindings:
 *  m          Toggle between perspective and orthogonal
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *  w/s        Move forwards/backwards
 *  c          Change color mode
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

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=6.0;   //  Size of world
int color=0;      //Color Scheme
double fpMoveInc = 0.05; //Multiplier for how much to move each keystroke in FP mode

//First person camera location
double fpX = 0;
double fpY = 0.45;
double fpZ = 0;

//x, y, z for refrence point in glLookAt() for FP mode
double refX = 5;
double refY = 0;
double refZ = 0;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

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
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode == 0)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else if (mode == 1)
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   else if (mode == 2)
      gluPerspective(fov,asp,0.1,4*dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     w (1 to color windows for car body, 0 otherwise)
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int w)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
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

   //Color to add windows
   if(w == 1) glColor3f(0.8, 0.8, 1);
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

   glColor3f(0.3,0.3,0.3);
   //Wheel
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, 0, 0.05);
   for (th=360;th>=0;th-=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glColor3f(0.3,0.3,0.3);
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
   for (th=360;th>=0;th-=s)
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

   //Offset the bumper so that the lights and grill are drawn directly on the surface
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);

   //Bumper
   if(color == 1) glColor3f(1, 0, 1);
   glBegin(GL_POLYGON);
   glVertex3f(0,0,0.4);
   glVertex3f(0,0,-0.4);
   glVertex3f(0.1,0,-0.35);
   glVertex3f(0.1,0,0.35);
   glEnd();

   if(color == 1) glColor3f(1, 0, 0);
   glBegin(GL_QUADS);
   glVertex3f(0.1, 0, 0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0, 0.2, 0.4);
   glVertex3f(0, 0, 0.4);

   if(color == 1) glColor3f(0.5, 1, 0.5);
   glVertex3f(0.1, 0, -0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0.1, 0, 0.35);

   if(color == 1) glColor3f(1, 0.5, 1);
   glVertex3f(0.1, 0, -0.35);
   glVertex3f(0, 0, -0.4);
   glVertex3f(0, 0.2, -0.4);
   glVertex3f(0.1, 0.2, -0.35);
   
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   if(color == 1) glColor3f(1, 0.8, 1);
   glVertex3f(0, 0.25, 0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0, 0.25, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   if(color == 1) glColor3f(1, 0.5, 0.7);
   glVertex3f(0, 0.2, 0.4);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0, 0.25, 0.35);

   if(color == 1) glColor3f(0.6, 0.5, 1);
   glVertex3f(0, 0.25, -0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0, 0.2, -0.4);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   //Lights (taillights or headlights)
   if(m == 1) {
      glColor3f(1, 1, 0.8);
   } else {
      glColor3f(.8, 0, 0);
   }

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0.1, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0.1, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   if (m == 1) {
      glColor3f(0.1,0.1,0.1);
      //Grill
      glBegin(GL_QUADS);
      glVertex3f(0.1, 0.15, 0.18);
      glVertex3f(0.1, 0.05, 0.18);
      glVertex3f(0.1, 0.05, -0.18);
      glVertex3f(0.1, 0.15, -0.18);
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
   cube(0,0.1,0, 0.8,0.1,0.4, 0, 0);
   //Cabin
   cube(-0.1,0.3,0, 0.3,0.1,0.35, 0, 1);

   wheel(0.6,0,0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,0.4, 1,1,1, 0, 8, 10);

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);
   glColor3f(cr, cb, cg);

   //Hood
   glBegin(GL_QUADS);
   if(color == 1) glColor3f(0.5, 0.6, 0.2);
   glVertex3f(-0.8, 0.25, 0.35);
   glVertex3f(-0.8, 0.2, 0.4);
   glVertex3f(0.8, 0.2, 0.4);
   glVertex3f(0.8, 0.25, 0.35);

   if(color == 1) glColor3f(0.9, 0.1, 0.6);
   glVertex3f(0.4, 0.25, 0.35);
   glVertex3f(0.8, 0.25, 0.35);
   glVertex3f(0.8, 0.25, -0.35);
   glVertex3f(0.4, 0.25, -0.35);
   
   if(color == 1) glColor3f(0.6, 0, 0.1);
   glVertex3f(-0.8, 0.2, -0.4);
   glVertex3f(-0.8, 0.25, -0.35);
   glVertex3f(0.8, 0.25, -0.35);
   glVertex3f(0.8, 0.2, -0.4);
   
   glEnd();

   //Windshield
   glColor3f(0.8, 0.8, 1);
   glBegin(GL_QUADS);
   glVertex3f(0.4,0.25,0.35);
   glVertex3f(0.4,0.25,-0.35);
   glVertex3f(0.2,0.4,-0.35);
   glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glVertex3f(0.2,0.4,0.35);
   glVertex3f(0.2,0.25,0.35);
   glVertex3f(0.4,0.25,0.35);

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
   if(color == 1) glColor3f(.8, 0, 0.6);
   glVertex3f(-0.8, 0.25, -0.35);
   glVertex3f(-0.8, 0.25, 0.35);
   glVertex3f(-0.6, 0.25, 0.35);
   glVertex3f(-0.6, 0.25, -0.35);
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
   
   glVertex3f(-0.4,0.4,-0.35);
   glVertex3f(-0.4,0.25,-0.35);
   glVertex3f(-0.6,0.25,-0.35);
   
   glEnd();

   glColor3f(0.1,0.1,0.1);
   //Spoiler
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0, 0);

   glBegin(GL_QUADS);
   if(color == 1) glColor3f(0.1,1,0.1);
   glVertex3f(-0.7,0.31,-0.35);
   glVertex3f(-0.7,0.31,0.35);
   glVertex3f(-0.8,0.31,0.35);
   glVertex3f(-0.8,0.31,-0.35);
   
   if(color == 1) glColor3f(0.1,0.5,0.1);
   glVertex3f(-0.8,0.33,-0.35);
   glVertex3f(-0.8,0.33,0.35);
   glVertex3f(-0.7,0.31,0.35);
   glVertex3f(-0.7,0.31,-0.35);

   if(color == 1) glColor3f(0,0,1);
   glVertex3f(-0.8,0.33,0.35);
   glVertex3f(-0.8,0.33,-0.35);
   glVertex3f(-0.8,0.31,-0.35);
   glVertex3f(-0.8,0.31,0.35);
   
   glEnd();

   glColor3f(cr, cb, cg);

   glBegin(GL_TRIANGLES);
   glVertex3f(-0.68,0.31,-0.35);
   glVertex3f(-0.82,0.31,-0.35);
   glVertex3f(-0.82,0.35,-0.35);

   glVertex3f(-0.82,0.35,0.35);
   glVertex3f(-0.82,0.31,0.35);
   glVertex3f(-0.68,0.31,0.35);
   
   //Duplicate to draw both sides when face culling is on
   glVertex3f(-0.82,0.35,-0.35);
   glVertex3f(-0.82,0.31,-0.35);
   glVertex3f(-0.68,0.31,-0.35);
   
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
   //Enable Face Culling
   glEnable(GL_CULL_FACE);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode == 0)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else if (mode == 1)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //First Person Perspective
   else if (mode == 2)
   {
      //Lock the dim to avoid weird behavior when the camera strays beyond it
      dim = 7.0;

      refX = (dim * Sin(th)) + fpX;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + fpZ;
      gluLookAt(fpX,fpY,fpZ, refX,refY,refZ, 0,1,0);
   }
   
   //All cars and surface
   //Red car
   car(-2,-0.07,-0.8, 0.5,0.5,0.5, 0, 1,0,0);
   //Blue car
   car(-2,0,0.8, 1,1,1, 0, 0,0,0.8);
   //Green car
   car(1,0,0.8, 1,1,1, 180, 0,0.5,0);
   //Teal car
   car(1.2,0,-2, 1,1,1, 220, 0,0.8,0.8);

   //Parking surface
   glColor3f(0.4, 0.4, 0.4);
   cube(0,-0.24,0, 4,0.1,4, 0, 0);

   //World edges
   glColor3f(0, 0.3, 0.1);
   cube(4.2,-0.24,0, 0.2,0.2,4.4, 0, 0);
   cube(-4.2,-0.24,0, 0.2,0.2,4.4, 0, 0);
   glColor3f(0, 0.5, 0.1);
   cube(0,-0.24,4.2, 0.2,0.2,4, 90, 0);
   cube(0,-0.24,-4.2, 0.2,0.2,4, 90, 0);

   //  Draw axes
   glColor3f(1,1,1);
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
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f  FOV=%d  Projection=%d  ColorMode=%d",th,ph,dim,fov,mode,color);
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT) {
      th += 5;
   }
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT) {
      th -= 5;
   }
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP) {
      ph += 5;
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN) {
      ph -= 5;
   }
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
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
   //  Reset view angle and position
   else if (ch == '0') {
      th = ph = 0;
      fpX = fpY = 0;
   }
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M') {
      mode = (mode+1)%3;
      //Reset the vertical angle when switching to first person mode.
      if (mode == 2) ph = 0;
   }
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if (ch == 'c' || ch == 'C')
      color = 1-color;
   else if (ch == 'w' || ch == 'W') {
      fpX += fpMoveInc * refX;
      fpZ += fpMoveInc * refZ;
   }
   else if (ch == 's' || ch == 'S') {
      fpX -= fpMoveInc * refX;
      fpZ -= fpMoveInc * refZ;
   }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Connor Guerin - Scene With Projections");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
