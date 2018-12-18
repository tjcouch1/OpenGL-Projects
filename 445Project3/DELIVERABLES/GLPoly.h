/* GLPoly.h
 * Project 3: Interactive Camera
 * CS 445
 * Timothy Couch
 * 1 November 2017
 */

#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <vector>
#include <map>
#include <glm/glm.hpp>
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
		float z;

		Vertex()
		{
			x = 0.0;
			y = 0.0;
			z = 0.0;
		}
		Vertex(float xStart, float yStart, float zStart)
		{
			x = xStart;
			y = yStart;
			z = zStart;
		}

		glm::vec3 getVector()
		{
			return glm::vec3(x, y, z);
		}
	};
	//map of number of vertices per shape. Initialized in a function below
	static map<int, int> vertexMap;

	GLPoly(void);
	GLPoly(int s);
	GLPoly(int s, float pS, float lW);
	GLPoly(Vertex pos);
	GLPoly(Vertex pos, int s, float lW);

	void setPosition(Vertex p);
	Vertex getPosition();

	void setRotation(float r);
	float getRotation();

	void setRotationAxis(Vertex a);
	Vertex getRotationAxis();

	void setVisible(bool v);
	bool isVisible();

	void addVertex(Vertex v);
	void addVertex(Vertex v, Color c);
	Vertex getVertex(int index);
	Color getColor(int index);

	void setPolyColor(Color c);

	void draw();

private:
	void init();
	vector<Vertex> vertices;
	int shape;
	vector<Color> colors;

	float pointSize;
	float lineWidth;

	Vertex position;
	float rotation;
	Vertex rotationAxis;

	bool visible;

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

