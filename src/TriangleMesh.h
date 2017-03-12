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
#ifndef _TRIANGLEMESH_H_
#define _TRIANGLEMESH_H_

#include <vector>
#include "dimensional_space.h"
#include "Triangle.h"

#define PI 3.14159265

/*
--|-------------------------------------------------------------------------
--| Class that represents a TriangleMesh, or a vector of Triangles in 3D space
--|-------------------------------------------------------------------------
*/
class TriangleMesh
{
public:
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Class constructor. Sets bounding box to default values.
	--| Args:
	--|     none
	--| Return:
	--|     A TriangleMesh object
	--|-------------------------------------------------------------------------
	*/
	TriangleMesh(void);
    
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
	~TriangleMesh(void);
    
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Gets the number of triangles in the current TriangleMesh
	--| Args:
	--|     none
	--| Return:
	--|     size_t: Number of triangles in the TriangleMesh
	--|-------------------------------------------------------------------------
	*/
	size_t GetMeshSize() const;
    
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Gets a hold of a TriangleMesh
	--| Args:
	--|     none
	--| Returns:
	--|     A const std::vector of Triangles
	--|-------------------------------------------------------------------------
	*/  
    const std::vector<Triangle>& GetMesh() const;
        
	/*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Gets a hold of a single Triangle
	--| Args:
	--|     iterator- Which triangle in the vector
	--| Returns:
	--|     A const std::vector of Triangles
	--|-------------------------------------------------------------------------
	*/  

	const Triangle& GetTriangle(int iterator) const;
	
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Add a Triangle to the TriangleMesh
	--| Args:
	--|     tri - The Triangle to add to the TriangleMesh
	--| Return:
	--|     none
	--|-------------------------------------------------------------------------
	*/
	void AddTriangle(const Triangle& tri);

    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Gets the size of the TriangleMesh's Bounding Box
	--| Args:
	--|     none
	--| Return:
	--|     point - The size of the TriangleMesh's Bounding Box represented by a point
	--|-------------------------------------------------------------------------
	*/
   	point GetBBoxSize() const;

    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Move the TriangleMesh's Bounding Box
	--| Args:
	--|     none
	--| Return:
	--|     none
	--|-------------------------------------------------------------------------
	*/
	void BBoxAdjust();
    
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Move the TriangleMesh's Bounding Box's Center of Gravity
	--| Args:
	--|     none
	--| Return:
	--|     none
	--|-------------------------------------------------------------------------
	*/
	void BBoxMoveCOG();

private:
    /*
	--|-------------------------------------------------------------------------
	--| Purpose:
	--|     Check to see if a triangle is within the current Bounding Box
    --|     and adjust the Bounding Box if it is not
	--| Args:
	--|     tri - The triangle to test
	--| Return:
	--|     none
	--|-------------------------------------------------------------------------
	*/
    void BBoxRecalibrate(const Triangle& tri);
    
    // The TriangleMesh is actually just a vector of Triangles...you know
	std::vector<Triangle> mesh;
    
    // Bounding Box Point One
	point BBox_One;
    
    // Bounding Box Point Two
	point BBox_Two;
};
#endif //_TRIANGLEMESH_H_