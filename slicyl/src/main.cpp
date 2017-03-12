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
Last Modified: March 11th, 2017
Version: 0.7
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
#include "slicer.h"

int main(int argc, char *argv[])
{
	char FileName[1024];
	int debug = 0;
	bool binary = false;
	float radius;
	float startradius;
	float thick;

    // Redesign this...
	if (argc < 5)
	{
		printf("ERROR in launching mesh generator!\nMake sure the input format is ./generate_mesh filename.stl startradius thickness endradius\n\n Try -h for help!\n");
		return 1;
	}
	if (argc == 6)
	{
		if (0==strcmp(argv[5], "-b"))
			{binary = true;}
		if (0==strcmp(argv[5], "-d"))
			{debug = 1;}
	}	
	if (argc == 7)
	{
		if (0==strcmp(argv[5], "-b"))
			{binary = true;}
		if (0==strcmp(argv[5], "-d"))
			{debug = 1;}
		if (0==strcmp(argv[6], "-b"))
			{binary = true;}
		if (0==strcmp(argv[6], "-d"))
			{debug = 1;}
	}	
	char* temp = argv[2];
	char* tmp2 = argv[3];
	char* tmp3 = argv[4];
    
	radius = strtof(tmp3, NULL);
	thick = strtof(tmp2, NULL);
	startradius = strtof(temp, NULL);
	strcpy(FileName, argv[1]);

	
	TriangleMesh mesh = stl_to_mesh(FileName, debug, binary);
    
	std::vector< std::vector<slicepiece> > allslicepieces;
	
	slice(&mesh, allslicepieces, thick, radius, startradius, debug);
    
	exportGIV(allslicepieces,mesh.GetBBoxSize());
	
	
	//Data tap
	/*
	for(size_t i = 0;i<allslicepieces.size();i++)
	{
		const std::vector<slicepiece> test = allslicepieces[i];
		for(size_t j = 0;j<test.size();j++)
		{
		printf("%0.3f %0.3f %0.3f | %0.3f %0.3f %0.3f | %0.3f \n",test[j].a.x,test[j].a.y,test[j].a.z,test[j].b.x,test[j].b.y,test[j].b.z,test[j].distance);
		}
	}*/
	//printf("%d Triangles created and sliced from radius %0.2f to %0.2f with thickness %0.2f from STL file %s !!\n\n=======================================================================================================\n",(int)mesh->GetMeshSize(),startradius, radius, thick,FileName);
	return 0;
}

	
