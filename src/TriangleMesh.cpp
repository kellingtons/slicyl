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

#include "TriangleMesh.h"


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

TriangleMesh::TriangleMesh(void)
{
    BBox_One = point(999999, 999999, 999999);
    BBox_Two = point(-999999, -999999, -999999);
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

TriangleMesh::~TriangleMesh(void)
{

}

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

size_t TriangleMesh::GetMeshSize() const
{
    return mesh.size();
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Fills a Triangle Mesh with the data from a binary STL file
--| Args:
--|     none
--| Returns:
--|     A const std::vector of Triangles
--|-------------------------------------------------------------------------
*/  
void TriangleMesh::LoadSTLToMeshBinary(const char* stl_file)
{
    FILE *f = fopen(stl_file, "rb");
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
        
        const Triangle tri(point(v[0], v[1], v[2]), point(v[3], v[4], v[5]), point(v[6], v[7], v[8]), point(v[9], v[10], v[11]));
        
        const Triangle& tri_ref = tri;
        
        this->AddTriangle(tri_ref);
    }
    fclose(f);
    
    this->BBoxAdjust();

}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Fills a Triangle Mesh with the data from an ASCII STL file
--| Args:
--|     none
--| Returns:
--|     A const std::vector of Triangles
--|-------------------------------------------------------------------------
*/  
void TriangleMesh::LoadSTLToMeshASCII(const char* stl_file)
{
// If the STL file is in ASCII format

    std::ifstream in(stl_file);
    
    if (!in.good())
    {
        printf("ERROR IN GENERATING MESH!\nMake sure you typed the file name correctly.\n");
        //return 1;
    }

    //Temp stuff
    std::string s0,s1;  
    float p0, p1, p2;
    
    //Output Data
    point normal, vertex_1, vertex_2, vertex_3;
    
    printf("Creating Triangles..\n");
    
    // Read the STL file one triangle at a time
    while (!in.eof())
    {
        in >> s0;
        // ASCII STL files begin with the word facet
        if (s0=="facet")
        {
            // Read: "normal" x, y, z
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
            
            // Create a new Triangle with the data
            Triangle tri(normal, vertex_1, vertex_2, vertex_3);
            
            // Add the new Triangle onto the mesh
            this->AddTriangle(tri);

        }
        // Keyword marking the end of the file
        else if (s0=="endsolid")
        {
            break;
        }
        
    }
    in.close();

    //this->BBoxMoveCOG(point(0,0,0));
    this->BBoxAdjust();
}

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

const std::vector<Triangle>& TriangleMesh::GetMesh() const
{
    return mesh;
}

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

const Triangle& TriangleMesh::GetTriangle(int iterator) const
{
    return mesh[iterator];
}

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
void TriangleMesh::AddTriangle(const Triangle& tri)
{
    // Push the triangle onto the TriangleMesh vector
    mesh.push_back(tri);
    
    // Recalibrate the Bounding Box as needed
    BBoxRecalibrate(tri);
}   

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

point TriangleMesh::GetBBoxSize() const
{
    float x_size = BBox_Two.x-BBox_One.x;
    float y_size = BBox_Two.y-BBox_One.y;
    float z_size = BBox_Two.z-BBox_One.z;
    
    return point(x_size, y_size, z_size);
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Move the TriangleMesh's Bounding Box so 
--| Args:
--|     none
--| Return:
--|     none
--|-------------------------------------------------------------------------
*/
void TriangleMesh::BBoxAdjust()
{
    // For now, the vector of which we do things with will be defined as going straight through the middle of the box
    
    point vectorbutt(BBox_One.x, (BBox_Two.y-BBox_One.y)/2.0f, (BBox_Two.z-BBox_One.z)/2.0f);
    point vectorhead(BBox_Two.x, (BBox_Two.y-BBox_One.y)/2.0f, (BBox_Two.z-BBox_One.z)/2.0f);

    printf("butt: %0.3f %0.3f %0.3f head: %0.3f %0.3f %0.3f\n",vectorbutt.x,vectorbutt.y,vectorbutt.z,vectorhead.x,vectorhead.y,vectorhead.z);
    
    point distance = vectorbutt;
    point tmp;
    vectorbutt -= distance;
    vectorhead -= distance;
    printf("\ntranslation: %0.3f %0.3f %0.3f\n",vectorhead.x,vectorhead.y,vectorhead.z);
    for (size_t i=0; i<mesh.size(); i++) //translation
    {
        Triangle &tri = mesh[i];
        tri.MoveTriangle(distance);
        BBoxRecalibrate(tri);
    }
    //rotation 1
    //float zmag = sqrt((vectorhead.x*vectorhead.x) + (vectorhead.y*vectorhead.y) + (vectorhead.z*vectorhead.z));

    //float phi = acos(vectorhead.x/zmag);
    float phi = atan(vectorhead.y/vectorhead.x);

    if(vectorhead.x >= 0.0f)
    {
        if(vectorhead.y > 0.0f)
        {
            phi = -phi;
        }
        else
        {
            phi = -phi;
        }       
    }
    else if (vectorhead.x < 0.0f)
    {
        if(vectorhead.y > 0.0f)
        {
            phi = PI - phi;
        }
        else
        {
            phi = PI - phi;
        }
    }
    //move the vector

    tmp.x = vectorhead.x*cos(phi)-vectorhead.y*sin(phi);
    tmp.y = vectorhead.y*cos(phi)+vectorhead.x*sin(phi);
    vectorhead.x = tmp.x;
    vectorhead.y = tmp.y;
    printf("\nrotation 1: %0.3f %0.3f %0.3f\n",vectorhead.x,vectorhead.y,vectorhead.z);
    //move the rest of the model
    for (size_t k=0; k<mesh.size(); k++)
    {
        Triangle &tri = mesh[k];
        for (int m = 0; m<3; m++) //rotate, keep z constant
        {
            tmp.x = tri.GetVertex(m).x*cos(phi)-tri.GetVertex(m).y*sin(phi);
            tmp.y = tri.GetVertex(m).x*sin(phi)+tri.GetVertex(m).y*cos(phi);
            //z doesnt change
            tri.MorphVertex_X(m,tmp.x);
            tri.MorphVertex_Y(m,tmp.y);
        }
    }
    //rotation 2

    //float ymag = sqrt((vectorhead.x*vectorhead.x) + (vectorhead.z*vectorhead.z));
    float theta = atan(vectorhead.z/vectorhead.x);
    if(vectorhead.x >= 0.0f)
    {
        if(vectorhead.z > 0.0f)
        {
            theta = -theta;
        }
        else
        {
            theta = 1.0f*(theta);
        }       
    }
    else if (vectorhead.x < 0.0f)
    {
        if(vectorhead.z > 0.0f)
        {
            theta = theta;
        }
        else
        {
            theta = -1.0f*(PI - theta);
        }
    }


    //move the vector
    tmp.x = vectorhead.x*cos(theta)+vectorhead.z*sin(theta);
    tmp.z = vectorhead.z*cos(theta)-vectorhead.x*sin(theta);
    vectorhead.x = tmp.x;
    vectorhead.z = tmp.z;
    printf("\nrotation 2: %0.3f %0.3f %0.3f\n",vectorhead.x,vectorhead.y,vectorhead.z);

    //move the rest
    for (size_t l=0; l<mesh.size(); l++)
    {
        Triangle &tri = mesh[l];
        for (int m = 0; m<3; m++) //rotate, keep y constant
        {
            tmp.x = tri.GetVertex(m).x*cos(theta)+tri.GetVertex(m).z*sin(theta);
            tmp.z = tri.GetVertex(m).z*cos(theta)-tri.GetVertex(m).x*sin(theta);
            //y doesnt change
            tri.MorphVertex_X(m,tmp.x);
            tri.MorphVertex_Z(m,tmp.z);
        }
    }
    
    //printf("%f %f",vectorhead.x,zmag);
    //printf("\nrotation 2: %0.3f %0.3f %0.3f\n",vectorhead.x,vectorhead.y,vectorhead.z);
}   

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Move the TriangleMesh's Bounding Box's Center of Gravity
--| Args:
--|     center - Point of which to center the model around
--| Return:
--|     none
--|-------------------------------------------------------------------------
*/
void TriangleMesh::BBoxMoveCOG(point center)
{
    point half = ((BBox_Two - BBox_One)/2.0f)+BBox_One;
    point distance = half - center; //negative value for positive movement
    point dist = distance *-1.0f;

    for (size_t i=0; i<mesh.size(); i++) //For all triangles in the mesh
    {
        Triangle &tri = mesh[i]; //Get a handle on one of the triangles
        tri.MoveTriangle(distance); //Move that triangle
        BBoxRecalibrate(tri);
    }
    printf("\nBounding Box Lower Bound: %f, %f, %f \nBounding Box Upper Bound: %f, %f, %f \n",BBox_One.x,BBox_One.y,BBox_One.z,BBox_Two.x,BBox_Two.y,BBox_Two.z);
    printf("\nCenter of Bounding Box: %f, %f, %f \n",half.x,half.y,half.z);
    printf("\nDistance to move center point: %f, %f, %f \n\n",dist.x,dist.y,dist.z);
    printf("\nAligned around point %f, %f, %f!\n\n",center.x,center.y,center.z);

}

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
void TriangleMesh::BBoxRecalibrate(const Triangle& tri)
{
    // For each vertex of the triangle
    for (size_t vertex=0; vertex<3; vertex++)
    {
            // Check if the triangle's x is inside BBox Point One
            if (tri.GetVertex(vertex).x < BBox_One.x) 
            {
                BBox_One.x = tri.GetVertex(vertex).x;
            }
            
            // Check if the triangle's y is inside BBox Point One
            if (tri.GetVertex(vertex).y < BBox_One.y)
            {
                BBox_One.y = tri.GetVertex(vertex).y;
            }
            
            // Check if the triangle's z is inside BBox Point One
            if (tri.GetVertex(vertex).z < BBox_One.z) 
            {
                BBox_One.z = tri.GetVertex(vertex).z;
            }
            
            // Check if the triangle's x is inside BBox Point Two
            if (tri.GetVertex(vertex).x > BBox_Two.x)
            {
                BBox_Two.x = tri.GetVertex(vertex).x;
            }
            
            // Check if the triangle's y is inside BBox Point Two
            if (tri.GetVertex(vertex).y > BBox_Two.y) 
            {
                BBox_Two.y = tri.GetVertex(vertex).y;
            }
            
            // Check if the triangle's z is inside BBox Point Two
            if (tri.GetVertex(vertex).z > BBox_Two.z) 
            {
                BBox_Two.z = tri.GetVertex(vertex).z;
            }
    }
}
