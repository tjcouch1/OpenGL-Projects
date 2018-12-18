/* GLPoly.h
 * Project 2: Hierarchical Transformations
 * CS 445
 * Timothy Couch
 * 26 September 2017
 */

#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <vector>
#include <map>
using namespace std;

class GLPoly
{
public:
	struct Color
	{
		float red;
		float green;
		float blue;
		float alpha;

		Color()
		{
			red = 0.0;
			green = 0.0;
			blue = 0.0;
			alpha = 1.0;
		}
		Color(float r, float g, float b, float a)
		{
			red = r;
			green = g;
			blue = b;
			alpha = a;
		}
		Color(float r, float g, float b)
		{
			red = r;
			green = g;
			blue = b;
			alpha = 1.0;
		}
	};
	struct Vertex
	{
		float x;
		float y;

		Vertex()
		{
			x = 0.0;
			y = 0.0;
		}
		Vertex(float xStart, float yStart)
		{
			x = xStart;
			y = yStart;
		}
	};

	GLPoly(void);
	GLPoly(int s);
	GLPoly(int s, float pS, float lW);
	GLPoly(Vertex pos);

	void setPosition(Vertex p);
	Vertex getPosition();

	void setRotation(float r);
	float getRotation();

	void addVertex(Vertex v);
	void addVertex(Vertex v, Color c);
	Vertex getVertex(int index);
	Color getColor(int index);

	void setPolyColor(Color c);

	void addChild(GLPoly p);
	void updateChildren();
	GLPoly &getChild(int index);
	int getNumChildren();

	GLPoly* getParent();
	int getChildNum();

	void draw();

	void setSelected(bool s);
	bool isSelected();

protected:
	int shape;
	
	void drawSelf();
	void drawOutline();

	Color colorSelected;
	Color colorDeselected;

private:
	void init();
	vector<Vertex> vertices;
	vector<Color> colors;

	float pointSize;
	float lineWidth;

	Vertex position;
	float rotation;

	vector<GLPoly> children;
	GLPoly* parent; //do not delete
	int childNum; //this object's index in parent's children

	bool selected;
};

