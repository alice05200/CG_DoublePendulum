/*HW1-Double Pendulum Systems

<insert TA's words...>

*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/**
 * this needs to be at the top, since some headers might already includes the
 * cmath header. So to prevent including cmath without _USE_MATH_DEFINES, put it on top of everything else.
 */
#define _USE_MATH_DEFINES

#include "./GL/glut.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "Image.hpp"
#include "DoublePendulum.hpp"

//number of textures desired, you may want to change it to get bonus point
#define TEXTURE_NUM 2
//directories of image files
char* texture_name[TEXTURE_NUM] = {
	"./Resource/sun.bmp", // 0
	"./Resource/ceiling.bmp", // 1
};
//texture id array
GLuint texture[TEXTURE_NUM];

//passTime parameter for helping coordinate your animation, you may utilize it to help perform animation
#define deltaTime 10 // in ms (1e-3 second)
double passTime = 0.0;

//light sorce parameter, no need to modify
//actually, modification of these parameter is forbidden in this homework
float LightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };//Light position
float LightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Ambient Light Values
float LightDif[] = { 0.7f, 0.7f, 0.7f, 1.0f };//Diffuse Light Values
float LightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Specular Light Values

float CameraPosition[] = { 0.0f, 0.0f, -36.0f };
float CameraLookPosition[] = { 0.0f, 0.0f, 0.0f };
float CameraNormPosition[] = { 0.0f, 1.0f, 0.0f };
float CameraRotate = 90.0f;

Image *sun, *ceiling;

// The double pendulum systems
DoublePendulum* doublePendulumSystems[3];

void RenderPlanes()
{
	// Draw 5 plane or boxes, the five planes shall display like a cube excluded the front face.

	// write your code here ...
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);//ceiling texture
	glBegin(GL_QUADS);
	//前
	glTexCoord2f(0.0, 0.0);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(10.0f, 10.0f, 10.0f);
	//上
	glTexCoord2f(0.0, 0.0);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(10.0f, 10.0f, 10.0f);
	//下
	glTexCoord2f(0.0, 0.0);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(10.0f, -10.0f, 10.0f);
	//左
	glTexCoord2f(0.0, 0.0);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(10.0f, 10.0f, -10.0f);
	//右
	glTexCoord2f(0.0, 0.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// Example: Draw One Red Plane by Draw Two Triangles
	
	// set Color to RGB (1, 0, 0) which is red
}

//callback function for drawing a frame
void Display(void)
{
	// Alway Clear Color and Depth Bit Before Drawing!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	// Load Identity Matrix
	glLoadIdentity();

	// Setup Camera Configurations
	gluLookAt(CameraPosition[0], CameraPosition[1], CameraPosition[2],
		CameraLookPosition[0], CameraLookPosition[1], CameraLookPosition[2],
		CameraNormPosition[0], CameraNormPosition[1], CameraNormPosition[2]);
	RenderPlanes();

	LightPos[0] = doublePendulumSystems[1]->GetBottomBobPosition().x;
	LightPos[1] = doublePendulumSystems[1]->GetBottomBobPosition().y;
	LightPos[2] = doublePendulumSystems[1]->GetBottomBobPosition().z;
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

	// Render the double pendulum systems
	for (int i = 0; i < 3; i++)
	{
		if (i == 1 && !doublePendulumSystems[i]->isUsingTexture){
			doublePendulumSystems[i]->isLightSource = true;
			doublePendulumSystems[i]->textureID = texture[0];
		}
		doublePendulumSystems[i]->render();	
	}
	

	// Swap the drawn buffer to the window
	glutSwapBuffers();
}

//callback funtion as a passTimer, no need to modify it
void Tick(int id)
{
	double d = deltaTime / 1000.0;
	passTime += d;

	// update your physics here
	for (int i = 0; i < 3; i++)
	{
		// here, we can multiply d with a constant to speed up simulation
		if(doublePendulumSystems[i])
			doublePendulumSystems[i]->updatePhysics(d * 3);
	}

	glutPostRedisplay();
	glutTimerFunc(deltaTime, Tick, 0); // 100ms for passTime step size
}

//callback function when the window size is changed, no need to modify it
void WindowSize(int w, int h)
{
	glViewport(0, 0, w, h);							//changing the buffer size the same to the window size
	glMatrixMode(GL_PROJECTION);					//choose the projection matrix
	glLoadIdentity();
	gluPerspective(60.0, (double)w /(double)h, 1.0, 1000.0);//set the projection matrix as perspective mode
	glMatrixMode(GL_MODELVIEW);						//it is suggested that modelview matrix is chosen if nothing specific being performed
	glLoadIdentity();
}

//initialization for parameters of this program, you must perform something here
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);//set what color should be used when we clean the color buffer
	glEnable(GL_LIGHT1);//Enable Light1
	glEnable(GL_LIGHTING);//Enable Lighting
	//***********
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);//Set Light1 Position, this setting function should be at another place
	//***********
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);//Set Light1 Ambience
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);//Set Light1 Diffuse
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);//Set Light1 Specular
	//since a light source is also an object in the 3D scene, we set its position in the display function after gluLookAt()
	//you should know that straight texture mapping(without modifying shader) may not have shading effect
	//you need to tweak certain parameter(when building texture) to obtain a lit and textured object
	glShadeModel(GL_SMOOTH);//shading model

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//you can choose which part of lighting model should be modified by texture mapping
	glEnable(GL_COLOR_MATERIAL);//enable this parameter to use glColor() as material of lighting model
	
	//glDisable(GL_LIGHT1);
	//glDisable(GL_LIGHTING);

	//please load all the textures here
	//use Image* loadTexture(file_directory) function to obtain char* data and size info.
	
	// write your code here ...
	sun = loadTexture(texture_name[0]);
	ceiling = loadTexture(texture_name[1]);

	glGenTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, sun->sizeX, sun->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, sun->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ceiling->sizeX, ceiling->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ceiling->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// initialize double pendulum systems

	// write your code here ...
	doublePendulumSystems[0] = new DoublePendulum(Vector3f(5, 10, -5), 5, 3, 0.05, 1);
	doublePendulumSystems[1] = new DoublePendulum(Vector3f(0, 10, 0), 7, 5, 1, 1);
	doublePendulumSystems[2] = new DoublePendulum(Vector3f(-5, 10, 5), 3, 8, 2, 0.5);
}

void DealKeyboardInput(unsigned char key, int x, int y)
{	
	// Deal with Inputs from Keyboard

	std::cout << "Input = " << key << std::endl;

	// write your code here ...
	switch (tolower(key)){
		case 'a'://X軸移動
			CameraPosition[0]++;
			break;
		case 'd':
			CameraPosition[0]--;
			break;
		case 'w'://Y軸移動
			CameraPosition[1]++;
			break;
		case 's':
			CameraPosition[1]--;
			break;
		case 'q'://Z軸移動
			CameraPosition[2]++;
			break;
		case 'e':
			CameraPosition[2]--;
			break;
		case 'j'://Y軸旋轉
			CameraLookPosition[0]++;
			break;
		case 'l':
			CameraLookPosition[0]--;
			break;
		case 'i'://X軸旋轉
			CameraLookPosition[1]++;
			break;
		case 'k':
			CameraLookPosition[1]--;
			break;
		case 'u'://Z軸旋轉
			CameraRotate++;
			CameraNormPosition[0] = cos(CameraRotate * M_PI / 180);
			CameraNormPosition[1] = sin(CameraRotate * M_PI / 180);
			break;
		case 'o':
			CameraRotate--;
			CameraNormPosition[0] = cos(CameraRotate * M_PI / 180);
			CameraNormPosition[1] = sin(CameraRotate * M_PI / 180);
			break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//glut function for simplifying OpenGL initialization
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//demanding: double-framed buffer | RGB colorbuffer | depth buffer
	glutInitWindowPosition(100, 50);//set the initial window position
	glutInitWindowSize(800, 600);//set the initial window size
	//**************************
	glutCreateWindow("CG_HW1_0756626");//IMPORTANT!! Create the window and set its title, please replace 12345678 with your own student ID
	//**************************
	glutDisplayFunc(Display);//callback funtion for drawing a frame
	glutReshapeFunc(WindowSize);//callback function when the window size is changed
	glutTimerFunc(deltaTime, Tick, 0);//passTimer function
	glutKeyboardFunc(DealKeyboardInput);

	//you may want to write your own callback funtion for other events(not demanded nor forbidden)
	init();//self-defined initialization function for the elegance of your code

	glutMainLoop();
	return 0;
}