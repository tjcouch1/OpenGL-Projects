/* main.cpp
 * Project 3: Interactive Camera
 * CS 445
 * Timothy Couch
 * 1 November 2017
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#include <iostream>
#include <map>
#include <vector>
#include "GLPoly.h"
using namespace std;

//keys held down
const int MOUSE_L = 0;
const int KEY_UP = 1;
const int KEY_DOWN = 2;
const int KEY_LEFT = 3;
const int KEY_RIGHT = 4;
bool keys[5];


float canvasSize[] = {20.0f, 20.0f};
int winSize[] = {1024, 512};

GLPoly::Color backgroundColor = GLPoly::Color(1.0, 1.0, 1.0);

//vector of all polys to draw
vector<GLPoly*> polys;

GLPoly::Vertex mousePos;
GLPoly::Vertex mousePosPrev;

//3d control vars
GLPoly::Vertex eye;
GLPoly::Vertex lookAt;
GLPoly::Vertex up;
float fov = 45.0f;
float nearPlane = .1f;
float farPlane = 100.0f;

float moveSpd = .1f;
float lookSpd = 3.0f;

//creates grid and axes
void createGrid()
{
	//grid
	GLPoly* grid = new GLPoly(GLPoly::Vertex(0, 0, 0), GL_LINES, 1);
	GLPoly::Color lineGray = GLPoly::Color(.4f, .4f, .4f);

	for (int i = 0; i <= 5; i++)//x
		for (int j = 0; j <= 5; j++)//z
		{
			grid->addVertex(GLPoly::Vertex(i, 0, 0), lineGray);
			grid->addVertex(GLPoly::Vertex(i, 0, 5), lineGray);

			grid->addVertex(GLPoly::Vertex(0, 0, j), lineGray);
			grid->addVertex(GLPoly::Vertex(5, 0, j), lineGray);
		}

	polys.push_back(grid);

	//axes
	GLPoly* axes = new GLPoly(GLPoly::Vertex(0, 0, 0), GL_LINES, 2);

	GLPoly::Color red = GLPoly::Color(1, 0, 0);
	GLPoly::Color green = GLPoly::Color(0, 1, 0);
	GLPoly::Color blue = GLPoly::Color(0, 0, 1);

	axes->addVertex(GLPoly::Vertex(0, 0, 0), red);
	axes->addVertex(GLPoly::Vertex(2, 0, 0), red);

	axes->addVertex(GLPoly::Vertex(0, 0, 0), green);
	axes->addVertex(GLPoly::Vertex(0, 2, 0), green);

	axes->addVertex(GLPoly::Vertex(0, 0, 0), blue);
	axes->addVertex(GLPoly::Vertex(0, 0, 2), blue);

	polys.push_back(axes);

	GLPoly* coolthing = new GLPoly(GLPoly::Vertex(1, 1, 1));
	coolthing->addVertex(GLPoly::Vertex(-1, -1, -1), red);
	coolthing->addVertex(GLPoly::Vertex(1, -1, 1), red);
	coolthing->addVertex(GLPoly::Vertex(1, 3, 2), green);
	coolthing->addVertex(GLPoly::Vertex(2, 4, 1), blue);
	coolthing->setVisible(false);

	polys.push_back(coolthing);
}

//set stuff up
void init(void)
{
	eye = GLPoly::Vertex(10, 10, -10);
	lookAt = GLPoly::Vertex(0, 0, 0);
	up = GLPoly::Vertex(0, 1, 0);

	createGrid();
}

//set up matrices and stuff
void initialGL()
{
	glEnable(GL_DEPTH_TEST);

    glClearColor(backgroundColor.red, backgroundColor.green, backgroundColor.blue, backgroundColor.alpha);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

//draw everything on the screen
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//draw all polys
	for (int i = 0; i < polys.size(); i++)
		polys.at(i)->draw();

    glutSwapBuffers();
}

void project()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, winSize[0]/winSize[1], nearPlane, farPlane);

    glViewport(0, 0, winSize[0], winSize[1]);
    
    glutPostRedisplay();
}

//resize the window
void reshape(int w, int h)
{
    winSize[0] = w;
    winSize[1] = h;

	project();
}

//get the mouse position relative to the canvas
GLPoly::Vertex getMouseCanvas(int mouseX, int mouseY)
{
	GLPoly::Vertex mouseP;
	mouseP.x = (float) mouseX / winSize[0] * canvasSize[0];
    mouseP.y = (float)(winSize[1] - mouseY) / winSize[1] * canvasSize[1];
	return mouseP;
}

int boolToInt(bool b)
{
	return b ? 1 : 0;
}

//runs all the time
void step()
{
	//move

	//forward/back
	int vdir = boolToInt(keys[KEY_DOWN]) - boolToInt(keys[KEY_UP]);
	float vspd = (float) vdir * moveSpd;
	if (vdir != 0)
	{
		glm::vec3 n = glm::normalize(eye.getVector() - lookAt.getVector());

		glm::vec3 newEye = eye.getVector() + n * vspd;
		eye = GLPoly::Vertex(newEye.x, newEye.y, newEye.z);

		glm::vec3 newLookAt = lookAt.getVector() + n * vspd;
		lookAt = GLPoly::Vertex(newLookAt.x, newLookAt.y, newLookAt.z);
	}

	//left/right
	int hdir = boolToInt(keys[KEY_RIGHT]) - boolToInt(keys[KEY_LEFT]);
	float hspd = (float) hdir * moveSpd;
	if (hdir != 0)
	{
		glm::vec3 n = glm::normalize(eye.getVector() - lookAt.getVector());
		glm::vec3 u = glm::normalize(glm::cross(up.getVector(), n));

		glm::vec3 newEye = eye.getVector() + u * hspd;
		eye = GLPoly::Vertex(newEye.x, newEye.y, newEye.z);

		glm::vec3 newLookAt = lookAt.getVector() + u * hspd;
		lookAt = GLPoly::Vertex(newLookAt.x, newLookAt.y, newLookAt.z);
	}

	//rotate camera
	if (keys[MOUSE_L])
	{
		glm::vec3 deltaMouse = mousePos.getVector() - mousePosPrev.getVector();

		glm::vec3 n = glm::normalize(eye.getVector() - lookAt.getVector());
		glm::vec3 u = glm::normalize(glm::cross(up.getVector(), n));
		glm::vec3 v = glm::cross(n, u);

		glm::mat4 lookDiff = glm::mat4(1.0f);
		lookDiff = glm::translate(lookDiff, eye.getVector());
		lookDiff = glm::rotate(lookDiff, -deltaMouse.x * lookSpd, v);
		lookDiff = glm::rotate(lookDiff, deltaMouse.y * lookSpd, u);
		lookDiff = glm::translate(lookDiff, -eye.getVector());

		glm::vec4 lookFinal = lookDiff * glm::vec4(lookAt.getVector(), 1.0f);

		lookAt = GLPoly::Vertex(lookFinal.x, lookFinal.y, lookFinal.z);

		//project();
	}

	mousePosPrev = mousePos;


	//reproject
	if (vdir != 0 || hdir != 0 || keys[MOUSE_L])
		project();
}

//handle left mouse click
void mouse (int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
			keys[MOUSE_L] = true;
		if (state == GLUT_UP)
			keys[MOUSE_L] = false;
	}
		
}

//handle mouse movement on screen
void motion(int x, int y)
{
    mousePos = getMouseCanvas(x, y);
}

//handle keyboard presses
void keyboard(unsigned char key, int x, int y)
{
	GLPoly* poly = polys.back();
    switch(key)
	{
    case 27://esc
		exit(0);
        break;
	case 119://w
		keys[KEY_UP] = true;
		break;
	case 97://a
		keys[KEY_LEFT] = true;
		break;
	case 115://s
		keys[KEY_DOWN] = true;
		break;
	case 100://d
		keys[KEY_RIGHT] = true;
		break;
	case 114://r
		poly->setRotation(poly->getRotation() + 10.0f);
		glutPostRedisplay();
		break;
	case 118://v
		poly->setVisible(!poly->isVisible());
		glutPostRedisplay();
		break;
    }
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 119://w
		keys[KEY_UP] = false;
		break;
	case 97://a
		keys[KEY_LEFT] = false;
		break;
	case 115://s
		keys[KEY_DOWN] = false;
		break;
	case 100://d
		keys[KEY_RIGHT] = false;
		break;
	}
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(winSize[0], winSize[1]);
	glutInitWindowPosition(0, 0);
    glutCreateWindow("CS 445 Project 3");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	initialGL();
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutIdleFunc(step);
    glutMainLoop();

    return 0;
}