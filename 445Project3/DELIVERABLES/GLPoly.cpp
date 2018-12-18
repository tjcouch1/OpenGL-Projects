/* GLPoly.cpp
 * Project 3: Interactive Camera
 * CS 445
 * Timothy Couch
 * 1 November 2017
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

map<int, int> GLPoly::vertexMap = initVertexMap();

//set defaults
void GLPoly::init(void)
{
	shape = GL_TRIANGLE_STRIP;
	pointSize = 3.0f;
	lineWidth = 2.0f;
	position = Vertex(0, 0, 0);
	rotation = 0.0f;
	rotationAxis = Vertex(0, 1, 0);
	visible = true;
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

GLPoly::GLPoly(GLPoly::Vertex pos, int s, float lW)
{
	init();
	position = pos;
	shape = s;
	lineWidth = lW;
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

void GLPoly::setRotationAxis(GLPoly::Vertex a)
{
	rotationAxis = a;
}

GLPoly::Vertex GLPoly::getRotationAxis()
{
	return rotationAxis;
}

void GLPoly::setVisible(bool v)
{
	visible = v;
}

bool GLPoly::isVisible()
{
	return visible;
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

//draw the polygon itself after offsetting CS
void GLPoly::draw()
{
	if (visible)
	{
		glPushMatrix();

		glPointSize(pointSize);
		glLineWidth(lineWidth);

		//transform CS to object
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);

		//draw shape
		glBegin(shape);

		for(int i=0; i < vertices.size(); i++)
		{
			glColor4f(colors.at(i).red, colors.at(i).green, colors.at(i).blue, colors.at(i).alpha);
			glVertex3f(vertices.at(i).x, vertices.at(i).y, vertices.at(i).z);
		}
		glEnd();

		glPointSize(1.0f);
		glLineWidth(1.0f);

		glPopMatrix();
	}
}