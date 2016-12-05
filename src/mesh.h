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

class Mesh {
	public:
		// CONSTRUCTORs
		Mesh(); // default constructor
		Mesh(const char *filename); // Parametric constructor

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

	protected:
		std::vector<vec4> Vertices;
		std::vector<vec2> UVs;
		std::vector<vec4> Normals;
};

#endif // MESH_H