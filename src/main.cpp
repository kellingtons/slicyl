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
/***************************************************************************
Author: Kyle Ruan
Date Created: February 7th, 2016
Last Modified: April 9th, 2017
Version: 0.71
****************************************************************************/

#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "TriangleMesh.h"
#include "Triangle.h"
#include "dimensional_space.h"
#include "Slicer.h"
#include "SlicedLayers.h"

int main(int argc, char *argv[])
{
	// Initialize things
	char FileName[1024];
	Slicer slice;
	TriangleMesh* mesh = new TriangleMesh;
	SlicedLayers* layers = new SlicedLayers;
	
	if (argc < 5)
	{
		printf("ERROR in launching mesh generator!\nMake sure the input format is ./generate_mesh filename.stl startradius thickness endradius\n\n Try -h for help!\n");
		return 1;
	}

	// Get the command line arguments
	strcpy(FileName, argv[1]);
	float start_radius = strtof(argv[2], NULL);
	float thickness = strtof(argv[3], NULL);
	float radius = strtof(argv[4], NULL);
	
	// Load the file
	mesh->LoadSTLToMeshASCII(FileName);

	slice.exportSTL(mesh,"asdf.stl");
	
	// Slice it up
	slice.SliceMesh(mesh, layers, thickness, radius, start_radius);
    
	// Make a pretty picture
	slice.exportGIV(layers, mesh->GetBBoxSize());
	
	printf("%d Triangles created and sliced from radius %0.2f to %0.2f with thickness %0.2f from STL file %s !!\n\n=======================================================================================================\n",(int)mesh->GetMeshSize(),start_radius, radius, thickness, FileName);
	return 0;
}

	
