
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <map>
#include <vector>
#include "GLPoly.h"
using namespace std;

float canvasSize[] = {10.0f, 10.0f};
int rasterSize[] = {800, 600};

GLPoly::Color backgroundColor = GLPoly::Color(1.0, 1.0, 1.0);

//vector of all polys to draw
vector<GLPoly> polys;

//polygon currently working on
GLPoly* currentPoly = nullptr;

GLPoly::Color color;
int shape = GL_TRIANGLES;
float pointSize = 2.5f;
float lineWidth = 2.5f;

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

	//update temporary vertex for current polygon
	if (currentPoly != nullptr)
		currentPoly->updateTempVert(mousePos, color);

	//draw all polys and current polygon
	for (int i = 0; i < polys.size(); i++)
		polys.at(i).draw();
	if (currentPoly != nullptr)
		currentPoly->draw();

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

//get the mouse position relative to the canvas
GLPoly::Vertex getMouseCanvas(int mouseX, int mouseY)
{
	GLPoly::Vertex mouseP;
	mouseP.x = (float) mouseX / rasterSize[0] * canvasSize[0];
    mouseP.y = (float)(rasterSize[1] - mouseY) / rasterSize[1] * canvasSize[1];
	return mouseP;
}

//delete the current polygon
void deleteCurrentPoly()
{
	delete currentPoly;
	currentPoly = nullptr;
	glutPostRedisplay();
}

//add the current polygon to the polygons vector and delete it
static void addCurrentPoly()
{
	polys.push_back(*currentPoly);
	deleteCurrentPoly();
}

//handle left mouse click
void mouse (int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mousePos = getMouseCanvas(x, y);

		//new current poly
		if (currentPoly == nullptr)
			currentPoly = new GLPoly(shape, pointSize, lineWidth);

		//add vertex of current mouse position
		currentPoly->addVertex(mousePos, color);

		//finish current polygon
		if (currentPoly != nullptr && currentPoly->isComplete())
			addCurrentPoly();

        glutPostRedisplay();
    }
}

//handle mouse movement on screen
void motion(int x, int y)
{
    mousePos = getMouseCanvas(x, y);
	glutPostRedisplay();
}

//right click menu actions
void menu(int value)
{
    switch (value) {
        case 0: //clear
			polys = vector<GLPoly>();
			deleteCurrentPoly();
            break;
        case 1: //exit
            exit(0);
		case 2://point
			shape = GL_POINTS;
			break;
		case 3://triangle
			shape = GL_TRIANGLES;
			break;
		case 4://quad
			shape = GL_QUADS;
			break;
		case 5://poly
			shape = GL_POLYGON;
			break;
		case 6://line
			shape = GL_LINES;
			break;
		case 7://lineloop
			shape = GL_LINE_LOOP;
			break;
		case 8://small point
			pointSize = 1.0f;
			break;
		case 9://medium point
			pointSize = 2.5f;
			break;
		case 10://large point
			pointSize = 5.0f;
			break;
		case 11://thin line
			lineWidth = 1.0f;
			break;
		case 12://average line
			lineWidth = 2.5f;
			break;
		case 13://thick line
			lineWidth = 5.0f;
			break;
        case 14: //red
			color = GLPoly::Color(1, 0, 0);
            glutPostRedisplay();
            break;
        case 15: //green
			color = GLPoly::Color(0, 1, 0);
            glutPostRedisplay();
            break;
        case 16: //blue
			color = GLPoly::Color(0, 0, 1);
            glutPostRedisplay();
            break;
        case 17: //yellow
			color = GLPoly::Color(1, 1, 0);
            glutPostRedisplay();
            break;
        case 18: //cyan
			color = GLPoly::Color(0, 1, 1);
            glutPostRedisplay();
            break;
        case 19: //magenta
			color = GLPoly::Color(1, 0, 1);
            glutPostRedisplay();
            break;
        case 20: //black
			color = GLPoly::Color(0, 0, 0);
            glutPostRedisplay();
            break;
        case 21: //white
			color = GLPoly::Color(1, 1, 1);
            glutPostRedisplay();
            break;
        case 22: //transparent
			color = GLPoly::Color(1, 1, 1, 0);
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

//handle keyboard presses
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
	{
        case 27://esc
			if (currentPoly != nullptr)
				deleteCurrentPoly();
			else exit(0);
            break;
		case 32://space
		case 13://enter
			//finish current poly
			if (currentPoly != nullptr && currentPoly->isCompletable())
			{
				currentPoly->setComplete(true);
				addCurrentPoly();
			}
			break;
			//1-9 change color
		case 49://1
			menu(14);
			break;
		case 50://2
			menu(15);
			break;
		case 51://3
			menu(16);
			break;
		case 52://4
			menu(17);
			break;
		case 53://5
			menu(18);
			break;
		case 54://6
			menu(19);
			break;
		case 55://7
			menu(20);
			break;
		case 56://8
			menu(21);
			break;
		case 57://9
			menu(22);
			break;
			//qwerty change shape
		case 113://q
			menu(2);
			break;
		case 119://w
			menu(3);
			break;
		case 101://e
			menu(4);
			break;
		case 114://r
			menu(5);
			break;
		case 116://t
			menu(6);
			break;
		case 121://y
			menu(7);
			break;
    }
}

//make the right-click menu
void createMenu()
{
	int shapeMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Point", 2);
	glutAddMenuEntry("Triangle", 3);
	glutAddMenuEntry("Quad", 4);
	glutAddMenuEntry("Polygon", 5);
	glutAddMenuEntry("Line", 6);
	glutAddMenuEntry("Line Loop", 7);

    int pointMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 8);
    glutAddMenuEntry("Medium", 9);
    glutAddMenuEntry("Large", 10);

    int lineMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Thin", 11);
    glutAddMenuEntry("Average", 12);
    glutAddMenuEntry("Thick", 13);

    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 14);
    glutAddMenuEntry("Green", 15);
    glutAddMenuEntry("Blue", 16);
    glutAddMenuEntry("Yellow", 17);
    glutAddMenuEntry("Cyan", 18);
    glutAddMenuEntry("Magenta", 19);
    glutAddMenuEntry("Black", 20);
    glutAddMenuEntry("White", 21);
    glutAddMenuEntry("Transparent", 22);
    
    glutCreateMenu(menu);
	glutAddSubMenu("Shapes", shapeMenu);
    glutAddSubMenu("Colors", colorMenu);
    glutAddSubMenu("Point Size", pointMenu);
    glutAddSubMenu("Line Width", lineMenu);
    glutAddMenuEntry("Clear", 0);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("CS 445 Project 1");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    createMenu();
    glutMainLoop();
    return 0;
    
    
}