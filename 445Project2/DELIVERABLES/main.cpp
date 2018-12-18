/* main.cpp
 * Project 2: Hierarchical Transformations
 * CS 445
 * Timothy Couch
 * 26 September 2017
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <map>
#include <vector>
#include "GLPoly.h"
#include "GLRect.h"
using namespace std;

float canvasSize[] = {20.0f, 20.0f};
int rasterSize[] = {600, 600};

GLPoly::Color backgroundColor = GLPoly::Color(1.0, 1.0, 1.0);

//root poly
GLPoly poly;

//polygon currently working on
GLPoly* currentPoly = nullptr;
//current polygon's index in parent's vector of children
int currentPolyIndex = -1;

//GLPoly::Color color;
//int shape = GL_TRIANGLE_STRIP;
//float pointSize = 2.5f;
//float lineWidth = 2.5f;

int rotateSpeed = 2;

GLPoly::Vertex mousePos;

//set stuff up
void init(void)
{
    mousePos.x = 0.0f;
	mousePos.y = 0.0f;
}

//draw everything on the screen
void display(void)
{
    glClearColor(backgroundColor.red, backgroundColor.green, backgroundColor.blue, backgroundColor.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//draw all polys
	poly.draw();

    glutSwapBuffers();
}

//resize the window
void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasSize[0], 0.0, canvasSize[1]);
    glViewport(0, 0, rasterSize[0], rasterSize[1]);
    
    glutPostRedisplay();
}

//handle keyboard presses
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
	{
        case 27://esc
			exit(0);
            break;
		case 97://a
			currentPoly->setRotation(currentPoly->getRotation() + rotateSpeed);
			glutPostRedisplay();
			break;
		case 100://d
			currentPoly->setRotation(currentPoly->getRotation() - rotateSpeed);
			glutPostRedisplay();
			break;
    }
}

void specialKeyboard(int key, int x, int y)
{
    switch(key)
	{
        case GLUT_KEY_DOWN:
			//get parent
			if (currentPoly->getParent() != nullptr)
			{
				currentPoly->setSelected(false);
				currentPoly = currentPoly->getParent();
				currentPoly->setSelected(true);
				currentPolyIndex = currentPoly->getChildNum();

				glutPostRedisplay();
			}
            break;
		case GLUT_KEY_UP:
			//get first child
			if (currentPoly->getNumChildren() > 0)
			{
				currentPoly->setSelected(false);
				currentPoly = &currentPoly->getChild(0);
				currentPoly->setSelected(true);
				currentPolyIndex = 0;

				glutPostRedisplay();
			}
			break;
		case GLUT_KEY_LEFT:
			//get parent's previous child
			if (currentPoly->getParent() != nullptr)
			{
				//get previous index
				currentPolyIndex -= 1;
				if (currentPolyIndex < 0)
					currentPolyIndex = currentPoly->getParent()->getNumChildren() - 1;

				//set previous child
				currentPoly->setSelected(false);
				currentPoly = &currentPoly->getParent()->getChild(currentPolyIndex);
				currentPoly->setSelected(true);

				glutPostRedisplay();
			}
			break;
		case GLUT_KEY_RIGHT:
			//get parent's next child
			if (currentPoly->getParent() != nullptr)
			{
				//get next index
				currentPolyIndex++;
				currentPolyIndex %= currentPoly->getParent()->getNumChildren();

				//set next child
				currentPoly->setSelected(false);
				currentPoly = &currentPoly->getParent()->getChild(currentPolyIndex);
				currentPoly->setSelected(true);

				glutPostRedisplay();
			}
			break;
	}
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("CS 445 Project 2");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);

	typedef GLPoly::Vertex Vertex;
	typedef GLPoly::Color Color;

	Color orange = Color(1, .6, .2);
	Color green = Color(.3, 1, .3);
	Color blue = Color(.5, .5, 1);
	Color gray = Color(.75, .75, .75);
	Color purple = Color(1, .4, 1);

	//pelvis
	GLRect rect = GLRect(Vertex(10, 8.5), Vertex(0, -1), 3, 2);
	rect.setPolyColor(blue);
	poly = rect;

	//chest
	rect = GLRect(Vertex(0, 2), Vertex(0, -1), 4, 3);
	rect.setPolyColor(gray);
	poly.addChild(rect);
	poly.updateChildren();

	//head
	rect = GLRect(Vertex(0, 3), Vertex(0, -1), 1, .5);
	rect.setPolyColor(orange);
	poly.getChild(0).addChild(rect);
	poly.updateChildren();

	rect = GLRect(Vertex(0, .5), Vertex(0, -1), 2.5, 2.25);
	rect.setPolyColor(orange);
	poly.getChild(0).getChild(0).addChild(rect);
	poly.updateChildren();

	//left arm
	rect = GLRect(Vertex(-2, 2.5), Vertex(1, 0), 1.5, 1);
	rect.setPolyColor(gray);
	poly.getChild(0).addChild(rect);
	poly.updateChildren();

	rect = GLRect(Vertex(-1.5, 0), Vertex(1, 0), 1.5, 1);
	rect.setPolyColor(orange);
	poly.getChild(0).getChild(1).addChild(rect);
	poly.updateChildren();

	rect = GLRect(Vertex(-1.5, 0), Vertex(1, 0), 1.5, 1.5);
	rect.setPolyColor(orange);
	poly.getChild(0).getChild(1).getChild(0).addChild(rect);
	poly.updateChildren();

	//right arm
	rect = GLRect(Vertex(2, 2.5), Vertex(-1, 0), 1.5, 1);
	rect.setPolyColor(gray);
	poly.getChild(0).addChild(rect);
	poly.updateChildren();

	rect = GLRect(Vertex(1.5, 0), Vertex(-1, 0), 1.5, 1);
	rect.setPolyColor(orange);
	poly.getChild(0).getChild(2).addChild(rect);
	poly.updateChildren();

	rect = GLRect(Vertex(1.5, 0), Vertex(-1, 0), 1.5, 1.5);
	rect.setPolyColor(orange);
	poly.getChild(0).getChild(2).getChild(0).addChild(rect);
	poly.updateChildren();

	//right leg
	rect = GLRect(Vertex(1, 0), Vertex(0, 1), 1, 2);
	rect.setPolyColor(blue);
	poly.addChild(rect);
	poly.updateChildren();
	
	rect = GLRect(Vertex(0, -2), Vertex(0, 1), 1, 2);
	rect.setPolyColor(purple);
	poly.getChild(1).addChild(rect);
	poly.updateChildren();
	
	rect = GLRect(Vertex(0, -2), Vertex(-.5, 1), 2, 1);
	rect.setPolyColor(green);
	poly.getChild(1).getChild(0).addChild(rect);
	poly.updateChildren();

	//left leg
	rect = GLRect(Vertex(-1, 0), Vertex(0, 1), 1, 2);
	rect.setPolyColor(blue);
	poly.addChild(rect);
	poly.updateChildren();
	
	rect = GLRect(Vertex(0, -2), Vertex(0, 1), 1, 2);
	rect.setPolyColor(purple);
	poly.getChild(2).addChild(rect);
	poly.updateChildren();
	
	rect = GLRect(Vertex(0, -2), Vertex(.5, 1), 2, 1);
	rect.setPolyColor(green);
	poly.getChild(2).getChild(0).addChild(rect);
	poly.updateChildren();

	currentPoly = &poly;
	currentPoly->setSelected(true);
	currentPolyIndex = 0;

    glutMainLoop();
    return 0;
    
    
}