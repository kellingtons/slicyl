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
#ifndef _DIMENSIONAL_SPACE_H_
#define _DIMENSIONAL_SPACE_H_

// A representation of a point in 3D space.
// Has operators for Translation, Subtraction, Addition, Division and Multiplication
typedef struct point 
{
	float x;
	float y;
	float z;

	point(float _x=0, float _y=0, float _z=0) : x(_x), y(_y), z(_z) 
	{
	}

    // Translation
	point& operator-=(const point &pt) 
	{ 
		x-=pt.x;
		y-=pt.y;
		z-=pt.z;
        
		return *this;    
	}
    
    // Subtraction
   	point operator-(const point &pt) 
	{ 
		return point(x-pt.x, y-pt.y, z-pt.z); 
	}
    
    // Addition
   	point operator+(const point &pt)
	{ 
		return point(x+pt.x, y+pt.y, z+pt.z); 
	}
    
    // Division
    point operator/(float a) 
	{ 
		return point(x/a, y/a, z/a); 
	}
    
    // Multiplication
    point operator*(float a) 	
	{ 
		return point(x*a, y*a, z*a); 
	}

}point;	

// Struct for a line segment consisting of two points. 
typedef struct LineSeg
{
	point pt0, pt1;
    LineSeg(point p0=point(), point p1=point()) 
    { 
        pt0=p0; 
        pt1=p1; 
    }

}LineSeg;

// Struct for a slicepiece used for toolpath generation. It's just two points and the distance between them.
typedef struct slicepiece
{
	point a;
	point b;
	float distance;
	
	slicepiece(point x, point y, float dist)
	{
		a = x;
		b = y;
		distance = dist;
	}
}slicepiece;

#endif //_DIMENSIONAL_SPACE_H_
