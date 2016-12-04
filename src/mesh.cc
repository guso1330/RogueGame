// Implementation file for the Mesh Class
#include "mesh.h"

// Default
Mesh::Mesh() {}

void Mesh::InitMesh(const char *filename) {
	load_obj(filename, Vertices, UVs, Normals);

	MyNumIndices = Vertices.size();

	// VAO creation
	glGenVertexArrays(1, &MyVAO);
	glBindVertexArray(MyVAO);

	// Handle the different buffers
	glGenBuffers(1, MyVertexArrayBuffers);

	// Instantiating the different buffers
	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, MyVertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(vec4), &Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// UV buffer
	glBindBuffer(GL_ARRAY_BUFFER, MyVertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, UVs.size()*sizeof(vec2), &UVs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, MyVertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size()*sizeof(vec4), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0); // Re-initialize after sending data
}

Mesh::~Mesh() {
	glDeleteBuffers(NUM_BUFFERS, MyVertexArrayBuffers); // Delete the Buffers
	glDeleteVertexArrays(1, &MyVAO); // Delete VAO
}

// Parametric Constructor
Mesh::Mesh(const char *filename) {
	InitMesh(filename);
}