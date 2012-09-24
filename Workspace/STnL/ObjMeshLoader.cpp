
/**
 * The MIT License
 * 
 * Copyright (c) 2010 Wouter Lindenhof (http://limegarden.net)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "Mesh.h"
#include "Vertex.h"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#define TOKEN_VERTEX_POS "v"
#define TOKEN_VERTEX_NOR "vn"
#define TOKEN_VERTEX_TEX "vt"
#define TOKEN_FACE "f"

namespace ObjLoader
{

struct Vector2f{
    float x, y;
};
struct Vector3f{
    float x, y, z;
};

//struct ObjMeshVertex{
//    Vector3f pos;
//    Vector2f texcoord;
//    Vector3f normal;
//};

/* This is a triangle, that we can render */
//struct ObjMeshFace{
//    ObjMeshVertex vertices[3];
//};

/* This contains a list of triangles */
//struct ObjMesh{
//    std::vector<ObjMeshFace> faces;
//};

/* Internal structure */
struct _ObjMeshFaceIndex{
    int pos_index[3];
    int tex_index[3];
    int nor_index[3];
};

/* Call this function to load a model, only loads triangulated meshes */
void LoadObjMesh(std::string filename, Mesh* mesh)
{
    //ObjMesh myMesh;

    std::vector<Vector3f>           positions;
    std::vector<Vector2f>           texcoords;
    std::vector<Vector3f>           normals;
    std::vector<_ObjMeshFaceIndex>  faces;
    /**
     * Load file, parse it
     * Lines beginning with: 
     * '#'  are comments can be ignored
     * 'v'  are vertices positions (3 floats that can be positive or negative)
     * 'vt' are vertices texcoords (2 floats that can be positive or negative)
     * 'vn' are vertices normals   (3 floats that can be positive or negative)
     * 'f'  are faces, 3 values that contain 3 values which are separated by / and <space>
     */

    std::ifstream filestream;
    filestream.open(filename.c_str());

	std::string line_stream;	// No longer depending on char arrays thanks to: Dale Weiler
	while(std::getline(filestream, line_stream)){	
		std::stringstream str_stream(line_stream);
		std::string type_str;
        str_stream >> type_str;
        if(type_str == TOKEN_VERTEX_POS){
            Vector3f pos;
            str_stream >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }else if(type_str == TOKEN_VERTEX_TEX){
            Vector2f tex;
            str_stream >> tex.x >> tex.y;
            texcoords.push_back(tex);
        }else if(type_str == TOKEN_VERTEX_NOR){
            Vector3f nor;
            str_stream >> nor.x >> nor.y >> nor.z;
            normals.push_back(nor);
        }else if(type_str == TOKEN_FACE){
            _ObjMeshFaceIndex face_index;
            char interupt;
            for(int i = 0; i < 3; ++i){
                str_stream >> face_index.pos_index[i] >> interupt 
                           >> face_index.tex_index[i]  >> interupt 
                           >> face_index.nor_index[i];
            }
            faces.push_back(face_index);
        }
    }
	// Explicit closing of the file 
    filestream.close();

	unsigned nVerts = positions.size();

	std::vector<Vertex> vertexList(nVerts);
	for (unsigned i = 0; i < vertexList.size(); i++)
	{
		vertexList[i].position.x = positions[i].x;
		vertexList[i].position.y = positions[i].y;
		vertexList[i].position.z = positions[i].z;

		vertexList[i].texCoord.x = 999.99f;
		vertexList[i].texCoord.y = 999.99f;
	}
	
	IndexBuffer& ib = *(mesh->GetIndexBuffer());
	ib.Initialize(faces.size() * 3);

	unsigned j = 0;
	for (unsigned i = 0; i < faces.size(); i++)
	{
		ib[j + 0] = faces[i].pos_index[0] - 1;
		ib[j + 1] = faces[i].pos_index[1] - 1;
		ib[j + 2] = faces[i].pos_index[2] - 1;

		if (vertexList[ib[j + 0]].texCoord.x != 999.99f || vertexList[ib[j + 0]].texCoord.y != 999.99f)
		{
			vertexList.push_back(vertexList[ib[j + 0]]);
			ib[j + 0] = vertexList.size() - 1;
		}

		if (vertexList[ib[j + 1]].texCoord.x != 999.99f || vertexList[ib[j + 1]].texCoord.y != 999.99f)
		{
			vertexList.push_back(vertexList[ib[j + 1]]);
			ib[j + 1] = vertexList.size() - 1;
		}

		if (vertexList[ib[j + 2]].texCoord.x != 999.99f || vertexList[ib[j + 2]].texCoord.y != 999.99f)
		{
			vertexList.push_back(vertexList[ib[j + 2]]);
			ib[j + 2] = vertexList.size() - 1;
		}

		vertexList[ib[j + 0]].texCoord.x = texcoords[faces[i].tex_index[0] - 1].x;
		vertexList[ib[j + 0]].texCoord.y = texcoords[faces[i].tex_index[0] - 1].y;

		vertexList[ib[j + 1]].texCoord.x = texcoords[faces[i].tex_index[1] - 1].x;
		vertexList[ib[j + 1]].texCoord.y = texcoords[faces[i].tex_index[1] - 1].y;

		vertexList[ib[j + 2]].texCoord.x = texcoords[faces[i].tex_index[2] - 1].x;
		vertexList[ib[j + 2]].texCoord.y = texcoords[faces[i].tex_index[2] - 1].y;

		vertexList[ib[j + 0]].normal.x = normals[faces[i].nor_index[0] - 1].x;
		vertexList[ib[j + 0]].normal.y = normals[faces[i].nor_index[0] - 1].y;
		vertexList[ib[j + 0]].normal.z = normals[faces[i].nor_index[0] - 1].z;

		vertexList[ib[j + 1]].normal.x = normals[faces[i].nor_index[1] - 1].x;
		vertexList[ib[j + 1]].normal.y = normals[faces[i].nor_index[1] - 1].y;
		vertexList[ib[j + 1]].normal.z = normals[faces[i].nor_index[1] - 1].z;

		vertexList[ib[j + 2]].normal.x = normals[faces[i].nor_index[2] - 1].x;
		vertexList[ib[j + 2]].normal.y = normals[faces[i].nor_index[2] - 1].y;
		vertexList[ib[j + 2]].normal.z = normals[faces[i].nor_index[2] - 1].z;

		j += 3;
	}

	VertexBuffer& vb = *(mesh->GetVertexBuffer());
	vb.Initialize(vertexList.size());

	for (unsigned i = 0; i < vertexList.size(); i++)
	{
		vb[i]= vertexList[i];
	}

    /*for(size_t i = 0; i < faces.size(); ++i){
        ObjMeshFace face;
        for(size_t j = 0; j < 3; ++j){
            face.vertices[j].pos        = positions[faces[i].pos_index[j] - 1];
            face.vertices[j].texcoord   = texcoords[faces[i].tex_index[j] - 1];
            face.vertices[j].normal     = normals[faces[i].nor_index[j] - 1];
        }
        myMesh.faces.push_back(face);
    }*/

    //return myMesh;
}

}
