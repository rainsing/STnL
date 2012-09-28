
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
#include "stdafx.h"
#include "Mesh.h"
#include "Vertex.h"

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

		vertexList[i].binormal = Vector3::ZERO;
		vertexList[i].tangent = Vector3::ZERO;
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

		Vertex& v0 = vertexList[ib[j + 0]];
		Vertex& v1 = vertexList[ib[j + 1]];
		Vertex& v2 = vertexList[ib[j + 2]];

		v0.texCoord.x = texcoords[faces[i].tex_index[0] - 1].x;
		v0.texCoord.y = texcoords[faces[i].tex_index[0] - 1].y;

		v1.texCoord.x = texcoords[faces[i].tex_index[1] - 1].x;
		v1.texCoord.y = texcoords[faces[i].tex_index[1] - 1].y;

		v2.texCoord.x = texcoords[faces[i].tex_index[2] - 1].x;
		v2.texCoord.y = texcoords[faces[i].tex_index[2] - 1].y;

		v0.normal.x = normals[faces[i].nor_index[0] - 1].x;
		v0.normal.y = normals[faces[i].nor_index[0] - 1].y;
		v0.normal.z = normals[faces[i].nor_index[0] - 1].z;

		v1.normal.x = normals[faces[i].nor_index[1] - 1].x;
		v1.normal.y = normals[faces[i].nor_index[1] - 1].y;
		v1.normal.z = normals[faces[i].nor_index[1] - 1].z;

		v2.normal.x = normals[faces[i].nor_index[2] - 1].x;
		v2.normal.y = normals[faces[i].nor_index[2] - 1].y;
		v2.normal.z = normals[faces[i].nor_index[2] - 1].z;

		// 计算切线和副法线
		float x1 = v1.position.x - v0.position.x;
		float x2 = v2.position.x - v0.position.x;
		float y1 = v1.position.y - v0.position.y;
		float y2 = v2.position.y - v0.position.y;
		float z1 = v1.position.z - v0.position.z;
		float z2 = v2.position.z - v0.position.z;

		float _u1 = v1.texCoord.x - v0.texCoord.x;
		float _u2 = v2.texCoord.x - v0.texCoord.x;
		float _v1 = v1.texCoord.y - v0.texCoord.y;
		float _v2 = v2.texCoord.y - v0.texCoord.y;

		if (_u1 * _v2 - _u2 * _v1 == 0.0f)
		{
			j += 3;
			continue;
		}

		float r = 1.0f / (_u1 * _v2 - _u2 * _v1);
		Vector3 uDir((_v2 * x1 - _v1 * x2) * r, (_v2 * y1 - _v1 * y2) * r,(_v2 * z1 - _v1 * z2) * r);
		Vector3 vDir((_u1 * x2 - _u2 * x1) * r, (_u1 * y2 - _u2 * y1) * r,(_u1 * z2 - _u2 * z1) * r);

		v0.binormal = v0.binormal + vDir;
		v1.binormal = v1.binormal + vDir;
		v2.binormal = v2.binormal + vDir;

		v0.tangent = v0.tangent - uDir;
		v1.tangent = v1.tangent - uDir;
		v2.tangent = v2.tangent - uDir;

		j += 3;
	}

	VertexBuffer& vb = *(mesh->GetVertexBuffer());
	vb.Initialize(vertexList.size());

	for (unsigned i = 0; i < vertexList.size(); i++)
	{
		// 对normal, binormal和tangent做Gram-Schmidt正交化处理
		// --begin--
		Vector3& normal = vertexList[i].normal;

		Vector3& binormal = vertexList[i].binormal;
		binormal = binormal - normal * binormal.Dot(normal);

		// 整出这种UV来还指望光照能正确么？！
		if (binormal.Equal(Vector3::ZERO, 0.0001f))
		{
			binormal.x = 1.0f;
		}

		binormal.Normalize();

		// TODO: 这里可能会有handedness的问题 想想春春是怎么解决NCIS的normal map问题的
		Vector3& tangent = vertexList[i].tangent;

		Vector3& d1 = tangent - normal * tangent.Dot(normal);
		Vector3& d2 = tangent - binormal * tangent.Dot(binormal);
		tangent = tangent - d1;
		tangent = tangent - d2;

		// 整出这种UV来光照肯定正确不了啊！
		if (tangent.Equal(Vector3::ZERO, 0.0001f))
		{
			tangent.y = 1.0f;
		}

		tangent.Normalize();

		// 对normal, binormal和tangent做Gram-Schmidt正交化处理
		// --end--

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
