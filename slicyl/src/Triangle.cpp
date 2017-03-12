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

#include "Triangle.h"

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
Triangle::Triangle(point n, point p0, point p1, point p2) : normal(n)
{
    //Define vertices
    v[0]=p0;
    v[1]=p1;
    v[2]=p2;
    
    //Define line segments
    line[0] = LineSeg(p0, p1);
    line[1] = LineSeg(p1, p2);
    line[2] = LineSeg(p2, p0);
    
}

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
Triangle::~Triangle(void)
{

}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Moves (translates) each vertex of the triangle a specific amount defined by a point
--| Args:
--|     distance- Amount to move the triangle
--| Return:
--|     A translated Triangle
--|-------------------------------------------------------------------------
*/
Triangle Triangle::MoveTriangle(const point& distance)
{ 
    // Move the vertices
    v[0]-=distance; 
    v[1]-=distance; 
    v[2]-=distance; 
    
    //Redefine the line segments
    line[0] = LineSeg(v[0], v[1]);
    line[1] = LineSeg(v[1], v[2]);
    line[2] = LineSeg(v[2], v[0]);
    
    return *this;
}

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
void Triangle::MorphVertex_X(int vertex, float new_value)
{ 
	v[vertex].x = new_value; 
}

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
void Triangle::MorphVertex_Y(int vertex, float new_value)
{ 
	v[vertex].y = new_value; 
}

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
void Triangle::MorphVertex_Z(int vertex, float new_value)
{ 
	v[vertex].z = new_value; 
}

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
const point& Triangle::GetVertex(int vertex) const
{
	return v[vertex];
}
	
/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Find the points of intersection between this Triangle and a Slicyl
--| Args:
--|     radius- Radius of the Slicyl
--|     intersects- Output vector for points of intersection
--| Return:
--|     num_intersections - Number of intersections detected
--|-------------------------------------------------------------------------
*/
int Triangle::FindIntersects(float radius, std::vector<point>& intersects) const
{
    // Initialize t values
    float t1[3] = {0,0,0};
    float t2[3] = {0,0,0};
    float x0, y0, z0, x1, y1, z1;
    
    int num_intersections = 0;
	
    // For each vertex of the triangle
    for (int vertex=0; vertex<3; vertex++)
    {
        // Lengths of each line segment used for calculations
        float u = this->line[vertex].pt1.x - this->line[vertex].pt0.x;
        float v = this->line[vertex].pt1.y - this->line[vertex].pt0.y;
        float w = this->line[vertex].pt1.z - this->line[vertex].pt0.z;
        
        //Get the delta from equating the Equation of a Circle to the Equation of a line consisting of two points
        
        //v^2 + w^2
        float A = pow(v, 2) + pow(w, 2); 
        
        //2*(y*v) + 2*(z*w)
        float B = (2.0f*(this->line[vertex].pt0.y * v)) + (2.0f*(this->line[vertex].pt0.z * w)); 
        
        //(y^2 + z^2) - r^2
        float C = (pow(this->line[vertex].pt0.y, 2)) + (pow(this->line[vertex].pt0.z, 2)) - (pow(radius,2)); 
        
        //B^2 - 4*A*C
        float delta = ((pow(B, 2))-(4.0f*A*C)); 

        // No Intersections...keep going
        if (delta < 0) 
        {
            continue;
        }
        
        // There could be one intersection here
        else if (delta == 0) 
        {
            //Make sure we aren't dividing by zero...
            if(A == 0) 
            {
                t1[vertex] = 0;
            }
            
            //Found one intersection
            else
            {
                //Define t values for finding the intersection point for one root
                t1[vertex] = (-1.0f * B)/(2.0f * A); 
            }
            
            // We dont really care about the other t value right now...
            t2[vertex] = 0;
            //Check to make sure t values are within range
            if (0 < t1[vertex] && t1[vertex] < 1) 
            {	
				printf("snacks0\n");
                //Get the coordinate of the intersection point
                x0 = this->line[vertex].pt0.x + (u*t1[vertex]); 
                y0 = this->line[vertex].pt0.y + (v*t1[vertex]);
                z0 = this->line[vertex].pt0.z + (w*t1[vertex]);
                
                //Push that point onto the output vector
                point found_point = point(x0, y0, z0);
                intersects.push_back(found_point); 
                printf("snacks0\n");
                num_intersections++;		
            }
        }
        
        //Two possible intersections here
        else if (delta > 0) 
        {
			
            //Define both t values as there are two roots
            t1[vertex] = (-1.0f * B + (sqrt(delta)))/(2.0f * A); 
            t2[vertex] = (-1.0f * B - (sqrt(delta)))/(2.0f * A);

            //Check to make sure the first t values are within range
            if (0 < t1[vertex] && t1[vertex] < 1) 
            {
                //Get the coordinate of the intersection point
                x0 = this->line[vertex].pt0.x + (u*t1[vertex]);
                y0 = this->line[vertex].pt0.y + (v*t1[vertex]);
                z0 = this->line[vertex].pt0.z + (w*t1[vertex]);	

                //Push that point onto the output vector
                point found_point = point(x0, y0, z0);
                intersects.push_back(found_point); 
                num_intersections++;
            }
            
            //Check to make sure the second t values are within range
            if (0 < t2[vertex] && t2[vertex] < 1) 
            {				
                //Get the coordinate of the intersection point
                x1 = this->line[vertex].pt0.x + (u*t2[vertex]); 
                y1 = this->line[vertex].pt0.y + (v*t2[vertex]);
                z1 = this->line[vertex].pt0.z + (w*t2[vertex]);				

                //Push that point onto the output vector
                point found_point_two = point(x1, y1, z1);
                intersects.push_back(found_point_two); 
                num_intersections++;
            }
        }
        else
        {
            // If we hit this there is a serious problem in the fabric of reality
            break;
        }
    }
    return num_intersections;
}
