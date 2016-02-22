#pragma once
#include <math.h>
#include<vector>
#include <list>
#include <algorithm>

extern void setFramebuffer(int x, int y, float R, float G, float B);

class Point {
public:
	int x;
	int y;
	Point(int xcoord, int ycoord) {
		x = xcoord;
		y = ycoord;
	}
	Point() {
		x = 0;
		y = 0;
	}
};

class Edge {
	Point p1;
	Point p2;	
	double slope;
public:
	int maxY;
	int currentX;
	double xInc;
	Edge(Point x, Point y)
	{
		p1 = x;
		p2 = y;
		maxY = p1.y > p2.y ? p1.y:p2.y; //max y value for this edge
		currentX = p1.y < p2.y ? p1.x : p2.x; //start drawing the line at the lowest points x value
		if (p1.x - p2.x == 0) {
			slope = INFINITY;
			xInc = 0; // xInc is zero for a vertical line
		}
		else
		{
			slope =abs( (p1.y - p2.y) / (p1.x - p2.x));
			xInc = slope > 0 ? 1 / slope : -1; //xInc is set to negative if it is a horizontal line
		}
		
	}
	Point lowestPoint()
	{
		return p1.y < p2.y ? p1 : p2;
	}

	void printEdge()
	{
		std::cout << "( " << currentX << " , " << lowestPoint().y<<" )\n";
	}

};

class Color {
public:
	float red;
	float green;
	float blue;

	Color() { red = (rand() % 90 + 10) / 100.0; green = (rand() % 90 + 10) / 100.0; blue = (rand() % 90 + 10) / 100.0; }
	Color(float r, float g, float b) {
		red = r;
		green = g;
		b = blue;
	}
};

class Polygon {
	std::vector<Edge> edges;
	
public:
	Color c;
	Polygon(Color color){
		c = color;
	}
	void addEdge(Edge e) {
		edges.push_back(e);
	}
	std::vector<Edge> getEdges() { return edges; }

	int maxy()
	{
		int max = edges[0].maxY;
		for each(Edge e in edges)
			if (e.maxY > max) max = e.maxY;
		return max;

	}

	int miny()
	{
		int min = edges[0].lowestPoint().y;
		for each(Edge e in edges)
			if (e.lowestPoint().y > min) min = e.lowestPoint().y;
		return min;
	}
	
};

class PolygonManager {
	std::vector<Polygon> polygons;
	std::vector<Point> points;
	std::vector<std::list<Edge> > sortedEdgeTable;
public:	
	PolygonManager() {
		
	}
	bool newPolygon(Point firstpoint)
	{
		if (polygons.size() < 10) {
			std::cout << "Creating an new polygon\n";
			Color c = Color(); //random color assigned for this polygon
			Polygon p(c);
			polygons.push_back(p);
			points.push_back(firstpoint);
			std::cout << "Point " << firstpoint.x << " , " << firstpoint.y << " saved\n";
			displayPoints();
			return true;
		}
		else
			return false;
	}
	bool addPoint(Point p)
	{
		if (points.size() < 10) {
			points.push_back(p);
			std::cout << "Point " << p.x << " , " << p.y << " saved\n";
			displayPoints();
			return true;
		}
		else
			return false;
	}
	void endPolygon(Point p)
	{
		points.push_back(p);
		std::cout << "Point " << p.x << " , " << p.y << " saved\n";
		displayPoints();
		addEdges();
		generateScanLines(0,400);
	}

	void displayPoints()
	{
		for (int i = 0; i < points.size(); i++) {
			setFramebuffer(points[i].x, points[i].y, polygons[polygons.size()-1].c.red, polygons[polygons.size() - 1].c.green, polygons[polygons.size() - 1].c.blue);
		}
	}

	void generateScanLines(int lowy, int highy)
	{
		//initialize the scanLines
		for (int i = 0; i < highy-lowy; i++) {
			std::list<Edge> edges;
			sortedEdgeTable.push_back(edges);
		}
		std::vector<Edge> edges = polygons[polygons.size() - 1].getEdges();
		std::cout << "unsorted edges: "<<edges.size()<<"\n";
		for (int j = 0; j < edges.size(); j++)
			edges[j].printEdge();

		edges= sortEdges(edges);

		 std::cout << "sorted edges values: " << edges.size() << "\n";
		for ( int j = 0; j < edges.size(); j++)
			std::cout << edges[j].currentX << "\n";

		//populate the scan lines vector
		for (int i = 0; i < edges.size(); i++)
		{
			sortedEdgeTable[edges[i].lowestPoint().y].push_back(edges[i]);
		}
	}
	
private:
	void addEdges()
	{
		for (int i = 0; i < points.size(); i++) {
			if (i < points.size() - 1)
			{
				Edge e(points[i], points[i + 1]);
				polygons[polygons.size() - 1].addEdge(e);
			}
			else {
				Edge e(points[i], points[0]); //last edge is from the last point to the first
				polygons[polygons.size() - 1].addEdge(e);
			}
			
		}
		points.clear();
	}

	std::vector<Edge> sortEdges(std::vector<Edge>e) {
		std::vector<Edge> sortedEdges;
		sortedEdges.push_back(e[0]);

		for (int i = 1; i < e.size(); i++)
		{
			for (int j = 0 ; j<sortedEdges.size(); j++)
			{
				if (sortedEdges[j].currentX >= e[i].currentX) {
					sortedEdges.insert(sortedEdges.begin()+j, e[i]);
					break;
				}
				else if (j == sortedEdges.size() - 1) //there is none in the sorted list bigger than this one
				{
					sortedEdges.push_back(e[i]);
					break;
				}
			}
		}
		return sortedEdges;
	}

};
