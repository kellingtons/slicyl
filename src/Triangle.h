/***************************************************************************
The MIT License

Copyright (c) 2017 Kyle Ruan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <vector>
#include <cmath>
#include <stdio.h>

#include "dimensional_space.h"


/*
--|-------------------------------------------------------------------------
--| Class that represents a Triangle in 3D space
--|-------------------------------------------------------------------------
*/
class Triangle
{
public:
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Class constructor. Creates a Triangle with three vertices and 3 line segments.
	--| Args:
	--|     n: Point indicating the normal vector to the Triangle
    --|     p0: Point for one of the vertices
    --|     p1: Point for one of the vertices
    --|     p2: Point for one of the vertices
	--| Return:
	--|     A Triangle Object
	--|-------------------------------------------------------------------------
	*/
	Triangle(point n, point p0, point p1, point p2);
    
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Class destructor. 
	--| Args:
	--|     none
	--| Return:
	--|     none
	--|-------------------------------------------------------------------------
	*/
    ~Triangle(void);
    
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Moves (translates) each vertex of the triangle a specific amount defined by a point
	--| Args:
	--|     distance- Amount to move the triangle
	--| Return:
	--|     A translated Triangle Object
	--|-------------------------------------------------------------------------
	*/
    Triangle MoveTriangle(const point& distance);
    
	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Changes the x dimension of a single vertex
	--| Args:
	--|     vertex- Vertex of the triangle to change
	--|		new_value- new value to assign to the point
	--| Return:
	--|     Nothing
	--|-------------------------------------------------------------------------
	*/
	void MorphVertex_X(int vertex, float new_value);

	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Changes the y dimension of a single vertex
	--| Args:
	--|     vertex- Vertex of the triangle to change
	--|		new_value- new value to assign to the point
	--| Return:
	--|     Nothing
	--|-------------------------------------------------------------------------
	*/
	void MorphVertex_Y(int vertex, float new_value);

	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Changes the z dimension of a single vertex
	--| Args:
	--|     vertex- Vertex of the triangle to change
	--|		new_value- new value to assign to the point
	--| Return:
	--|     Nothing
	--|-------------------------------------------------------------------------
	*/
	void MorphVertex_Z(int vertex, float new_value);
	
	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Gets the coordinates of one of the vertices
	--| Args:
	--|     vertex- Which vertex is being requested
	--| Return:
	--|     point - The vertex requested
	--|-------------------------------------------------------------------------
	*/
    const point& GetVertex(int vertex) const;
	
	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Gets the dimensions of the normal vector
	--| Args:
	--|     None
	--| Return:
	--|     point - The normal vector of this triangle
	--|-------------------------------------------------------------------------
	*/
	const point& GetNormal() const;
	
	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Find the points of intersection between this Triangle and a Slicyl
	--| Args:
	--|     radius- Radius of the Slicyl
	--| Return:
	--|     A vector of all intersection points
	--|-------------------------------------------------------------------------
	*/
	std::vector<point> FindIntersects(float radius) const;

	
private:
    // All three vertices of the triangle
    point v[3];
    
    // The triangle's normal vector
	point normal;
    
    // The line segments of the triangle
	LineSeg line[3];
};
#endif // _TRIANGLE_H_