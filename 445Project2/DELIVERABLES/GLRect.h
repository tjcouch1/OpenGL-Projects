/* GLRect.h
 * Project 2: Hierarchical Transformations
 * CS 445
 * Timothy Couch
 * 26 September 2017
 */

#pragma once
#include "glpoly.h"
class GLRect : public GLPoly
{
public:
	GLRect(void);
	GLRect(Vertex pos, Vertex relPos, float w, float h);
	~GLRect(void);
protected:
};

