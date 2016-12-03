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
#include "src/Angel.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <typeinfo>
#include "src/irrKlang/irrKlang.h"

#include "src/objloader.h"
#include "src/object.h"
#include "src/camera.h"
#include "src/utilities.h"
#include "src/GenerationStuff/floor.h"

using namespace std;
using namespace irrklang;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// start the sound engine with default parameters
ISoundEngine* SOUND_ENGINE;

//
// GLOBAL CONSTANTS
//
bool player_placed = false;
int playerX;
int playerZ;

// OBJECTS IN SCENE
Object *Cube;
Object *floor_tile;
Object *Player;
Object *StairsUp;
Object *StairsDown;
Object *PlaceholderObject; 

//Objects
Object *Object1;
Object *Object2;
Object *Object3;
Object *Object4;
Object *Object5;

Floor lvl_floor;
float fx=0.0, fz=0.0;

// Window dimension constants
int WIN_W = 1024;
int WIN_H = 768;

// Array for keyboard values
bool key[255];
bool DEBUG_CAM = false;

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
Camera camera(vec4(playerX, 10.0f, playerZ*1.0, 0.0f), 70.0f, (float)WIN_W/(float)WIN_H, 0.1f, 100.0f);
float camera_speed = 0.5f;
float camera_rotate_speed = (M_PI/180) * 0.5;

//
// CALLBACKS
//

void updateFound(int x, int z)
{
	if(x >= 0)
		//if(x < lvl_floor.x_dim)
			if(z >= 0){
				//if(z < lvl_floor.y_dim){
					cout << "Updating " << x << "," << z << "to found. " << endl;
					lvl_floor.floor_map[z][x].is_found = 1; 
				}
}

void updateCluster(int x, int z)
{
	updateFound(x,z-1);
	updateFound(x-1,z-1);
	updateFound(x+1,z-1);
	updateFound(x,z);
	updateFound(x-1,z);
	updateFound(x+1,z);
	updateFound(x,z+1);
	updateFound(x-1,z+1);
	updateFound(x+1,z+1);
	updateFound(x+2,z);
	updateFound(x-2,z);
	updateFound(x,z+2);
	updateFound(x,z-2);
}

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
	// RENDER THE LEVEL
	for(int i=0; i < lvl_floor.floor_map.size(); ++i) {
		for(int j=0; j < lvl_floor.floor_map[i].size(); ++j) {
			Cube->Move(fx, 0.0, fz);
			floor_tile->Move(fx, 0.0, fz);
			Player ->Move(playerX*5,0.5,playerZ*5);
			StairsUp ->Move(fx, 0.0, fz);
			StairsDown ->Move(fx, -5.0, fz);
			PlaceholderObject ->Move(fx, 0.5, fz);
			Object1->Move(fx, 0.5, fz);
			Object2->Move(fx, 0.5, fz);
			Object3->Move(fx, 0.5, fz);
			Object4->Move(fx, 0.5, fz);
			Object5->Move(fx, 0.5, fz);
			block t_block = lvl_floor.floor_map[i][j];
			if((t_block.get_block_id() == 3 || t_block.get_block_id() == 1) && t_block.is_found == 1) {
				floor_tile->SetColor(0.4, 0.5, 0.3);
				floor_tile->DrawSolid();
				if(t_block.get_block_content_id() != 0)
				{
					if(t_block.get_block_content_id() == 1)
						Object1 -> DrawSolid();
					if(t_block.get_block_content_id() == 2)
						Object2 -> DrawSolid();
					if(t_block.get_block_content_id() == 3)
						Object3 -> DrawSolid();
					if(t_block.get_block_content_id() == 4)
						Object4 -> DrawSolid();
					if(t_block.get_block_content_id() == 5)
						Object5 -> DrawSolid();

				} 
				//	PlaceholderObject->DrawSolid();
				//}
			} else if(t_block.get_block_id() == 2 && t_block.is_found == 1) {
				floor_tile->SetColor(0.4, 0.5, 0.0);
				floor_tile->DrawSolid();
			} else if (t_block.get_block_id() == 10) {
				StairsUp -> SetColor(200.0/255.0, 200.0/255.0, 200.0/255.0);
				StairsUp -> DrawSolid();
				//Entry Stairs
				//Places player, may need to tweek later to move. 
				Player -> DrawSolid();
				if(player_placed == false)
				{
					//Get starting pos for player.
					playerX = fx/5;
					playerZ = fz/5;
					playerX = abs(playerX);
					playerZ = abs(playerZ);
					std:: cout << "Player placed at: " << playerX << "," << playerZ << std::endl;
					player_placed = true;
					camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
					updateCluster(playerX,playerZ);
					camera.SetPitch(-45.0);
					camera.Rotate(90.0);
				}
			} else if (t_block.get_block_id() == 11 && t_block.is_found == 1) {
				//Exit Stairs
				StairsDown -> SetColor(200.0/255.0, 200.0/255.0, 200.0/255.0);
				StairsDown -> DrawSolid();
			} else { // draw the walls
				if(t_block.is_found == 1){
					Cube->SetColor(220.0/255.0, 220.0/255.0, 150.0/255.0);
					Cube->DrawSolid();
				}
			}
			fx += 5.0;
		}
		fz += 5.0;
		fx = 0.0;
	}
	fz = 0.0;
	glutSwapBuffers();
}

//X AND Z GET SWAPPED WHEN RENDERING, WHAT IS SHOWING IS NOT HOW IT IS IN DATA. 

void makeNextFloor()
{
	Floor new_floor;
	playerX = 1;
	playerZ = 1;
	player_placed = false; 
	lvl_floor = new_floor;
}

bool checkCol(int toCheck)
{

	if(toCheck == 11)
	{
		std::cout << "Exit Found!" << std:: endl;
		makeNextFloor();
	}

	std:: cout << "toCheck = " << toCheck << std::endl;
	if(toCheck != 0) //if its a floor tile
		return true; //can move
	return false;
}

extern "C" void SpecialKeys(int key, int x, int y)
{
	switch(key)
	{

		case GLUT_KEY_UP:
			std:: cout << "Move player up. " << std::endl;
			if(checkCol(lvl_floor.floor_map[playerZ][playerX -1].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerX = playerX - 1;
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
			}
			camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
			std:: cout << "Player at : " << playerX << "," << playerZ << std:: endl;
			std:: cout << "Floor type under player is: " << lvl_floor.floor_map[playerX][playerZ].get_block_id() << std:: endl;
			break;
		case GLUT_KEY_LEFT:
			std:: cout << "Move player left. " << std::endl;
			if(checkCol(lvl_floor.floor_map[playerZ +1][playerX].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerZ = playerZ + 1; 
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
			}
				camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
				std:: cout << "Player at : " << playerX << "," << playerZ << std:: endl;
				std:: cout << "Floor type under player is: " << lvl_floor.floor_map[playerX][playerZ].get_block_id() << std:: endl;
			break;
		case GLUT_KEY_RIGHT:
			std:: cout << "Move player right. " << std::endl;
			if(checkCol(lvl_floor.floor_map[playerZ -1][playerX].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerZ = playerZ - 1;
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
			}
			camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
			break;
		case GLUT_KEY_DOWN:
			std:: cout << "Move player down. " << std::endl;
			if(checkCol(lvl_floor.floor_map[playerZ][playerX +1].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerX = playerX + 1;
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
			}
			camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
			std:: cout << "Player at : " << playerX << "," << playerZ << std:: endl;
			break;	
	}

	glutPostRedisplay();

}

extern "C" void keyDown(unsigned char k, int nx, int ny) {

	key[k] = true;

	switch(k) {
		case 27: // Exit program case
			exit(0);
			break;
		case 't':
		case 'T':
			DEBUG_CAM = !DEBUG_CAM;
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
	glutPostRedisplay();
}

extern "C" void motion(int x, int y) {
	static bool just_warped = false;
	if(just_warped) {
		just_warped = false;
		return;
	}

	if(DEBUG_CAM) {
		int dx = x - WIN_W/2;
		int dy = y - WIN_H/2;
		if(dx) { // get rotation in the x direction
		    camera.RotateYaw(-camera_rotate_speed*dx);
		}
		if(dy) {
			camera.RotatePitch(-camera_rotate_speed*dy);
		}
	
		glutWarpPointer(WIN_W/2, WIN_H/2);
	
		just_warped = true;
	}
}

void GLUTinit() {	
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |  GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glutInitWindowSize(WIN_W, WIN_H);
	glutInitWindowPosition(20,20);

	glutCreateWindow("Rogue Game");

	/* CALLBACKS */
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(SpecialKeys);

	glutMouseFunc (mouse);
	glutMotionFunc (motion);
	glutPassiveMotionFunc (motion);
}

void initSound() {

	// Start the sound engine
	SOUND_ENGINE = createIrrKlangDevice();

	// Error check that the sound engine is working
	if (!SOUND_ENGINE) {
		printf("Could not startup engine\n");
		exit(0); // error starting up the engine
	}


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
	Cube = new Object("models/cube_5unit.obj", 0, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Cube->GetVertices());
	Cube->SetColor(1.0, 0.0, 0.0);

	floor_tile = new Object("models/plane_5unit.obj", Cube->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, floor_tile->GetVertices());
	floor_tile->SetColor(1.0, 1.0, 1.0);
	
	Player = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Player->GetVertices());
	Player -> SetColor(1.0,0.0,0.0);

	StairsUp = new Object("models/StairsUp.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, StairsUp->GetVertices());

	StairsDown = new Object("models/StairsUp.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, StairsDown->GetVertices());

	//default object id, used 
	PlaceholderObject = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size() + StairsDown->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, PlaceholderObject->GetVertices());
	PlaceholderObject -> SetColor(0.0,0.0,0.5);


	Object1 = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size() + StairsDown->GetVertices().size() + PlaceholderObject ->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object1->GetVertices());
	Object1 -> SetColor(0.0,0.3,0.4);

	Object2 = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size() + StairsDown->GetVertices().size() + PlaceholderObject ->GetVertices().size() + Object1->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object2->GetVertices());
	Object2 -> SetColor(0.0,0.5,0.6);

	Object3 = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size() + StairsDown->GetVertices().size() + PlaceholderObject ->GetVertices().size() + Object1->GetVertices().size() + Object2->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object3->GetVertices());
	Object3 -> SetColor(0.0,0.7,0.8);

	Object4 = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size() + StairsDown->GetVertices().size() + PlaceholderObject ->GetVertices().size() + Object1->GetVertices().size() + Object2->GetVertices().size() + Object3->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object4->GetVertices());
	Object4 -> SetColor(0.0,0.9,1.0);

	Object5 = new Object("models/cube.obj", Cube->GetVertices().size() + floor_tile->GetVertices().size() + Player->GetVertices().size() + StairsUp->GetVertices().size() + StairsDown->GetVertices().size() + PlaceholderObject ->GetVertices().size() + Object1->GetVertices().size() + Object2->GetVertices().size() + Object3->GetVertices().size() + Object4->GetVertices().size(), colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object5->GetVertices());
	Object5 -> SetColor(0.0,0.9,1.0);


	// Initialization of all vertices
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);


	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0, 0.0, 0.0, 1.0); // white background
	camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
	camera.SetDir(vec4(0.0,0.0,1.0,0.0));
	camera.SetDirToForward();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);

	// Initializes the GLUT and callbacks 
	GLUTinit();

	glewInit();

	// Initializes the buffers and vao
	initSound();
	init();

	glEnable(GL_DEPTH_TEST);

	glutMainLoop(); // enter event loop

	return (EXIT_SUCCESS);
}