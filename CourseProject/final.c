/*
 *  Final Course Project
 *  Connor Guerin
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  w/a/s/d    Move the camera (in first person mode)
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles first person/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out (in perspective view)
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=0;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1.333;     //  Aspect ratio
double dim=8.0;   //  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   30;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 13;  // Elevation of light

double fpMoveInc = 0.02; //Multiplier for how much to move each keystroke in FP mode

//First person camera location
double fpX = 0;
double fpY = 0.7;
double fpZ = 0;

//x, y, z for refrence point in glLookAt() for FP mode
double refX = 5;
double refY = 0;
double refZ = 0;

//Texture Variables
int tMode = 0;
float texScale = 0.5;

//Light Vecotrs
float Ambient[]   = {0.01*80 ,0.01*80 ,0.01*80 ,1.0};
float Diffuse[]   = {1.0,1.0,1.0,1.0};
float Specular[]  = {0.01*0,0.01*0,0.01*0,1.0};

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
   // float white[] = {1,1,1,1};
   // float black[] = {0,0,0,1};
   // glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
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

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("basic-metal.bmp"));
   texScale = 0.1;

   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);

   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);

   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);

   //  End
   glEnd();

   glDisable(GL_POLYGON_OFFSET_FILL);
   //Color and texture to add windows
   if(w == 1) {
      glColor3f(0.8, 0.8, 1);
      glBindTexture(GL_TEXTURE_2D,LoadTexBMP("glass.bmp"));
      texRepX = 1.0;
      texRepY = 1.0;
      glBegin(GL_QUADS);
      //  Front
      glNormal3f(0, 0, 1);
      glTexCoord2f(0.0,0.0); glVertex3f(-0.8,-1, 1);
      glTexCoord2f(texRepX,0.0); glVertex3f(+0.8,-1, 1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(+0.8,+1, 1);
      glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,+1, 1);
      //  Back
      glNormal3f(0, 0,-1);
      glTexCoord2f(0.0,0.0); glVertex3f(+0.8,-1,-1);
      glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,-1,-1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,+1,-1);
      glTexCoord2f(0.0,texRepY); glVertex3f(+0.8,+1,-1);
      glEnd();
   }
   glEnable(GL_POLYGON_OFFSET_FILL);
   
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

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("car-wheel.bmp"));

   glColor3f(0.8,0.8,0.8);
   //Tire
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, 0.05);
   for (th=360;th>=0;th-=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("tire-tread.bmp"));

   glColor3f(0.5,0.5,0.55);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glNormal3f(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0);
      glTexCoord2f(0,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
      glTexCoord2f(1,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0.05);
   }
   // glNormal3f(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), -0.05);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0.05);
   glEnd();

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

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("basic-metal.bmp"));
   texScale = 0.1;

   //Offset the bumper so that the lights and grill are drawn directly on the surface
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Bumper

   //Base
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_POLYGON);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0,0,0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(0,0,-0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1,0,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1,0,0.35);
   glEnd();

   //Front Panels
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, 0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0, 0.4);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, -0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0, 0, -0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, -0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0.2, -0.35);
   
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0.894427, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0.25, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, 0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.25, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, -0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.25, -0.35);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.2, -0.4);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   if (m == 1) {
      glColor3f(0.2,0.2,0.2);
      glBindTexture(GL_TEXTURE_2D,LoadTexBMP("car-grill.bmp"));

      //Grill
      glBegin(GL_QUADS);
      glNormal3f(1, 0, 0);
      glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.15, 0.18);
      glTexCoord2f(0.5,0.0); glVertex3f(0.1, 0.03, 0.18);
      glTexCoord2f(0.5,1.0); glVertex3f(0.1, 0.03, -0.18);
      glTexCoord2f(0.0,1.0); glVertex3f(0.1, 0.15, -0.18);
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

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("headlamp.bmp"));

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glEnable(GL_POLYGON_OFFSET_FILL);

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();  
}

static void policeCar(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   float cr = 0.1;
   float cb = 0.1;
   float cg = 0.1;

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
   glColor3f(cr, cb, cg);
   body(0,0.3,0, 0.2,0.1,0.35, 0, 1);
   glColor3f(cr, cb, cg);
   body(-0.4,0.3,0, 0.2,0.1,0.35, 0, 1);

   texScale = 1.0;

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(cr, cb, cg);

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("basic-metal.bmp"));

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //Hood and upper side pannels
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.2, 0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.2, -0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.2, -0.4);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("glass.bmp"));

   glColor3f(0.8, 0.8, 1);

   //Windshield
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.2,0.4,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2,0.4,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.2,0.4,-0.35);
   glEnd();

   //Rear Window
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.25,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.25,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.6,0.4,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.6,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.25,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.6,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.4,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.6,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.8,0.25,-0.35);
   glEnd();

   int t = glutGet(GLUT_ELAPSED_TIME)/1000.0;

   //Light bar

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("glass.bmp"));
   //Mid section
   glColor3f(0.1, 0.1, 0.1);
   cube(-0.2,0.42,0, 0.07,0.02,0.05, 0);

   //  Enable light 1 - Police Light
   glEnable(GL_LIGHT1);
   
   if (t % 2 == 0) {
      float dif[4] = {0.8,0,0,1};
      float spec[4] = {0,0,0,1};
      float pos[4] = {-0.2,0.26,-0.15,1.0};
      //Red Light
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT0,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT0,GL_POSITION,pos);

      //Red Light
      float redEm[4] = {0.8, 0, 0, 1.0};
      glMaterialf(GL_FRONT,GL_SHININESS,0);
      glMaterialfv(GL_FRONT,GL_SPECULAR,redEm);
      glMaterialfv(GL_FRONT,GL_EMISSION,redEm);
      glColor3f(0.5, 0, 0);
      cube(-0.2,0.42,-0.15, 0.07,0.02,0.1, 0);
      //Blue Light
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
      glColor3f(0, 0, 0.5);
      cube(-0.2,0.42,0.15, 0.07,0.02,0.1, 0);
   } else {
      float dif[4] = {0,0,0.8,1};
      float spec[4] = {0,0,0,1};
      float pos[4] = {-0.2,0.26,0.15,1.0};
      //Blue Light
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT0,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT0,GL_POSITION,pos);

      //Red Light
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
      glColor3f(0.5, 0, 0);
      cube(-0.2,0.42,-0.15, 0.07,0.02,0.1, 0);
      //Blue Light
      float blueEm[4] = {0, 0, 0.8, 1.0};
      glMaterialf(GL_FRONT,GL_SHININESS,0);
      glMaterialfv(GL_FRONT,GL_SPECULAR,blueEm);
      glMaterialfv(GL_FRONT,GL_EMISSION,blueEm);
      glColor3f(0, 0, 0.5);
      cube(-0.2,0.42,0.15, 0.07,0.02,0.1, 0);
   }

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

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

   texScale = 1.0;

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(cr, cb, cg);

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("basic-metal.bmp"));

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //Hood and upper side pannels
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.2, 0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.2, -0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.2, -0.4);
   glEnd();

   //Trunk
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.6, 0.25, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.6, 0.25, -0.35);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("glass.bmp"));

   glColor3f(0.8, 0.8, 1);

   //Windshield
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.2,0.4,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2,0.4,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.2,0.4,-0.35);
   glEnd();

   //Rear Window
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.4,0.4,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.4,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.4,0.4,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.6,0.25,-0.35);
   glEnd();

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("carbon-fiber.bmp"));

   //Spoiler
   glColor3f(0.3,0.3,0.3);
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0);

   texRepX = 5.0;
   texRepY = 1.0;

   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.7,0.31,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,-0.35);
   
   glNormal3f(0.196116, 0.980581, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.7,0.31,-0.35);

   texRepX = 5.0;
   texRepY = 0.2;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,0.35);
   
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("basic-metal.bmp"));
   glColor3f(cr, cb, cg);

   //Spoiler Fins
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   //Duplicate to draw both sides (with inverted normals) when face culling is on
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void workshop(double x, double z, double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,0,z);
   glRotated(th,0,1,0);

   //Building - Brown Workshop
   // glColor3f(0.7, 0.7, 0.7);
   // glBindTexture(GL_TEXTURE_2D,LoadTexBMP("brown-brick.bmp"));
   texScale = 0.5;
   cube(0,1.6,0, 2,0.4,1, 0); //Top
   cube(-1.75,0.65,0, 0.25,0.55,1, 0); //Left
   cube(1.75,0.65,0, 0.25,0.55,1, 0); //Right
   cube(0,0.65,0, 0.2,0.55,1, 0); //Middle

   //"Floor" for brown workshop
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("sidewalk.bmp"));
   texScale = 1;
   cube(0,-0.05,0.9, 2,0.15,0.1, 0);
   
   //Garage Doors
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("garage-door.bmp"));
   glBegin(GL_QUADS);
   float texRepX = 1.0;
   float texRepY = 1.0;
   //Left
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1.5, 0.1, 0.9);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.2, 0.1, 0.9);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.2, 1.2, 0.9);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1.5, 1.2, 0.9);
   //Right
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2, 0.1, 0.9);
   glTexCoord2f(texRepX,0.0); glVertex3f(1.5, 0.1, 0.9);
   glTexCoord2f(texRepX,texRepY); glVertex3f(1.5, 1.2, 0.9);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2, 1.2, 0.9);
   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void greyHouse(double x, double z, double th) {
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,0,z);
   glRotated(th,0,1,0);

   glPolygonOffset(1,1);
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Walkway (to house)
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("walkway.bmp"));
   texScale = 0.4;
   cube(0,-0.05,-1.5, 0.5,0.15,0.4, 90); // Between hedges

   //Hedges
   glColor3f(0, 0.3, 0.1);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("hedge.bmp"));
   texScale = 0.25;
   cube(1.23,0.3,-1.2, 0.17,0.2,0.77, 90);
   cube(-1.23,0.3,-1.2, 0.17,0.2,0.77, 90);

   //Grass
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("grass.bmp"));
   texScale = 0.5;
   cube(-1.2,-0.05,-1.5, 0.8,0.15,0.5, 0);
   cube(1.2,-0.05,-1.5, 0.8,0.15,0.5, 0);

   //Building - Grey House
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("grey-brick.bmp"));
   texScale = 0.5;
   cube(0,0.9,-3, 2,0.8,1, 0);

   //Door Frame
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("wood-beam.bmp"));
   texScale = 0.5;
   cube(-0.4,0.65,-1.95, 0.1,0.55,0.05, 0);
   cube(0.4,0.65,-1.95, 0.1,0.55,0.05, 0);
   cube(0,1.25,-1.95, 0.5,0.05,0.05, 0);

   //Window Sills
   cube(-1.3,0.65,-1.95, 0.45,0.05,0.05, 0); //Left
   cube(1.3,0.65,-1.95, 0.45,0.05,0.05, 0); //Right

   glDisable(GL_POLYGON_OFFSET_FILL);

   //Door
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("front-door-1.bmp"));
   glBegin(GL_QUADS);
   double texRepX = 1.0;
   double texRepY = 1.0;
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.3, 0.2, -2);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.3, 0.2, -2);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.3, 1.2, -2);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.3, 1.2, -2);
   glEnd();

   //Windows
   glColor3f(0.7, 0.7, 0.7);

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("window-1.bmp"));
   glBegin(GL_QUADS);
   texRepX = 1.0;
   texRepY = 2.0;

   //Left Window
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1.7, 0.7, -2);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.9, 0.7, -2);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.9, 1.3, -2);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1.7, 1.3, -2);

   //Right Window
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.9, 0.7, -2);
   glTexCoord2f(texRepX,0.0); glVertex3f(1.7, 0.7, -2);
   glTexCoord2f(texRepX,texRepY); glVertex3f(1.7, 1.3, -2);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.9, 1.3, -2);

   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void skybox(float dim) {
   glDisable(GL_POLYGON_OFFSET_FILL);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("skybox-front.bmp"));
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, -1);
   glTexCoord2f(1.0,0.0); glVertex3f(+dim,-dim, dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, dim);
   glTexCoord2f(1.0,1.0); glVertex3f(+dim,+dim, dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("skybox-back.bmp"));
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, 1);
   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, -dim);
   glTexCoord2f(0.0,0.0); glVertex3f(+dim,-dim, -dim);
   glTexCoord2f(0.0,1.0); glVertex3f(+dim,+dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, -dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("skybox-right.bmp"));
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(dim,-dim, -dim);
   glTexCoord2f(0.0,0.0); glVertex3f(dim,-dim, +dim);
   glTexCoord2f(0.0,1.0); glVertex3f(dim,+dim, +dim);
   glTexCoord2f(1.0,1.0); glVertex3f(dim,+dim, -dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("skybox-left.bmp"));
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(1, 0, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, +dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, -dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, +dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("skybox-top.bmp"));
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(+dim,dim, +dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,dim, +dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(+dim,dim, -dim);
   glEnd();
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
   //Enable Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , tMode?GL_REPLACE:GL_MODULATE);

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
   //  First Person
   else
   {
      refX = (dim * Sin(th)) + fpX;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + fpZ;
      gluLookAt(fpX,fpY,fpZ, refX,refY,refZ, 0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light) {
      //  Light position
      float Position[]  = {distance*Cos(zh),distance*Sin(zh),0,1.0};
      // float MoonPosition[]  = {-(distance*Cos(zh)),-(distance*Sin(zh)),0,1.0};

      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.6); //Sun
      // ball(MoonPosition[0],MoonPosition[1],MoonPosition[2], 0.3); //Moon

      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

      //  Enable light 0 - Sun
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0 (Sun)
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else {
      glDisable(GL_LIGHTING);
   }

   //Inital values for texture repitition
   double texRepX = 1.0;
   double texRepY = 1.0;

   //Draw scene

   //Skybox
   skybox(64);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Enable offset for windows, doors, and other decals
   glPolygonOffset(1,1);
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Police Car
   policeCar(3,0.12,1.2, 1,1,1, 30);

   // //Blue car
   // car(-1,0.12,0.8, 1,1,1, 0, 0,0,0.8);

   // //Red car
   // car(3,0.12,0.8, 1,1,1, 0, 0.8,0,0);

   //Street surface - Main Street
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("asphalt.bmp"));
   texScale = 0.5;
   float xPos = -6;
   for(int i = 0; i < 7; i++){
      cube(xPos,-0.1,0.75, 1,0.1,0.75, 0);
      cube(xPos,-0.1,2.25, 1,0.1,0.75, 0);
      xPos += 2;
   }

   //Street Surface - Side Streets
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("asphalt.bmp"));
   texScale = 0.5;
   float yPos = -3.5;
   for(int i = 0; i < 6; i++){
      cube(-7.75,-0.1,yPos, 1,0.1,0.75, 90);
      cube(-9.25,-0.1,yPos, 1,0.1,0.75, 90);
      cube(7.75,-0.1,yPos, 1,0.1,0.75, 90);
      cube(9.25,-0.1,yPos, 1,0.1,0.75, 90);
      yPos += 2;
   }

   //Sidewalks (Player Side)
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("sidewalk.bmp"));
   texScale = 0.5;
   cube(0,-0.05,-0.5, 2,0.15,0.5, 0); // Along Street
   cube(-4,-0.05,-0.5, 2,0.15,0.5, 0); // Along Street
   cube(4,-0.05,-0.5, 2,0.15,0.5, 0); // Along Street

   //Sidewalks (Far Side)
   cube(0,-0.05,3.5, 2,0.15,0.5, 0); // Along Street
   cube(-4,-0.05,3.5, 2,0.15,0.5, 0); // Along Street
   cube(4,-0.05,3.5, 2,0.15,0.5, 0); // Along Street

   //Sidewalks (Ends)
   //Near
   cube(-6.5,-0.05,5, 2,0.15,0.5, 90); // Far side
   cube(6.5,-0.05,5, 2,0.15,0.5, 90); // Far side
   cube(-6.5,-0.05,-2, 2,0.15,0.5, 90); // Player side
   cube(6.5,-0.05,-2, 2,0.15,0.5, 90); // Player side
   //Far
   cube(-10.5,-0.05,5, 2,0.15,0.5, 90); // Far side
   cube(10.5,-0.05,5, 2,0.15,0.5, 90); // Far side

   cube(-10.5,-0.05,1.5, 1.5,0.15,0.5, 90); // Middle
   cube(10.5,-0.05,1.5, 1.5,0.15,0.5, 90); // Middle

   cube(-10.5,-0.05,-2, 2,0.15,0.5, 90); // Player side
   cube(10.5,-0.05,-2, 2,0.15,0.5, 90); // Player side


   //Grey town house - player side
   greyHouse(0,0,0);

   //Grey town house - Far
   greyHouse(10,-2.05,-90); //Left
   greyHouse(10,2.05,-90); //Middle
   greyHouse(10,6.15,-90); //Right

   //Fence
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("wood-fence.bmp"));
   texScale = 0.5;
   cube(13,0.6,0, 0.05,0.5,2, 90); //Left
   cube(13,0.6,4.1, 0.05,0.5,2, 90); //Left

   //Brown workshop - player side
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("brown-brick.bmp"));
   workshop(-4,-2,0);

   //Storage Facility
   glColor3f(0.33, 0.22, 0.11);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("white-brick.bmp"));
   workshop(4,5,180);
   glColor3f(0.33, 0.22, 0.11);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("white-brick.bmp"));
   workshop(0,5,180);

   //Triagular Roof
   texRepX = 2/texScale;
   texRepY = 0.7/texScale;
   float xVal = 6;
   for(int i = 0; i < 4; i++){
      texRepX = 2/texScale;
      texRepY = 0.7/texScale;
      glColor3f(0.33, 0.22, 0.11);
      //Side
      glBindTexture(GL_TEXTURE_2D,LoadTexBMP("white-brick.bmp"));
      glBegin(GL_TRIANGLES);
      glNormal3f(0,0,-1);
      glTexCoord2f(0.0,0.0); glVertex3f(xVal,2,4);
      glTexCoord2f(texRepX, 0.0); glVertex3f(xVal-2,2,4);
      glTexCoord2f(texRepX, texRepY); glVertex3f(xVal-2,2.7,4);
      glEnd();

      texRepX = 4/texScale;
      texRepY = 2/texScale;
      glColor3f(0.5, 0.5, 0.5);
      //Roof
      glBindTexture(GL_TEXTURE_2D,LoadTexBMP("metal-roof.bmp"));
      glBegin(GL_QUADS);
      glNormal3f(-0.33035, 0.943858, 0);
      glTexCoord2f(0.0,0.0); glVertex3f(xVal,2,4);
      glTexCoord2f(texRepX,0.0); glVertex3f(xVal-2,2.7,4);
      glTexCoord2f(texRepX,texRepY); glVertex3f(xVal-2,2.7,6);
      glTexCoord2f(0.0,texRepY); glVertex3f(xVal,2,6);
      glEnd();

      texRepX = 1;
      texRepY = 3;
      glColor3f(0.5, 0.5, 0.5);
      //Windows
      glBindTexture(GL_TEXTURE_2D,LoadTexBMP("warehouse-window.bmp"));
      glBegin(GL_QUADS);
      glNormal3f(-1, 0, 0);
      glTexCoord2f(texRepX,0.0); glVertex3f(xVal-2,2.7,4);
      glTexCoord2f(0.0,0.0); glVertex3f(xVal-2,2,4);
      glTexCoord2f(0.0,texRepY); glVertex3f(xVal-2,2,6);
      glTexCoord2f(texRepX,texRepY); glVertex3f(xVal-2,2.7,6);
      glEnd();

      xVal -= 2;
   }
   


   //Grass Square
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("grass.bmp"));
   texScale = 0.5;
   cube(4,-0.05,-3, 2,0.15,2, 0);

   //Fence
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("wood-fence.bmp"));
   texScale = 0.5;
   cube(2.05,0.6,-3, 0.05,0.5,2, 0); //Left
   cube(5.95,0.6,-3, 0.05,0.5,2, 0); //Right
   cube(2.8,0.6,-1.05, 0.05,0.5,0.7, 90); //Front Left
   cube(5.2,0.6,-1.05, 0.05,0.5,0.7, 90); //Front Right

   //Conder block wall - Far Side
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,LoadTexBMP("cinder-block.bmp"));
   texScale = 0.5;
   cube(-4,0.6,4.05, 2,0.5,0.05, 0); //Left

   


   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   //  Switch off textures so it doesn't apply to the rest
   glDisable(GL_TEXTURE_2D);

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
     th,ph,dim,fov,mode?"Perpective":"FP",light?"On":"Off");
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

static void setEnvLighting()
{
   float ambScale = 0.8;

   if(Sin(zh) >= 0.2) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = (255 / 255) * ambScale;
      Ambient[2] = (255 / 255) * ambScale;

      Diffuse[0] = 1.0;
      Diffuse[1] = 1.0;
      Diffuse[2] = 1.0;
   } else if (Sin(zh) < 0.4 && Sin(zh) >= 0) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = ((60 + (195*Sin(zh)*2.5)) / 255) * ambScale;
      Ambient[2] = ((60 + (195*Sin(zh)*2.5)) / 255) * ambScale;

      Diffuse[0] = Sin(zh)*2.5;
      Diffuse[1] = Sin(zh)*2.5;
      Diffuse[2] = Sin(zh)*2.5;
   } else if (Sin(zh) < 0 && Sin(zh) >= -0.4){
      Ambient[0] = ((255 - (235*Sin(zh)*-2.5)) / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = ((60 + (120*Sin(zh)*-2.5)) / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   } else {
      Ambient[0] = (20 / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = (180 / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   }
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(15*t,360.0);

   setEnvLighting();

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
   Project(fov,asp,dim);
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
   //First Person Controls
   // else if (ch == 'w' || ch == 'W') {
   //    fpX += fpMoveInc * refX;
   //    fpZ += fpMoveInc * refZ;
   // }
   // else if (ch == 's' || ch == 'S') {
   //    fpX -= fpMoveInc * refX;
   //    fpZ -= fpMoveInc * refZ;
   // }
   // else if (ch == 'd' || ch == 'D') {
   //    fpX += fpMoveInc * -refZ;
   //    fpZ += fpMoveInc * refX;
   // }
   // else if (ch == 'a' || ch == 'A') {
   //    fpX += fpMoveInc * refZ;
   //    fpZ += fpMoveInc * -refX;
   // }
   // Ambient level
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
   Project(fov,asp,dim);
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
   Project(fov,asp,dim);
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
   glutInitWindowSize(800,600);
   glutCreateWindow("Connor Guerin - Course Project");
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
