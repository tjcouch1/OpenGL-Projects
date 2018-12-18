/* GLPoly.cpp
 * Project 2: Hierarchical Transformations
 * CS 445
 * Timothy Couch
 * 26 September 2017
 */

#include "GLPoly.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

//set defaults
void GLPoly::init(void)
{
	shape = GL_TRIANGLE_STRIP;
	pointSize = 3.0f;
	lineWidth = 2.0f;
	position = Vertex(0, 0);
	rotation = 0;
	selected = false;
	colorSelected = Color(1, 0, 0, 1);
	colorDeselected = Color(0, 0, 0, 1);
	parent = nullptr;
}

GLPoly::GLPoly(void)
{
	init();
}

GLPoly::GLPoly(int s)
{
	init();
	shape = s;
}

GLPoly::GLPoly(int s, float pS, float lW)
{
	init();
	shape = s;
	pointSize = pS;
	lineWidth = lW;
}

GLPoly::GLPoly(GLPoly::Vertex pos)
{
	init();
	position = pos;
}

void GLPoly::setPosition(Vertex p)
{
	position = p;
}

GLPoly::Vertex GLPoly::getPosition()
{
	return position;
}

void GLPoly::setRotation(float r)
{
	rotation = r;
}

float GLPoly::getRotation()
{
	return rotation;
}

//add vertex with previous color
void GLPoly::addVertex(Vertex v)
{
	if (vertices.size() > 0)
		colors.push_back(colors.at(vertices.size() - 1));
	else colors.push_back(Color());
	vertices.push_back(v);
}

void GLPoly::addVertex(Vertex v, Color c)
{
	colors.push_back(c);
	vertices.push_back(v);
}

GLPoly::Vertex GLPoly::getVertex(int index)
{
	return vertices.at(index);
}

GLPoly::Color GLPoly::getColor(int index)
{
	return colors.at(index);
}

void GLPoly::setPolyColor(GLPoly::Color c)
{
	for (int i = 0; i < colors.size(); i++)
		colors.at(i) = c;
}

void GLPoly::addChild(GLPoly p)
{
	children.push_back(p);
	GLPoly &child = children.at(children.size() - 1);
	child.parent = this;
	child.childNum = children.size() - 1;
}

void GLPoly::updateChildren()
{
	for (int i = 0; i < children.size(); i++)
	{
		children.at(i).parent = this;
		children.at(i).updateChildren();
	}
}

GLPoly &GLPoly::getChild(int index)
{
	return children.at(index);
}

int GLPoly::getNumChildren()
{
	return children.size();
}

GLPoly* GLPoly::getParent()
{
	return parent;
}

int GLPoly::getChildNum()
{
	return childNum;
}

//draw the polygon itself after offsetting CS
void GLPoly::drawSelf()
{
	
	glPointSize(pointSize);
	glLineWidth(lineWidth);

	//transform CS to object
	glTranslatef(position.x, position.y, 0);
	glRotatef(rotation, 0, 0, 1);

	//draw shape
	glBegin(shape);

	for(int i=0; i < vertices.size(); i++)
	{
		glColor4f(colors.at(i).red, colors.at(i).green, colors.at(i).blue, colors.at(i).alpha);
		glVertex2f(vertices.at(i).x, vertices.at(i).y);
	}
	glEnd();

	//draw outline
	drawOutline();

	glPointSize(1.0f);
	glLineWidth(1.0f);
}

void GLPoly::drawOutline()
{
	glBegin(GL_LINE_LOOP);

	if (isSelected())
		glColor4f(colorSelected.red, colorSelected.green, colorSelected.blue, colorSelected.alpha);
	else glColor4f(colorDeselected.red, colorDeselected.green, colorDeselected.blue, colorDeselected.alpha);

	for(int i=0; i < vertices.size(); i++)
		glVertex2f(vertices.at(i).x, vertices.at(i).y);

	glEnd();
}

void GLPoly::draw()
{
	//save CS before drawing object
	glPushMatrix();

	drawSelf();
	for (int i = 0; i < children.size(); i++)
	{
		//save CS before drawing each child
		glPushMatrix();
		children.at(i).draw();
		//reset CS to before drawing child each time
		glPopMatrix();
	}

	//reset CS to before drawing object
	glPopMatrix();
}

void GLPoly::setSelected(bool s)
{
	selected = s;
}

bool GLPoly::isSelected()
{
	return selected;
}