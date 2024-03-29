/*
 *  HW5 - Lighting
 *  Connor Guerin
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=6.0;   //  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  10;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 1.5;  // Elevation of light

//x, y, z for refrence point in glLookAt() for FP mode
double refX = 5;
double refY = 0;
double refZ = 0;

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
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f( 0, 0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0,-one, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a section of the car body
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     w (1 to color windows for car body, 0 otherwise)
 */
static void body(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int w)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Top
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0,-1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);

   //Color to add windows
   if(w == 1) glColor3f(0.8, 0.8, 1);
   //  Front
   glNormal3f(0,0,1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f(0,0,-1);
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
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   //Turn off shininess for the rubber tire
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Offset the tire so that the rim is in front of it
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(0.1,0.1,0.1);
   //Wheel
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, -1);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, 1);
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
      glNormal3f(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0);
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0.05);
   }
   glNormal3f(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0);
   glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), -0.05);
   glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0.05);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   //Rim
   //Set the shininess high for the metal rim
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);

   d = d * 0.7;
   glColor3f(0.1,0.1,0.1);
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, -1);
   glVertex3f(0, 0, -0.05);
   int interval = 0;
   for (th=0;th<=360;th+=s)
   {
      if (interval % 4 != 0) glColor3f(0.1,0.1,0.1); else glColor3f(0.8,0.8,0.8);
      interval++;
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glColor3f(0.1,0.1,0.1);
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, 1);
   glVertex3f(0, 0, 0.05);
   interval = 0;
   for (th=360;th>=0;th-=s)
   {
      if (interval % 4 != 0) glColor3f(0.1,0.1,0.1); else glColor3f(0.8,0.8,0.8);
      interval++;
      double ph = d-90;
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Undo transformations
   glPopMatrix();
}

static void bumper(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int m)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Offset the bumper so that the lights and grill are drawn directly on the surface
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Bumper

   //Base
   glBegin(GL_POLYGON);
   glNormal3f(0, -1, 0);
   glVertex3f(0,0,0.4);
   glVertex3f(0,0,-0.4);
   glVertex3f(0.1,0,-0.35);
   glVertex3f(0.1,0,0.35);
   glEnd();

   //Front Panels
   glBegin(GL_QUADS);
   glNormal3f(0.447214, 0, 0.894427);
   glVertex3f(0.1, 0, 0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0, 0.2, 0.4);
   glVertex3f(0, 0, 0.4);

   glNormal3f(1, 0, 0);
   glVertex3f(0.1, 0, -0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0.1, 0, 0.35);

   glNormal3f(0.447214, 0, -0.894427);
   glVertex3f(0.1, 0, -0.35);
   glVertex3f(0, 0, -0.4);
   glVertex3f(0, 0.2, -0.4);
   glVertex3f(0.1, 0.2, -0.35);
   
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   glNormal3f(0.447214, 0.894427, 0);
   glVertex3f(0, 0.25, 0.35);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0, 0.25, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0.333333, 0.666667, 0.666667);
   glVertex3f(0, 0.2, 0.4);
   glVertex3f(0.1, 0.2, 0.35);
   glVertex3f(0, 0.25, 0.35);

   glNormal3f(0.333333, 0.666667, -0.666667);
   glVertex3f(0, 0.25, -0.35);
   glVertex3f(0.1, 0.2, -0.35);
   glVertex3f(0, 0.2, -0.4);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   if (m == 1) {
      glColor3f(0.1,0.1,0.1);
      //Grill
      glBegin(GL_QUADS);
      glNormal3f(1, 0, 0);
      glVertex3f(0.1, 0.15, 0.18);
      glVertex3f(0.1, 0.05, 0.18);
      glVertex3f(0.1, 0.05, -0.18);
      glVertex3f(0.1, 0.15, -0.18);
      glEnd();
   }

   //Lights (taillights or headlights)
   float emColor[4];
   if(m == 1) {
      emColor[0] = 1.0 * emission;
      emColor[1] = 1.0 * emission;
      emColor[2] = 0.8 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(1, 1, 0.8);
   } else {
      emColor[0] = 0.8 * emission;
      emColor[1] = 0.0 * emission;
      emColor[2] = 0.0 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(.8, 0, 0);
   }

   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,emColor);
   glMaterialfv(GL_FRONT,GL_EMISSION,emColor);

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glVertex3f(0.1, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glVertex3f(0.1, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glEnable(GL_POLYGON_OFFSET_FILL);

   //Undo transformations
   glPopMatrix();  
}

static void car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 float cr, float cb, float cg)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glPolygonOffset(1,1);

   wheel(0.6,0,0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,0.4, 1,1,1, 0, 8, 10);

   glColor3f(cr, cb, cg);

   //Lower Body
   body(0,0.1,0, 0.8,0.1,0.4, 0, 0);
   //Cabin
   body(-0.1,0.3,0, 0.3,0.1,0.35, 0, 1);

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(cr, cb, cg);

   //Hood
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glVertex3f(-0.8, 0.25, 0.35);
   glVertex3f(-0.8, 0.2, 0.4);
   glVertex3f(0.8, 0.2, 0.4);
   glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glVertex3f(0.4, 0.25, 0.35);
   glVertex3f(0.8, 0.25, 0.35);
   glVertex3f(0.8, 0.25, -0.35);
   glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glVertex3f(-0.8, 0.2, -0.4);
   glVertex3f(-0.8, 0.25, -0.35);
   glVertex3f(0.8, 0.25, -0.35);
   glVertex3f(0.8, 0.2, -0.4);
   
   glEnd();

   //Windshield
   glColor3f(0.8, 0.8, 1);
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glVertex3f(0.4,0.25,0.35);
   glVertex3f(0.4,0.25,-0.35);
   glVertex3f(0.2,0.4,-0.35);
   glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glVertex3f(0.2,0.4,0.35);
   glVertex3f(0.2,0.25,0.35);
   glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glVertex3f(0.4,0.25,-0.35);
   glVertex3f(0.2,0.25,-0.35);
   glVertex3f(0.2,0.4,-0.35);
   
   glEnd();

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);
   glColor3f(cr, cb, cg);

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Trunk
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glVertex3f(-0.8, 0.25, -0.35);
   glVertex3f(-0.8, 0.25, 0.35);
   glVertex3f(-0.6, 0.25, 0.35);
   glVertex3f(-0.6, 0.25, -0.35);
   glEnd();

   //Rear Window
   glColor3f(0.8, 0.8, 1);
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glVertex3f(-0.6,0.25,-0.35);
   glVertex3f(-0.6,0.25,0.35);
   glVertex3f(-0.4,0.4,0.35);
   glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glVertex3f(-0.6,0.25,0.35);
   glVertex3f(-0.4,0.25,0.35);
   glVertex3f(-0.4,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glVertex3f(-0.4,0.4,-0.35);
   glVertex3f(-0.4,0.25,-0.35);
   glVertex3f(-0.6,0.25,-0.35);
   
   glEnd();

   //Spoiler
   glColor3f(0.1,0.1,0.1);
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0);

   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glVertex3f(-0.7,0.31,-0.35);
   glVertex3f(-0.7,0.31,0.35);
   glVertex3f(-0.8,0.31,0.35);
   glVertex3f(-0.8,0.31,-0.35);
   
   glNormal3f(0.196116, 0.980581, 0);
   glVertex3f(-0.8,0.33,-0.35);
   glVertex3f(-0.8,0.33,0.35);
   glVertex3f(-0.7,0.31,0.35);
   glVertex3f(-0.7,0.31,-0.35);

   glNormal3f(-1, 0, 0);
   glVertex3f(-0.8,0.33,0.35);
   glVertex3f(-0.8,0.33,-0.35);
   glVertex3f(-0.8,0.31,-0.35);
   glVertex3f(-0.8,0.31,0.35);
   
   glEnd();

   //Spoiler Fins
   glColor3f(cr, cb, cg);

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,-1);
   glVertex3f(-0.68,0.31,-0.35);
   glVertex3f(-0.82,0.31,-0.35);
   glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,1);
   glVertex3f(-0.82,0.35,0.35);
   glVertex3f(-0.82,0.31,0.35);
   glVertex3f(-0.68,0.31,0.35);
   
   //Duplicate to draw both sides (With inverted normals) when face culling is on
   glNormal3f(0,0,1);
   glVertex3f(-0.82,0.35,-0.35);
   glVertex3f(-0.82,0.31,-0.35);
   glVertex3f(-0.68,0.31,-0.35);
   
   glNormal3f(0,0,-1);
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
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //Enable Face Culling
   glEnable(GL_CULL_FACE);


   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Draw scene: All cars and surface

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
   cube(0,-0.24,0, 4,0.1,4, 0);

   //World edges
   glColor3f(0, 0.3, 0.1);
   cube(4.2,-0.12,0, 0.2,0.2,4, 0);
   cube(-4.2,-0.12,0, 0.2,0.2,4, 0);
   cube(0,-0.12,4.2, 0.2,0.2,4, 90);
   cube(0,-0.12,-4.2, 0.2,0.2,4, 90);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
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
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
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
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   Project(mode?fov:0,asp,dim);
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
   glutCreateWindow("Connor Guerin - HW5 Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
