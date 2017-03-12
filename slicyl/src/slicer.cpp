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

#include "slicer.h"

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
TriangleMesh stl_to_mesh(const char *stlfile, int debug, bool binary)
{
    TriangleMesh tri_mesh;
    
    // If the STL file is in binary format
	if(binary)
	{
		FILE *f = fopen(stlfile, "rb");
		if (!f)
		{
			//return ;
		}
		char name[80];
		unsigned int nFaces;
		fread(name, 80, 1, f);
		fread((void*)&nFaces, 4, 1, f);
		float v[12];
		unsigned short uint16;

		for (size_t i=0; i<nFaces; ++i)
		{
			for (size_t j=0; j<12; ++j)
			{
				fread((void*)&v[j], sizeof(float), 1, f);
			}

			fread((void*)&uint16, sizeof(unsigned short), 1, f);
            
			Triangle tri(point(v[0], v[1], v[2]), point(v[3], v[4], v[5]), point(v[6], v[7], v[8]), point(v[9], v[10], v[11]));
            
			tri_mesh.AddTriangle(tri);
		}
		fclose(f);
	}
    
    // If the STL file is in ASCII format
	else
	{
		std::ifstream in(stlfile);
        
		if (!in.good())
		{
			printf("ERROR IN GENERATING MESH!\nMake sure you typed the file name correctly.\n");
			//return 1;
		}

        //Temp string
		std::string s0,s1; 	
        
        //Temporary points
		float p0, p1, p2;
        
        //Output Data
        point normal, vertex_1, vertex_2, vertex_3;
		
		printf("Creating Triangles..\n");
        
		while (!in.eof())
		{
			in >> s0;
			if (s0=="facet")
			{
                // "normal" x, y, z
				in >> s0 >> p0 >> p1 >> p2; 	
				normal = point(p0, p1, p2);
                
                // "outer" "loop"
				in >> s0 >> s1;		 	
                
                // "vertex" x y z
				in >> s0 >> p0 >> p1 >> p2;
                vertex_1 = point(p0, p1, p2);
                
                // "vertex" x y z
				in >> s0 >> p0 >> p1 >> p2;	
				vertex_2 = point(p0, p1, p2);

                // "vertex" x y z
				in >> s0 >> p0 >> p1 >> p2;	
                    vertex_3 = point(p0, p1, p2);
                
                // "endloop"
				in >> s0;			
                
                // "endfacet"
				in >> s0;			
                
                // Create a new triangle
				Triangle tri(normal, vertex_1, vertex_2, vertex_3);
                
                // Push the new Triangle onto the mesh
				tri_mesh.AddTriangle(tri);

			}
			else if (s0=="endsolid")
			{
				break;
			}
			
		}
		in.close();
	}

	tri_mesh.BBoxAdjust(); //Adjust bounding box for the model and center it around the origin
	tri_mesh.BBoxMoveCOG();
	TriangleMesh* return_mesh = new TriangleMesh();

	return_mesh = &tri_mesh;

	return tri_mesh;
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Slices a TriangleMesh into slicepieces according to a Slicyl
--| Args:
--|     mesh - Pointer to the TriangleMesh to be sliced
--|		output_slicepieces
--|     thickness - Thickness between Slicyls
--|     end_radius - Largest Slicyl radius
--|     start_radius - Smallest Slicyl radius
--|     debug - Debug flag
--| Return:
--|     A pointer to the created TriangleMesh
--|-------------------------------------------------------------------------
*/
int slice(const TriangleMesh* mesh, std::vector< std::vector<slicepiece> > &output_slicepieces, const float thickness, float end_radius, float start_radius, int debug)
{
		int s0 = 0;
		int s1 = 0;
		int s2 = 0;
		int s3 = 0;
		int s4 = 0;
		int s5 = 0;
		int s6 = 0;
		int slices = 0;

		for (float rad=start_radius; rad<end_radius; rad+=thickness) //FOR EVERY SLICE
		{
			slices++;
			std::vector<slicepiece> slicepiecedata;
			for (size_t j=0; j<mesh->GetMeshSize(); j++) //for every triangle
			{
				const Triangle &tri = mesh->GetTriangle(j);
				std::vector<point> intpoints; 
				int intersects = tri.FindIntersects(rad, intpoints);
				
				if (intersects==0)
				{
					s0++;
				}
				else if (intersects==1)
				{
					//printf("\n======CASE 1 INTERSECTION======\n");
					s1++;
				}
				else
				{
					//ROLLOUT
					
					for(int i=0;i<intersects;i++) //for every intersection point in the vector
					{
						float length = sqrt((intpoints[i].y*intpoints[i].y)+((intpoints[i].z - rad)*(intpoints[i].z - rad)));
						float theta = acos(1-((length*length)/(2*(rad*rad))));
						intpoints[i].y = theta*rad;
						intpoints[i].z = rad;
							
						//printf("%0.3f %0.3f %0.3f \n",intpoints[i].x,intpoints[i].y,intpoints[i].z);
					}
					
					if (intersects == 2) //case 2 output
					{				
						//printf("\n======CASE 2 INTERSECTION======\n");
						s2++;
						float distance, x1, y1, x0, y0;
						x1=intpoints[1].x;
						x0=intpoints[0].x;
						y1=intpoints[1].y;
						y0=intpoints[0].y;
						distance = sqrt((y1-y0)*(y1-y0))+((x1-x0)*(x1-x0));
						slicepiece sp = slicepiece(intpoints[0],intpoints[1],distance);
						slicepiecedata.push_back(sp);
					}
					if (intersects == 3)
					{
						//printf("\n======CASE 3 INTERSECTION======\n");
						s3++;
					}
					if (intersects == 4)
					{
						//printf("\n======CASE 4 INTERSECTION======\n");
						s4++;
					}
					if (intersects == 5)
					{
						//printf("\n======CASE 5 INTERSECTION======\n");
						s5++;
					}
					if (intersects == 6)
					{
						//printf("\n======CASE 6 INTERSECTION======\n");
						s6++;
					}		
							
				}
				/*
				for (int k=0; k<intersects; k++)
				{
					printf("%0.3f %0.3f %0.3f \n",intpoints[k].x,intpoints[k].y,intpoints[k].z);
				}	
				*/

			}
			output_slicepieces.push_back(slicepiecedata);
			
		}
		printf("\n\n\n=======================================================================================================\n\nCase 0: %d\nCase 1: %d\nCase 2: %d\nCase 3: %d\nCase 4: %d\nCase 5: %d\nCase 6: %d\n\nTotal slices: %d \n\n=======================================================================================================\n\n",s0,s1,s2,s3,s4,s5,s6,slices);
		
	return 0;
}

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
void exportGIV(std::vector<std::vector<slicepiece> > &output_slicepieces, const point &aabbSize) 
{
    FILE *f;
    float dx=0, dy=0;

    f=fopen("slicyl_out.marks", "w");
    if (!f){return;}
    printf("Genrating Output GIV file slicyl_out.marks now...\n");
    const size_t nSlices = output_slicepieces.size();
    const size_t slicePerRow = (size_t)sqrt((float)nSlices);

    for (size_t i=0; i<nSlices; i++) 
    {
        const std::vector<slicepiece> &sp = output_slicepieces[i];
        dx = (float)(i%slicePerRow)*(aabbSize.x*1.05f);
        dy = (float)(i/slicePerRow)*(aabbSize.y*1.05f);
	//fprintf(f, "\n\n$line");
	//fprintf(f, "\n$color red");
	//fprintf(f, "\n%f %f", (float)(i%slicePerRow)*aabbur.x*1.05f, (float)(i/slicePerRow)*aabbur.y*1.05f);
	//fprintf(f, "\n%f %f", (float)(i%slicePerRow)*aabbbl.x*1.05f, (float)(i/slicePerRow)*aabbbl.y*1.05f);
        for (size_t j=0; j<sp.size(); ++j) 
	{
            fprintf(f, "\n\n$line");
            fprintf(f, "\n$color blue");
            fprintf(f, "\n%f %f", dx+sp[j].a.x, dy+sp[j].a.y);
            fprintf(f, "\n%f %f", dx+sp[j].b.x, dy+sp[j].b.y);
        }
    }
    fclose(f);
    printf("...Done!\n\n");
}