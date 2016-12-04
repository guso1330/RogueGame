//******************************************************************* 
//                                                                    
//  Class:     Mesh - contains the vertex class
//                                                                     
//  Author:      Gus Oberdick
//  Email:       go125412@ohio.edu
//                                                                    
//                                                                    
//  Description: This class builds a simple mesh object that reads
//				 in vertices from a .obj file.
//                                                                    
//  Date:        October 27, 2016 3:41:00pm
//                                                                    
//*******************************************************************
#ifndef MESH_H
#define MESH_H

#include "Angel.h"
#include <iostream>
#include <vector>
#include "objloader.h"

enum MeshBufferPositions {
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB
};

class Mesh {
	public:
		// CONSTRUCTORs
		Mesh(); // default constructor
		Mesh(const char *filename); // Parametric constructor
		virtual ~Mesh();

		// Manipulator Functions
		void InitMesh(const char *filename);

		// Accessor functions
		inline vector<vec4> GetVertices() const { return Vertices; }
		inline unsigned int GetVerticesSize() const { return Vertices.size(); }
		inline vector<vec2> GetUVs() const { return UVs; }
		inline unsigned int GetUVsSize() const { return UVs.size(); }
		inline vector<vec4> GetNormals() const { return Normals; }
		inline unsigned int GetNormalsSize() const { return Normals.size(); }

		// Virtual Functions
		virtual void DrawSolid() = 0;
		virtual void DrawWireframe() = 0;

		// Debug functions
		void PrintVertices() {
			for(size_t i=0; i<Vertices.size(); ++i) {
				cout << Vertices[i] << " ";
			}
			cout << endl;
		}

		void PrintUVs() {
			for(size_t i=0; i<UVs.size(); ++i) {
				cout << UVs[i] << " ";
			}
			cout << endl;
		}

	private:
		void operator=(const Mesh& mesh) {}
		Mesh(const Mesh& mesh) {}

	protected:
		std::vector<vec4> Vertices;
		std::vector<vec2> UVs;
		std::vector<vec4> Normals;

		static const unsigned int NUM_BUFFERS = 3; // Instantiate how many buffers we'll be using
		GLuint MyVAO;
		GLuint MyVertexArrayBuffers[NUM_BUFFERS];
		unsigned int MyNumIndices;
};

#endif // MESH_H