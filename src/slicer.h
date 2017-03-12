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
#ifndef _SLICER_H_
#define _SLICER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <stdio.h>
#include <vector>

#include "dimensional_space.h"
#include "TriangleMesh.h"
#include "Triangle.h"

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Parses an ASCII or Binary STL file and creates a TriangleMesh from the information
--| Args:
--|     stlfile - Pointer to the STL file
--|     debug- Debug flag
--|     binary- Is the file 
--| Return:
--|     A pointer to the created TriangleMesh
--|-------------------------------------------------------------------------
*/
TriangleMesh stl_to_mesh(const char *stlfile, int debug, bool binary);

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Slices a TriangleMesh into slicepieces according to a Slicyl
--| Args:
--|     mesh - Pointer to the TriangleMesh to be sliced
--|		output_slicepieces - 
--|     thickness - Thickness between Slicyls
--|     end_radius - Largest Slicyl radius
--|     start_radius - Smallest Slicyl radius
--|     debug - Debug flag
--| Return:
--|     A pointer to the created TriangleMesh
--|-------------------------------------------------------------------------
*/
int slice(const TriangleMesh *mesh, std::vector< std::vector<slicepiece> > &output_slicepieces, const float thickness, float end_radius, float start_radius, int debug);

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Exports a slicepiece set into GIV format
--| Args:
--|     output_slicepieces - Set of slicepieces to output
--|     aabbSize - Bounding box size
--| Return:
--|     none
--|-------------------------------------------------------------------------
*/
void exportGIV(std::vector<std::vector<slicepiece> > &output_slicepieces, const point &aabbSize);

#endif //_SLICER_H_