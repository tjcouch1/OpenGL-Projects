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
	//map of number of vertices per shape. Initialized in a function below
	static map<int, int> vertexMap;

	GLPoly(void);
	GLPoly(int s);
	GLPoly(int s, float pS, float lW);
	void addVertex(Vertex v);
	void addVertex(Vertex v, Color c);
	Vertex getVertex(int index);
	void updateTempVert(Vertex v, Color c);
	Color getColor(int index);
	void draw();
	bool isCompletable(void);
	bool isComplete(void);
	void setComplete(bool comp);
private:
	void init();
	vector<Vertex> vertices;
	int shape;
	vector<Color> colors;
	bool complete;
	float pointSize;
	float lineWidth;
	
	//vert to draw transparent lines to and from (mouse position)
	Vertex tempVert;
	Color tempColor;

	//make map of vertices per shape
	static map<int, int> initVertexMap()
	{
		map<int, int> vertexMap;
		vertexMap[GL_POINTS] = 1;
		vertexMap[GL_TRIANGLES] = 3;
		vertexMap[GL_QUADS] = 4;
		vertexMap[GL_POLYGON] = -1;
		vertexMap[GL_LINES] = 2;
		vertexMap[GL_LINE_LOOP] = -1;
		return vertexMap;
	}
};

