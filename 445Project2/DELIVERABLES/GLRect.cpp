/* GLRect.h
 * Project 2: Hierarchical Transformations
 * CS 445
 * Timothy Couch
 * 26 September 2017
 */

#include "GLRect.h"

GLRect::GLRect(void)
{
	shape = GL_POLYGON;
}

GLRect::GLRect(GLPoly::Vertex pos, GLPoly::Vertex relPos, float w, float h)
{
	setPosition(pos);

	addVertex(Vertex((1 - relPos.x) * w / 2, (1 - relPos.y) * h / 2));
	addVertex(Vertex((1 - relPos.x) * w / 2, (1 + relPos.y) * -h / 2));
	addVertex(Vertex((1 + relPos.x) * -w / 2, (1 + relPos.y) * -h / 2));
	addVertex(Vertex((1 + relPos.x) * -w / 2, (1 - relPos.y) * h / 2));
	addVertex(Vertex((1 - relPos.x) * w / 2, (1 - relPos.y) * h / 2));
}

GLRect::~GLRect(void)
{
}
