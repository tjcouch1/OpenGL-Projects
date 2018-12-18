#include "GLPoly.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <vector>
#include <algorithm>
using namespace std;

map<int, int> GLPoly::vertexMap = initVertexMap();

//set defaults
void GLPoly::init(void)
{
	shape = GL_TRIANGLES;
	//vertices = vector<Vertex>();
	complete = false;
	pointSize = 3.0f;
	lineWidth = 2.0f;
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

//add vertex with previous color
void GLPoly::addVertex(Vertex v)
{
	if (vertices.size() > 0)
		colors.push_back(colors.at(vertices.size() - 1));
	else colors.push_back(Color());
	vertices.push_back(v);
	isComplete();
}

void GLPoly::addVertex(Vertex v, Color c)
{
	colors.push_back(c);
	vertices.push_back(v);
	isComplete();
}

GLPoly::Vertex GLPoly::getVertex(int index)
{
	return vertices.at(index);
}

void GLPoly::updateTempVert(Vertex v, Color c)
{
	tempVert = v;
	tempColor = c;
}

GLPoly::Color GLPoly::getColor(int index)
{
	return colors.at(index);
}

//draw the whole thing together as a line loop
/*void GLPoly::draw(void)
{
	vector<Vertex> verts = vertices;
	vector<Color> cols = colors;
	int maxVerts = vertexMap[shape];
	
	if (shape == GL_POINTS)
		glPointSize(pointSize);
	if (shape == GL_LINES || shape == GL_LINE_LOOP)
		glLineWidth(lineWidth);

	if (!isComplete())
	{
		verts.push_back(tempVert);
		cols.push_back(tempColor);
		//if (maxVerts <= 0 || verts.size() < maxVerts)
			glBegin(GL_LINE_LOOP);
		//else glBegin(GL_LINE_LOOP);
	}
	else glBegin(shape);

    for(int i=0; i < verts.size(); i++)
	{
		glColor4f(cols.at(i).red, cols.at(i).green, cols.at(i).blue, cols.at(i).alpha);
        glVertex2f(verts.at(i).x, verts.at(i).y);
	}
    glEnd();
	glPointSize(1.0);
	glLineWidth(1.0);
}*/

//draw the polygon itself, then draw transparent lines to the temp vertex
void GLPoly::draw(void)
{
	int maxVerts = vertexMap[shape];
	
	if (shape == GL_POINTS)
		glPointSize(pointSize);
	if (shape == GL_LINES || shape == GL_LINE_LOOP)
		glLineWidth(lineWidth);

	if (!isComplete())
		glBegin(GL_LINE_LOOP);
	else glBegin(shape);

    for(int i=0; i < vertices.size(); i++)
	{
		glColor4f(colors.at(i).red, colors.at(i).green, colors.at(i).blue, colors.at(i).alpha);
        glVertex2f(vertices.at(i).x, vertices.at(i).y);
	}
    glEnd();
	
	if (shape == GL_LINES || shape == GL_LINE_LOOP)
		glLineWidth(lineWidth);
	else glLineWidth(1.0f);

	if (!isComplete())
	{
		float tempAlpha = .25f;

		glBegin(GL_LINE_STRIP);
		glColor4f(colors.at(vertices.size() - 1).red, colors.at(vertices.size() - 1).green, colors.at(vertices.size() - 1).blue, colors.at(vertices.size() - 1).alpha * tempAlpha);
        glVertex2f(vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);

		glColor4f(tempColor.red, tempColor.green, tempColor.blue, tempColor.alpha * tempAlpha);
        glVertex2f(tempVert.x, tempVert.y);

		glColor4f(colors.at(0).red, colors.at(0).green, colors.at(0).blue, colors.at(0).alpha * tempAlpha);
        glVertex2f(vertices.at(0).x, vertices.at(0).y);

		glEnd();
	}
	
	glPointSize(1.0f);
	glLineWidth(1.0f);
}

//whether or not the polygon is manually completable
bool GLPoly::isCompletable(void)
{
	return vertexMap[shape] <= 0;
}

//checks/sets whether or not the polygon has all vertices
bool GLPoly::isComplete(void)
{
	if (complete == false && vertexMap[shape] > 0 && vertices.size() >= vertexMap[shape])
		complete = true;
	return complete;
}

void GLPoly::setComplete(bool comp)
{
	complete = comp;
}