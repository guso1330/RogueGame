#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h"
#include <iostream>

class Object : public Mesh {
	public:
		Object();
		Object(const char *filename, GLuint program, GLint ncolorLoc, GLint nmatrix_loc);

		void Move(GLfloat nx, GLfloat ny, GLfloat nz); // point form
		void Move(vec4 where); // vector form
		void Rotate(int axis, GLfloat theta);
		void ChangeGoal(GLfloat nx, GLfloat ny, GLfloat nz);
		void ChangeGoal(vec3 npos);
		void Update();
		void DrawSolid();
		void DrawWireframe();

		// Set Functions
		inline void SetSpeed(GLfloat nspeed) { speed = nspeed; }
		inline void SetModelView(mat4 m) { ModelView = m; };
		inline void SetColor(GLfloat nr, GLfloat ng, GLfloat nb) { r = nr; g = ng; b = nb; }
		inline void SetColorAlpha(GLfloat nr, GLfloat ng, GLfloat nb, GLfloat na) { r = nr; g = ng; b = nb; a = na; }

		// Get Functions
		inline vec3 GetPos() const { return vec3(x, y, z); }
		inline GLfloat GetSpeed() const { return speed; }

	private:
		// GL Variables
		GLuint m_program;
		GLuint vert_loc;
		GLuint uv_loc;
		GLint matrix_loc;
		GLint colorLoc;
		mat4 ModelView;

		// Object Attributes
		GLfloat x, y, z;
		GLfloat r, g, b, a;
		GLfloat goal_x, goal_y, goal_z;
		GLfloat speed;
		GLint last_time;
};

#endif