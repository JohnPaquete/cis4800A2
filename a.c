
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Linux OpenGL Headers
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

// MacOS OpenGL Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


//#define STEPU (2.0*M_PI/50.0)
//#define STEPV (M_PI/50.0)
	/* flags used to control the appearance of the image */
int lineDrawing = 0;	// draw polygons as solid or lines
int lighting = 1;	// use diffuse and specular lighting
int drawNormals = 0;	// draw normals on object
int heightmap = 0;	// use heightmap to move vertices when == 1
int drawDots = 0;	// draw only vertices when == 1
int smoothShade = 1;	// use normal vertices when ==1,surface normals when ==0 

	/* used to rotate object in update() */
float rot = 0.0;
float r = 1.0;

	/* used for height map */
int iheight, iwidth, idepth;
int image[50][50];

/*  Initialize material property and light source.  */
void init (void) {
   GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat light_specular[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat light_ambient1[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat light_diffuse1[] = { 0.4, 0.4, 0.4, 1.0 };
   GLfloat light_specular1[] = { 0.9, 0.9, 0.9, 1.0 };
   GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
   GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

   GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
   GLfloat light_position2[] = { -5.0, -5.0, -5.0, 0.0 };

	/* if lighting is turned on then use ambient, diffuse and specular
	   lights, otherwise use ambient lighting only */
   if (lighting == 1) {
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_on);

      glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient1);
      glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
      glLightfv (GL_LIGHT1, GL_SPECULAR, light_full_on);
   } else {
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_off);
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_full_on);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_full_off);
      glLightfv (GL_LIGHT1, GL_SPECULAR, light_full_off);
      glLightfv (GL_LIGHT1, GL_AMBIENT, light_full_on);
      glLightfv (GL_LIGHT1, GL_DIFFUSE, light_full_off);
   }
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);
   glLightfv (GL_LIGHT1, GL_POSITION, light_position2);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable (GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);

   glLineWidth(2.0);
}


void display (void)
{
GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};
GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
GLfloat darkred[]   = {0.3, 0.0, 0.0, 1.0};
GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gray[] = {0.8, 0.8, 0.8, 1.0};
GLfloat darkgray[] = {0.3, 0.3, 0.3, 1.0};


   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw surfaces as smooth shaded */
	/* do not change this to GL_FLAT, use normals for flat shading */
   glShadeModel(GL_SMOOTH);

	/* draw polygons as either solid or outlines */
   if (lineDrawing == 1)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else 
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* give all objects the same shininess value */
   glMaterialf(GL_FRONT, GL_SHININESS, 128);

	/* set starting location of objects */
   glPushMatrix ();
   glTranslatef(0.0, 0.0, -7.0);
	/* rotate around the y axis, angle or rotation (rot) modified in
	   the update() function */
   glRotatef (rot, 0.0, 1.0, 0.0);

	/* set polygon colour */
   glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	/* set point size so vertices are visible */
   glPointSize(5.0);

	/* move to location for object then draw it */

   float STEPU = (2.0*M_PI/50.0);
   float STEPV = (M_PI/50.0);
   /* Drawing sphere */
   for (int i = 0; i < 50; i++)
   {
      for (int j = 0; j< 50; j++)
      {

         float u = 0.0 + (i * STEPU);
         float v = (-M_PI/2.0) + (j * STEPV);

         /* Calculating vertexes of quad */
         float x1 = r * cos(v) * cos(u);
         float y1 = r * cos(v) * sin(u);
         float z1 = r * sin(v);

         float x2 = r * cos(v) * cos(u + STEPU);
         float y2 = r * cos(v) * sin(u + STEPU);
         float z2 = r * sin(v);

         float x3 = r * cos(v + STEPV) * cos(u + STEPU);
         float y3 = r * cos(v + STEPV) * sin(u + STEPU);
         float z3 = r * sin(v + STEPV);

         float x4 = r * cos(v + STEPV) * cos(u);
         float y4 = r * cos(v + STEPV) * sin(u);
         float z4 = r * sin(v + STEPV);

         /* Storing normals */
         float nx1 = x1;
         float ny1 = y1;
         float nz1 = z1;

         float nx2 = x2;
         float ny2 = y2;
         float nz2 = z2;

         float nx3 = x3;
         float ny3 = y3;
         float nz3 = z3;

         float nx4 = x4;
         float ny4 = y4;
         float nz4 = z4;

         /* Transform quad acording to height map */
         if (heightmap == 1)
         {
            x1 *= (1.0 + (image[i][j]/255.0));
            y1 *= (1.0 + (image[i][j]/255.0));
            z1 *= (1.0 + (image[i][j]/255.0));

            x4 *= (1.0 + (image[i][(j+1)%50]/255.0));
            y4 *= (1.0 + (image[i][(j+1)%50]/255.0));
            z4 *= (1.0 + (image[i][(j+1)%50]/255.0));

            x3 *= (1.0 + (image[(i+1)%50][(j+1)%50]/255.0));
            y3 *= (1.0 + (image[(i+1)%50][(j+1)%50]/255.0));
            z3 *= (1.0 + (image[(i+1)%50][(j+1)%50]/255.0));

            x2 *= (1.0 + (image[(i+1)%50][j]/255.0));
            y2 *= (1.0 + (image[(i+1)%50][j]/255.0));
            z2 *= (1.0 + (image[(i+1)%50][j]/255.0));
         }

         

         /* Drawing points */
         if (drawDots == 1)
         {
            glBegin(GL_POINTS);
               glVertex3f(x1, y1, z1);
            glEnd();
         } 
         else
         {
            /* Drawing with hard shading */
            if (smoothShade == 0)
            {
               glBegin(GL_QUADS);
                  glNormal3f(nx1, ny1, nz1);
                  glVertex3f(x1, y1, z1);
                  glVertex3f(x2, y2, z2);
                  glVertex3f(x3, y3, z3);
                  glVertex3f(x4, y4, z4);
               glEnd();
            }
            else if (smoothShade == 1)
            {
               /* Drawing with smooth shading */
               glBegin(GL_QUADS);
                  glNormal3f(nx1, ny1, nz1);
                  glVertex3f(x1, y1, z1);
                  glNormal3f(nx2, ny2, nz2);
                  glVertex3f(x2, y2, z2);
                  glNormal3f(nx3, ny3, nz3);
                  glVertex3f(x3, y3, z3);
                  glNormal3f(nx4, ny4, nz4);
                  glVertex3f(x4, y4, z4);
               glEnd();
            }
         }
         /* Drawing normals */
         if (drawNormals == 1)
         {
            /* Switching material */
            glMaterialfv(GL_FRONT, GL_AMBIENT, green);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
            /* Drawing normal */
            glBegin(GL_LINES);
               glVertex3f(x1, y1, z1);
               glVertex3f(x1 * 1.1, y1 * 1.1, z1 * 1.1);
            glEnd();
            /* Switching material */
            glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
         }

      }
   }

   glPopMatrix ();
   glFlush ();
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 10.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:
      case 'q':
         exit(0);
         break;
      case '1':		// draw polygons as outlines
         lineDrawing = 1;
         lighting = 0;
         init();
         //display();
         break;
      case '2':		// draw polygons as filled but not shaded (ambient only)
         lineDrawing = 0;
         lighting = 0;
         init();
         //display();
         break;
      case '3':		// diffuse and specular lighting, smooth shading
         lineDrawing = 0;
         lighting = 1;
         init();
         //display();
         break;
      case '4':		// draw vertices only, no polygons when ==1
         if (drawDots == 0) drawDots = 1;
         else drawDots = 0;
         break;
      case '5':		// flat shade, use only one normal
         if (smoothShade == 0) smoothShade = 1;
         else smoothShade = 0;
         break;
      case '6':		// draw normals to points when ==1
         if (drawNormals == 0) drawNormals = 1;
	 else drawNormals = 0;
         break;
      case '7':		// add height map to sphere when ==1
         if (heightmap == 0) heightmap = 1;
         else heightmap = 0;
         break;
   }
}

void update() {
   rot += 0.0005;
   display();
}


/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv) {

   if (argc != 2)
   {
      printf("No heightmap given.\n");
      return 1;
   }

   FILE* fp = fopen(argv[1], "r");

   if (fp == NULL)
   {
      printf("Failed to open file: %s\n", argv[1]);
      return 1;
   }

   fscanf(fp, "%*[^\n]\n");
   fscanf(fp, "%*[^\n]\n");
   fscanf(fp, "%*[^\n]\n");
   fscanf(fp, "%*[^\n]\n");
   for (int i = 0; i < 50; i++)
   {
      for (int j = 0; j < 50; j++)
      {
         fscanf(fp, "%d", &image[i][j]);
      }
   }
   fclose(fp);
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (1024, 768);
   glutCreateWindow (argv[0]);
   init();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutIdleFunc(update);
   glutMainLoop();
   return 0; 
}

