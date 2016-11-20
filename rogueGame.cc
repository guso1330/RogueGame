//******************************************************************* 
//                                                                    
//  Program:     Rogue Game                                        
//                                                                     
//  Author:      Gus Oberdick
//  Email:       go125412@ohio.edu
//                                                                    
//                                                                    
//  Description: // Fill int                    
//                                                                    
//  Date:        November 14, 2016
//                                                                    
//*******************************************************************

// TO DO:
//	- Watch thebennybox - Intro to Modern OpenGL Tutorial #6: Camera and Perspective	
//		- There's also a guy name Jamie King who does good opengl tutorials
//

#include <iostream>
#include "Angel.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <typeinfo>

#include "src/objloader.h"
#include "src/object.h"
#include "src/camera.h"
#include "src/utilities.h"

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

//
// GLOBAL CONSTANTS
//

// OBJECTS IN SCENE
Object *Cube;

// Window dimension constants
int WIN_W = 1024;
int WIN_H = 768;

// Array for keyboard values
bool key[255];

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};

// Containers for vertices and normals
vector<vec4> vertices;
vector<vec2> uvs;
vector<vec4> normals;

// Uniform variables
GLuint program;
GLuint loc;
GLint matrix_loc, projection_loc;

// Declaring the projection and model view
mat4 model_view;
mat4 projection;

// Initialize the camera
Camera camera(vec4(0.0f, 0.0f, -2.0f, 0.0f), 70.0f, (float)WIN_W/(float)WIN_H, 0.1f, 100.0f);
float camera_speed = 0.5f;
float camera_rotate_speed = (M_PI/180) * 2.5;

//
// CALLBACKS
//
extern "C" void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up camera
	projection = camera.GetViewProjection();
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, model_view);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	ControlCamera(camera, key, camera_speed, camera_rotate_speed);
	camera.Update();

	//
	// DRAWING
	//
	Cube->DrawWireframe();

	glutSwapBuffers();
}

extern "C" void keyDown(unsigned char k, int nx, int ny) {

	key[k] = true;

	switch(k) {
		case 27: // Exit program case
			exit(0);
			break;

		default:
			break;
	}

	// Something might have changed requiring redisplay
	glutPostRedisplay();
}

extern "C" void keyUp(unsigned char k, int x, int y) {
	key[k] = false;
}

extern "C" void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch(button) {
    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
    }
  }
}
extern "C" void idle()
{
	// static GLint time=glutGet(GLUT_ELAPSED_TIME);
	// Theta[Axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
	// time = glutGet(GLUT_ELAPSED_TIME);

	// if (Theta[Axis] > 360.0) {
	// 	Theta[Axis] -= 360.0;
	// }

	glutPostRedisplay();
}

void GLUTinit() {	
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIN_W, WIN_H);
	glutInitWindowPosition(20,20);

	glutCreateWindow("Rogue Game");

	/* CALLBACKS */
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutMouseFunc (mouse);

	// glutMotionFunc (motion);
	// glutPassiveMotionFunc (passivemotion);

	// glutReshapeFunc (myReshape);
}

void init() {

	GLint colorLoc;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	// get shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	colorLoc = glGetUniformLocation(program, "vcolor");
	if(colorLoc == -1) { // Handle some error checking
		std::cerr << "Unable to find the colorLoc parameter" << std::endl;
	}

	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	matrix_loc = glGetUniformLocation(program, "model_view");
	projection_loc = glGetUniformLocation(program, "projection");

	//
	// Build all objects in scene
	//
	Cube = new Object("models/cube.obj", 0, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Cube->GetVertices());
	Cube->Move(0.0, 0.0, 10.0);
	Cube->SetColor(1.0, 0.0, 0.0);
	
	// Initialization of all vertices
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);

	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0, 0.0, 0.0, 1.0); // white background
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);

	// load_obj("models/pipe.obj", vertices, uvs, normals);
	
	// Initializes the GLUT and callbacks 
	GLUTinit();

	glewInit();

	// Initializes the buffers and vao	
	init();

	glEnable(GL_DEPTH_TEST);

	glutMainLoop(); // enter event loop

	return (EXIT_SUCCESS);
}