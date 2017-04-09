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

#include "Slicer.h"

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Slices a TriangleMesh into slicepieces according to a Slicyl
--| Args:
--|     mesh - Pointer to the TriangleMesh to be sliced
--|     output_slicepieces
--|     thickness - Thickness between Slicyls
--|     end_radius - Largest Slicyl radius
--|     start_radius - Smallest Slicyl radius
--| Return:
--|     A pointer to the created TriangleMesh
--|-------------------------------------------------------------------------
*/
int Slicer::SliceMesh(const TriangleMesh* mesh, SlicedLayers* output, float thickness, float end_radius, float start_radius)
{
    printf("Slicing Model Now...be patient\n");
    int s0, s1, s2, s3, s4, s5, s6 = 0;
    int num_slices = 0;

    // For each Slicyl of such a radius
    for (float rad = start_radius; rad < end_radius + thickness; rad += thickness) 
    {
        num_slices++;
        std::vector<slicepiece> all_pieces_in_layer;
        // For each Triangle in the mesh
        for (size_t j = 0; j < mesh->GetMeshSize(); j++) 
        {
            //Grab a Triangle
            const Triangle &tri = mesh->GetTriangle(j);
            
            //Find the intersections between this Triangle and a Slicyl of such a radius
            std::vector<point> intersection_points = tri.FindIntersects(rad);
            
            // Nothing...too bad
            if (intersection_points.size()==0)
            {
                s0++;
            }
            // Just one intersection
            else if (intersection_points.size()==1)
            {
                s1++;
            }
            else
            {
                // Rollout
                for(size_t i = 0; i < intersection_points.size(); i++) //for every intersection point in the vector
                {
                    float length = sqrt((intersection_points[i].y*intersection_points[i].y)+((intersection_points[i].z - rad)*(intersection_points[i].z - rad)));
                    float theta = acos(1-((length*length)/(2*(rad*rad))));
                    intersection_points[i].y = theta*rad;
                    intersection_points[i].z = rad;
                        
                    //printf("%0.3f %0.3f %0.3f \n",intersection_points[i].x,intersection_points[i].y,intersection_points[i].z);
                }
                // Two intersections
                if (intersection_points.size() == 2) 
                {               
                    s2++;
                    float distance, x1, y1, x0, y0;
                    x1=intersection_points[1].x;
                    x0=intersection_points[0].x;
                    y1=intersection_points[1].y;
                    y0=intersection_points[0].y;
                    distance = sqrt((y1-y0)*(y1-y0))+((x1-x0)*(x1-x0));
                    slicepiece sp = slicepiece(intersection_points[0],intersection_points[1],distance);
                    all_pieces_in_layer.push_back(sp);
                }
                // Three intersections
                if (intersection_points.size() == 3)
                {
                    s3++;
                }
                // Four intersections
                if (intersection_points.size() == 4)
                {
                    s4++;
                }
                // Five intersections
                if (intersection_points.size() == 5)
                {
                    s5++;
                }
                // Six intersections
                if (intersection_points.size() == 6)
                {
                    s6++;
                }       
                        
            }
            /*
            for (int k=0; k<intersection_points.size(; k++)
            {
                printf("%0.3f %0.3f %0.3f \n",intersection_points[k].x,intersection_points[k].y,intersection_points[k].z);
            }   
            */

        }
        output->AddLayer(all_pieces_in_layer);
        
    }
    printf("\n\n\n=======================================================================================================\n\nCase 0: %d\nCase 1: %d\nCase 2: %d\nCase 3: %d\nCase 4: %d\nCase 5: %d\nCase 6: %d\n\nTotal slices: %d \n\n=======================================================================================================\n\n",s0,s1,s2,s3,s4,s5,s6,num_slices);
        
    return 0;
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Exports a slicepiece set into GIV format
--| Args:
--|     output_slices - Set of slicepieces to output
--|     aabbSize - Bounding box size
--| Return:
--|     none
--|-------------------------------------------------------------------------
*/
void Slicer::exportGIV(SlicedLayers* output_slices, const point &aabbSize) 
{
    FILE *f;
    float dx=0, dy=0;

    f=fopen("slicyl_out.marks", "w");
    if (!f){return;}
    printf("Genrating Output GIV file slicyl_out.marks now...\n");
    const size_t nSlices = output_slices->GetSize();
    const size_t slicePerRow = (size_t)sqrt((float)nSlices);

    for (size_t i=0; i<nSlices; i++) 
    {
        const std::vector<slicepiece> &sp = output_slices->GetLayer(i);
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

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Exports a mesh into STL
--| Args:
--|     mesh - Pointer to a mesh to export
--|     file_name - Name of the stl file
--| Return:
--|     none
--|-------------------------------------------------------------------------
*/

void Slicer::exportSTL(TriangleMesh* mesh, const char* file_name)
{
    std::ofstream out(file_name);

    out << "solid " << "ascii" << std::endl;
    
    // For each Triangle in the mesh
    for (size_t j = 0; j < mesh->GetMeshSize(); j++) 
    {
        //Grab a Triangle
        const Triangle &tri = mesh->GetTriangle(j);
        point normal = tri.GetNormal();
        point v0 = tri.GetVertex(0);
        point v1 = tri.GetVertex(1);
        point v2 = tri.GetVertex(2);
        out << "facet " << "normal " << normal.x << " " << normal.y << " " << normal.z << std::endl;
        out << "outer " << "loop" << std::endl;
        out << "vertex " << v0.x << " " << v0.y << " " << v0.z << " " << std::endl;
        out << "vertex " << v1.x << " " << v1.y << " " << v1.z << " " << std::endl;
        out << "vertex " << v2.x << " " << v2.y << " " << v2.z << " " << std::endl;
        out << "endloop" << std::endl;
        out << "endfacet" << std::endl;
    }
    out << "endsolid" << std::endl;
    out.close();
}
