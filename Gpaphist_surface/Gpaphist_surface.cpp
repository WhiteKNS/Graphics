#include "stdafx.h"

#include <glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "SOIL.lib")
#include <math.h>
#include <Windows.h>

#include "SOIL.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

GLfloat yang = 0.0;
GLfloat xang = 0.0;
double const pi = 3.1415926;

typedef unsigned int u32;
int step = 0;

void displayNothing()
{
	GLfloat colors[][3] = { { 255 / 255.f, 104 / 255.f, 0.0f },{ 255 / 255.f, 127 / 255.f, 80 / 255.f }, { 0.f, 0.f, 1.f} };
	static int back;
	back ^= 1;
	glClearColor(colors[back][0], colors[back][1], colors[back][2], 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

bool show_texture = false;
bool rotate = true;

void displayCone()
{
	int vang, ang;
	int delang = 10;
	float r0 = 0.4;
	
	float x0, y0, z0, x1, y1, z1, x2, z2, y2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (show_texture)
		glPushMatrix();

	glLoadIdentity();
	if (rotate) {
		glRotatef(yang, 1.0, 1.0, 0.0);
		glTranslatef(-0.4, 0.25, 0.0);
		glRotatef(xang, 0.0, 0.0, 1.0);
	}

	if (show_texture) {
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
	} else {
		glBegin(GL_LINES);
	}
	float height = 0.75; // vang = 100
	float radius = 0.1;

	float points = 360.0;
	float i = radius;
	
	float r=((height-i)/height)*radius;
	float theta = 2 *pi*0/points;

	x0 = (cos(theta) + theta * sin(theta)) * r;
	y0 = i;
	z0 = (sin(theta) - theta*cos(theta)) * r;

	float j = 0;
	int size;

	if (show_texture)
		size = 1;
	else
		size = height * 200;

	float prev = 0;
	float prev_delang = delang;
	for (vang = 0; vang <= size; vang += delang)
	{
		for (ang = 0; ang < 360; ang +=delang) {
			float r = ((height-i)/height)*radius;
			float v_theta = 2 *pi*(ang)/points;
			float v_ntheta = 2 *pi*(ang + delang)/points;

			x1 = (cos(v_theta) + v_theta * sin(v_theta)) * r;
			y1 = i;
			z1 = (sin(v_theta) - v_theta*cos(v_theta)) * r;
			
			x2 = (cos(v_ntheta) + v_ntheta * sin(v_ntheta)) * r;
			y2 = i;
			z2 = (sin(v_ntheta) - v_ntheta*cos(v_ntheta)) * r;

			if (!show_texture)
				glColor3f((r0 - x0) / r0, (r0 - y0) / r0, (r0 - z0) / r0);

			if (show_texture)
				glTexCoord2f(prev/360,(float)ang/360);
			glVertex3f(x1, y1, z1); // ring
	
			if (show_texture)
				glTexCoord2f((float)(prev_delang/360),(float)(ang+delang)/360);

			glVertex3f(x2, y2, z2); // ring
		
			if (show_texture)
				glTexCoord2f(prev/360,(float)(ang+delang)/360);
			glVertex3f(0, height, 0); //top

			if (show_texture)
				glTexCoord2f((float)(prev_delang/360),(float)(ang)/360);

			glVertex3f(x0, y0, z0); // strips	
			 
			prev = (float)ang;
			x0 = x1;
			z0 = z1;
			y0 = y1;

			i+= 0.001;
		}
	}

	// Back to 3D land
	if (show_texture) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);

	yang = yang + 1.2;
	if (yang>360.0)
		yang = 0.0;
	xang = xang - 0.5;
	if (xang<0.0)
		xang = 360.0;
	glutSwapBuffers();
	glFlush();

	Sleep(50);
}

void init()
{
	glClearColor(0.0, 0.0, 0.2, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
}

void KeyUp(unsigned char key, int x, int y) {
	switch (key) {
	case 't':
		show_texture = !show_texture;
		if (show_texture) {
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		} else {
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
		}
		glutPostRedisplay();
		break;
	case 'c':
		glutDisplayFunc(displayNothing);
		glutPostRedisplay();
		break;
	case 'r':
		rotate = !rotate;
		glutPostRedisplay();
		break;
	}
}

void InitGraphics(void)
{
   int width, height;
   void* pTextureImage;
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glShadeModel(GL_SMOOTH);
   // Create texture for cube; load marble texture from file and bind it
 
   pTextureImage = SOIL_load_image("chess.jpg", &width, &height, 0, SOIL_LOAD_RGB);
   GLuint tex_2d = SOIL_load_OGL_texture
    (
        "chess.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT
        );

   glBindTexture(GL_TEXTURE_2D, tex_2d);

   glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
   glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Rotation");

	InitGraphics();

	glutDisplayFunc(displayCone);
	glutIdleFunc(displayCone);

	glutKeyboardUpFunc(KeyUp);
	init();

	glutMainLoop();

	return 0;
}