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
	double currentX;
	double xInc;
	Edge(){}
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
			slope = ((double)(p1.y - p2.y)) / ((double)(p1.x - p2.x));
			xInc = slope!=0 ? 1.0 / slope : -1000000; //xInc is set to negative if it is a horizontal line
		}
		
	}
	Point lowestPoint()
	{
		return p1.y < p2.y ? p1 : p2;
	}

	void printEdge()
	{
		std::cout << "current x: " << currentX << ", xInc: " << xInc<<"\n";
	}

	void increase()
	{
		currentX = currentX + xInc;
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
	std::vector<Point> points;
public:
	Color c;
	Polygon(Color color){
		c = color;
	}
	void addEdge(Edge e) {
		edges.push_back(e);
	}
	void addPoint(Point p) {
		points.push_back(p);
	}
	std::vector<Edge> getEdges() { return edges; }
	std::vector<Point> getPoints() { return points; }
	int maxy()
	{
		int max = edges[0].maxY;
		for (Edge e : edges)
			if (e.maxY > max) max = e.maxY;
		return max;

	}

	int miny()
	{
		int min = edges[0].lowestPoint().y;
		for(Edge e : edges)
			if (e.lowestPoint().y < min) min = e.lowestPoint().y;
		return min;
	}
	
};

class ClippingWindow {
	Point topleft;
	Point bottomright;
	Point topright;
	Point bottomleft;
	std::vector<Edge> cwedges;
	Color c;
public:
	ClippingWindow(){}

	ClippingWindow(int upleftx,int uplefty)
	{
		topleft = Point(upleftx,uplefty);
		c = Color(0, 1, 0);
	}

	void updateDraw(int brightx, int brighty) {
		bottomright = Point(brightx, brighty);
		topright = Point(brightx, topleft.y);
		bottomleft = Point(topleft.x, brighty);
		//topline
		for (int i = topleft.x; i < topright.x; i++)
			setFramebuffer(i, topright.y, c.red, c.green, c.blue);

		//leftline
		for (int i = topleft.y; i < bottomleft.y; i++)
			setFramebuffer(topleft.x, i, c.red, c.green, c.blue);

		//rightline
		for (int i = topright.y; i < bottomright.y; i++)
			setFramebuffer(topright.x, i, c.red, c.green, c.blue);

		//bottomline
		for (int i = bottomleft.x; i < bottomright.x; i++)
			setFramebuffer(i, bottomleft.y, c.red, c.green, c.blue);
	}

	void finishDraw(int brightx, int brighty) {
		cwedges.push_back(Edge(topleft, topright));
		cwedges.push_back(Edge(topright, bottomright));
		cwedges.push_back(Edge(bottomright,bottomleft));
		cwedges.push_back(Edge(bottomleft, bottomright));
	}	

	std::vector<Edge> get_edges()
	{
		std::cout << cwedges.size();
		return cwedges;
	}
};


class PolygonManager {
	std::vector<Polygon> polygons;
	std::vector<Point> points;
	std::vector<std::vector<Edge> > sortedEdgeTable;
	std::vector<Polygon> clippedPolys;
	ClippingWindow cwindow;
public:	
	PolygonManager() {
		
	}
	bool newPolygon(Point firstpoint)
	{
		if (polygons.size() < 10) {
			//std::cout << "Creating an new polygon\n";
			Color c = Color(); //random color assigned for this polygon
			Polygon p(c);
			polygons.push_back(p);
			points.push_back(firstpoint);
			p.addPoint(firstpoint);
			//std::cout << "Point " << firstpoint.x << " , " << firstpoint.y << " saved\n";
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
			polygons[polygons.size() - 1].addPoint(p);
			//std::cout << "Point " << p.x << " , " << p.y << " saved\n";
			displayPoints();
			return true;
		}
		else
			return false;
	}
	void endPolygon(Point p)
	{
		points.push_back(p);
		polygons[polygons.size() - 1].addPoint(p);
		std::cout << "Point " << p.x << " , " << p.y << " saved\n";
		displayPoints();
		addEdges();
		generateSortedEdgeTable(polygons[polygons.size()-1]); //the screen goes from 0 to 400
		scanLineConversion(polygons[polygons.size() - 1]);
	}

	void displayPoints()
	{
		for (int i = 0; i < points.size(); i++) {
			setFramebuffer(points[i].x, points[i].y, polygons[polygons.size()-1].c.red, polygons[polygons.size() - 1].c.green, polygons[polygons.size() - 1].c.blue);
		}
	}

	void generateSortedEdgeTable(Polygon p)
	{
		//initialize the scanLines
		for (int i = 0; i < 400; i++) {
			std::vector<Edge> edges;
			sortedEdgeTable.push_back(edges);
		}
		std::vector<Edge> edges = p.getEdges();
		//std::cout << "unsorted edges: "<<edges.size()<<"\n";
		//for (int j = 0; j < edges.size(); j++)
		//	edges[j].printEdge();

		edges= sortEdges(edges);

		// std::cout << "sorted edges values: " << edges.size() << "\n";
		//for ( int j = 0; j < edges.size(); j++)
		//	std::cout << edges[j].currentX << "\n";

		//populate the scan lines vector
		for (int i = 0; i < edges.size(); i++)
		{
			if(edges[i].xInc != -1000000) //ignore horizontal lines
				sortedEdgeTable[edges[i].lowestPoint().y].push_back(edges[i]);
		}
	}

	void scanLineConversion(Polygon p)
	{
		//std::cout << "inside scan line conversion\n";
		std::vector<Edge> activeEdges;
		int line = p.miny();
		//std::cout << "starting line: " << line << '\n';
		while (line < p.maxy()) //do not fill the top line of the polygon
		{
			//add the edges that start at this line to the active edge table
			for (int i = 0; i < sortedEdgeTable[line].size(); i++) {
				activeEdges.push_back(sortedEdgeTable[line][i]);
				//sortedEdgeTable[line][i].printEdge();
			}
		//	std::cout << "active edges size: " << activeEdges.size()<<'\n';
			//remove the edges that end at this line from the active edge table
			for (int i = 0; i < activeEdges.size(); i++)
			{
				if (activeEdges[i].maxY == line) {
				//	std::cout << "at y= " << line << ", removed edge: ";
				//	activeEdges[i].printEdge();
					activeEdges.erase(activeEdges.begin() + i);
					
				}
			}
			activeEdges = sortEdges(activeEdges);
			//std::cout << "finished removing the finished edges\n";
			// fill interior pixels on the current scan line
			for (int i = 0; i < activeEdges.size(); i++)
			{
				if (i < activeEdges.size() - 1)
				{
					int startx = ceil(activeEdges[i].currentX);
					int endx = floor(activeEdges[i + 1].currentX);
					for (int j = startx; j < endx; j++)
					{
						//std::cout << "setting the frame buffer for this segment of the scan line\n";
						setFramebuffer(j, line, p.c.red, p.c.green, p.c.blue);
					}
					i++;
				}
			}

			// update the xvalues on edges in the Active Edge table -- this is updating the intersection points
			for (int i = 0; i < activeEdges.size(); i++)
			{
				if (activeEdges[i].xInc != -1000000) // at this point in program this should never fail
				{
					activeEdges[i].increase();
				}
			}
			//increment line
			line++;
		}
		sortedEdgeTable.clear();
		activeEdges.clear();
	}

	void redrawPolygons()
	{
		for (int i = 0; i < polygons.size(); i++) {
			generateSortedEdgeTable(polygons[i]);
			scanLineConversion(polygons[i]);
		}
	}
	
	void clipPolygons(ClippingWindow& c)
	{
		cwindow = c;
		for (int i = 0; i < polygons.size(); i++)
		{
			Polygon p = clip(polygons[i].getPoints());
			clippedPolys.push_back(p);
		}
		for (int i = 0; i < clippedPolys.size(); i++)
		{
			generateSortedEdgeTable(clippedPolys[i]);
			scanLineConversion(clippedPolys[i]);
		}
	}

	Polygon clip(std::vector<Point> ord_points)
	{
		std::vector<Edge> cedges = cwindow.get_edges();
		std::vector<Point> newVerts;
		std::vector<Point> input = ord_points;

		//Top edge first
		Point s = input[input.size() - 1];
		for (Point p : input)
		{
			if (s.y < cedges[0].maxY) {
				if (p.y <= cedges[0].maxY)
				{
					newVerts.push_back(p);
				}
				else //p is outside the plane
				{
					Edge temp(p, s); //to get how much x is increased for each y
					int newx = p.x;
					int newy = p.y;
					while (newy > cedges[0].maxY)
					{
						newy -= 1; // step y down
						newx -= temp.xInc; // modify x accordingly
					}
					newVerts.push_back(Point(newx, newy));
				}
			}
			else // s is outside
			{
				if (p.y <= cedges[0].maxY) // p is inside
				{
					Edge temp(p, s); //to get how much x is increased for each y
					int newx = s.x;
					int newy = s.y;
					while (newy > cedges[0].maxY)
					{
						newy -= 1; // step y down
						newx -= temp.xInc; // modify x accordingly
					}
					newVerts.push_back(Point(newx, newy));
					newVerts.push_back(p);
				}
			}
			s = p; // p becomes the new s

		}


		//right edge
		input = newVerts;
		newVerts.clear();
		 s = input[input.size() - 1];
		for (Point p : input)
		{
			if (s.x < cedges[1].currentX) // s is inside
			{
				if (p.x < cedges[1].currentX)
				{
					newVerts.push_back(p);
				}
				else // p is outside
				{
					Edge temp(p, s);
					double yInc = 1 / temp.xInc;
					int newx = p.x;
					int newy = p.y;
					while (newx > cedges[1].currentX)
					{
						newx -= 1;
						newy -= yInc;
					}
					newVerts.push_back(Point(newx, newy));
				}
			}
			else // s is outside
			{
				if (p.x < cedges[1].currentX) // p is inside
				{
					Edge temp(p, s);
					double yInc = 1 / temp.xInc;
					int newx = s.x;
					int newy = s.y;
					while (newx > cedges[1].currentX)
					{
						newx -= 1;
						newy -= yInc;
					}
					newVerts.push_back(Point(newx, newy));
					newVerts.push_back(p);
				}
			}


			s = p; //p becomes the new s
		}


		//botton edge
		input = newVerts;
		newVerts.clear();
		 s = input[input.size() - 1];
		for (Point p : input)
		{
			if (s.y > cedges[2].maxY) {
				if (p.y >= cedges[2].maxY)
				{
					newVerts.push_back(p);
				}
				else //p is outside the plane
				{
					Edge temp(p, s); //to get how much x is increased for each y
					int newx = p.x;
					int newy = p.y;
					while (newy < cedges[2].maxY)
					{
						newy += 1; // step y down
						newx += temp.xInc; // modify x accordingly
					}
					newVerts.push_back(Point(newx, newy));
				}
			}
			else // s is outside
			{
				if (p.y >= cedges[2].maxY) // p is inside
				{
					Edge temp(p, s); //to get how much x is increased for each y
					int newx = s.x;
					int newy = s.y;
					while (newy < cedges[2].maxY)
					{
						newy += 1; // step y down
						newx += temp.xInc; // modify x accordingly
					}
					newVerts.push_back(Point(newx, newy));
					newVerts.push_back(p);
				}
			}


			s = p; //p becomes the new s
		}


		//left edge
		input = newVerts;
		newVerts.clear();
		 s = input[input.size() - 1];
		for (Point p : input)
		{
			if (s.x > cedges[3].currentX) // s is inside
			{
				if (p.x > cedges[3].currentX)
				{
					newVerts.push_back(p);
				}
				else // p is outside
				{
					Edge temp(p, s);
					double yInc = 1 / temp.xInc;
					int newx = p.x;
					int newy = p.y;
					while (newx < cedges[3].currentX)
					{
						newx += 1;
						newy += yInc;
					}
					newVerts.push_back(Point(newx, newy));
				}
			}
			else // s is outside
			{
				if (p.x > cedges[3].currentX) // p is inside
				{
					Edge temp(p, s);
					double yInc = 1 / temp.xInc;
					int newx = s.x;
					int newy = s.y;
					while (newx < cedges[3].currentX)
					{
						newx += 1;
						newy += yInc;
					}
					newVerts.push_back(Point(newx, newy));
					newVerts.push_back(p);
				}
			}


			s = p; //p becomes the new s
		}

		//Generate the new polygon
		Color c = Color();
		Polygon poly(c);
		for (Point p : newVerts)
			poly.addPoint(p);
		for (int i = 0; i < newVerts.size(); i++)
		{
			if (i < newVerts.size() - 1)
				poly.addEdge(Edge(newVerts[i], newVerts[i + 1]));
			else
				poly.addEdge(Edge(newVerts[i], newVerts[0]));
		}
		for (Edge e : poly.getEdges())
			e.printEdge();
		return poly;
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
