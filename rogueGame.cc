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
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION // needed to use the stb_image library
#include "src/stb_image/stb_image.h"
// #include "src/texture.h"
#include "src/soundengine.h"
#include "src/objloader.h"
#include "src/object.h"
#include "src/camera.h"
#include "src/utilities.h"
#include "src/GenerationStuff/floor.h"

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// start the sound engine with default parameters
// ISoundEngine* SOUND_ENGINE;
SoundEngine sound;

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
Object *HostileUnit;
Object *light1;
bool canMove = 1 ;

//Objects
Object *Object1;
Object *Object2;
Object *Object3;
Object *Object4;
Object *Object5;

int NUMVERTICES = 0;

Floor lvl_floor;
float fx=0.0, fz=0.0;

// Window dimension constants
int WIN_W = 1024;
int WIN_H = 768;
float ASPECT_RATIO = 1.0*((float)WIN_W/(float)WIN_H);

// Array for keyboard values
bool key[255];
bool DEBUG_CAM = false;

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};

// Containers for the game
vector<vec4> vertices;
vector<vec2> uvs;
vector<vec4> normals;
vector<vec4> tangents;
vector<vec4> bitangents;

// TEXTURE VARIABLES
vector<GLuint> Textures; // Container to hold all of the textures
GLuint texBufferID;
GLuint texID;

vector<GLuint> NormalTextures;
GLuint normalTexID;

// LIGHT VARIABLES
GLint lightPositionWorldSpaceLoc;
vec4 lightPos;

// Uniform variables
GLuint program;
GLuint loc;
GLuint uv_loc;
GLuint normal_loc;
GLuint tangents_loc;
GLuint bitangents_loc;
GLint matrix_loc, projection_loc;

// Declaring the projection and model view
mat4 model_view;
mat4 projection;

// Initialize the camera
float FOV = 70.0f;
float NEAR = 0.1f;
float FAR = 1000.0f;
Camera camera(vec4(playerX, 10.0f, playerZ*1.0, 0.0f), FOV, ASPECT_RATIO, NEAR, FAR);
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
					//cout << "Updating " << x << "," << z << "to found. " << endl;
					lvl_floor.floor_map[z][x].is_found = 1; 
				}
}

void updateCluster(int x, int z)
{
	//
	//       |(+1,-1)|(+1)|
	//( 0,-2)|( 0,-1)|( 0, 0)|( 0,+1)|(0,+2)
	//
	//
	//

	//update block unit is on
	updateFound(x,z);

	//update diagonals
	updateFound(x-1,z-1);
	updateFound(x+1,z-1);
	updateFound(x-1,z+1);
	updateFound(x+1,z+1);

	//update vertical & horizontal with the condition that theres no wall in the way for the
	//farthest ones. Basic Line of Sight functionality. 

	updateFound(x,z-1);
	if(lvl_floor.floor_map[z-1][x].get_block_id() != 0)
		updateFound(x,z-2);

	updateFound(x-1,z);
	if(lvl_floor.floor_map[z][x-1].get_block_id() != 0)
		updateFound(x-2,z);

	updateFound(x+1,z);
	if(lvl_floor.floor_map[z][x+1].get_block_id() != 0)
		updateFound(x+2,z);

	updateFound(x,z+1);
	if(lvl_floor.floor_map[z+1][x].get_block_id() != 0)
		updateFound(x,z+2);	
}

extern "C" void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up camera
	projection = camera.GetViewProjection();
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, model_view);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	ControlCamera(camera, key, camera_speed, camera_rotate_speed);
	camera.Update();

	// // UPDATE THE LIGHT POSITION BASED ON THE PLAYER POS
	// light1->DrawSolid();
	vec3 PlayerPos = Player->GetPos();
	lightPos.x=PlayerPos.x;
	lightPos.y=PlayerPos.y+7.0f;
	lightPos.z=PlayerPos.z;
	glUniform4f(lightPositionWorldSpaceLoc, lightPos.x, lightPos.y, lightPos.z, 0.0);

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
			HostileUnit->Move(fx, 0.5, fz);
			block t_block = lvl_floor.floor_map[i][j];
			if((t_block.get_block_id() == 3 || t_block.get_block_id() == 1) && t_block.is_found == 1) {
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
				if(t_block.get_has_hostile() == 1)
				{
					HostileUnit -> DrawSolid();
				}
				//	PlaceholderObject->DrawSolid();
				//}
			} else if(t_block.get_block_id() == 2 && t_block.is_found == 1) {
				floor_tile->DrawSolid();
			} else if (t_block.get_block_id() == 10) {
				// StairsUp -> SetColor(200.0/255.0, 200.0/255.0, 200.0/255.0);
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
					camera.Rotate(90);
				}
			} else if (t_block.get_block_id() == 11 && t_block.is_found == 1) {
				//Exit Stairs
				// StairsDown -> SetColor(200.0/255.0, 200.0/255.0, 200.0/255.0);
				StairsDown -> DrawSolid();
			} else { // draw the walls
				if(t_block.is_found == 1){
					// Cube->SetColor(220.0/255.0, 220.0/255.0, 150.0/255.0);
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

extern "C" void resize(int w, int h) {
	if(h == 0) {
		h = 1; // prevents a divide by zero
	}

	ASPECT_RATIO = 1.0*((float)w /(float)h);
	camera.SetProjection(Perspective(FOV, ASPECT_RATIO, NEAR, FAR));
	mat4 projection = camera.GetViewProjection();
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, model_view);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	camera.Update();

	glViewport(0, 0, w, h);

	glutPostRedisplay();
}

//X AND Z GET SWAPPED WHEN RENDERING, WHAT IS SHOWING IS NOT HOW IT IS IN DATA. 

void makeNextFloor()
{
	Floor new_floor;
	playerX = 1;
	playerZ = 1;
	player_placed = false; 
	//sleep(1);

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

void process_hostile_moves()
{
	std::cout << "Process all hostile unit movement." << std::endl;
	/*int num_hostiles = lvl_floor.hostile_unit_pos.size();
	int rx, ry;
	for(int i = 0; i < num_hostiles; ++i)
	{
		rx = rand() % 2;
		ry = rand() % 2; 
		if(checkCol(lvl_floor.floor_map[ lvl_floor.hostile_unit_pos[i].y_coord + ry][ lvl_floor.hostile_unit_pos[i].x_coord + rx].get_block_id()) == true){
			lvl_floor.move_enemy(lvl_floor.hostile_unit_pos[i].x_coord + rx,lvl_floor.hostile_unit_pos[i].y_coord + ry,i);
			
		}
	}*/
}

extern "C" void SpecialKeys(int key, int x, int y)
{
	switch(key)
	{

		case GLUT_KEY_UP:
			std:: cout << "Move player up. " << std::endl;
			sound.PlaySound("sounds/footsteps.wav");
			if(checkCol(lvl_floor.floor_map[playerZ][playerX -1].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerX = playerX - 1;
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
				process_hostile_moves();
			}
			camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
			std:: cout << "Player at : " << playerX << "," << playerZ << std:: endl;
			std:: cout << "Floor type under player is: " << lvl_floor.floor_map[playerX][playerZ].get_block_id() << std:: endl;
			
			break;
		case GLUT_KEY_LEFT:
			std:: cout << "Move player left. " << std::endl;
			sound.PlaySound("sounds/footsteps.wav");

			if(checkCol(lvl_floor.floor_map[playerZ +1][playerX].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerZ = playerZ + 1; 
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
				process_hostile_moves();
			}
				camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
				std:: cout << "Player at : " << playerX << "," << playerZ << std:: endl;
				std:: cout << "Floor type under player is: " << lvl_floor.floor_map[playerX][playerZ].get_block_id() << std:: endl;
				
			break;
		case GLUT_KEY_RIGHT:
			std:: cout << "Move player right. " << std::endl;
			sound.PlaySound("sounds/footsteps.wav");

			if(checkCol(lvl_floor.floor_map[playerZ -1][playerX].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerZ = playerZ - 1;
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
				process_hostile_moves();
			}
			camera.SetPos(vec4(playerX*5 +15.0f, 45.0f, playerZ*5, 0.0f));
			
			break;
		case GLUT_KEY_DOWN:
			std:: cout << "Move player down. " << std::endl;
			sound.PlaySound("sounds/footsteps.wav");

			if(checkCol(lvl_floor.floor_map[playerZ][playerX +1].get_block_id())){
				std::cout << "Can move" << std:: endl;
				playerX = playerX + 1;
				std::cout << "Did move" << std:: endl;
				updateCluster(playerX,playerZ);
				std::cout << "Updated local cluster" << std:: endl;
				process_hostile_moves();
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
		case 'o':
		case 'O':
			process_hostile_moves();
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
	glutFullScreen();

	/* CALLBACKS */
	glutDisplayFunc(display);
	// Here is our new entry in the main function
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(SpecialKeys);

	glutMouseFunc (mouse);
	glutMotionFunc (motion);
	glutPassiveMotionFunc (motion);
}

void initSound() {
	sound.PlayLoop("sounds/dungeon.wav");
}

GLuint loadImage(const std::string& filename) {
	int width, height, numComponents; // Image properties
	unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, &numComponents, 4); // Load the image

	if(image_data == NULL) { // error checking
		std::cerr << "Texture loading failed for texture: " << filename << std::endl;
	}
	GLuint texBufferID;
	glGenTextures(1, &texBufferID); // Generate space for the texture
	glBindTexture(GL_TEXTURE_2D, texBufferID);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	stbi_image_free(image_data);

	return texBufferID;
}

void loadTextures() {
	Textures.push_back(loadImage("textures/cartoon_floor_texture.jpg")); // Textures[0]
	Textures.push_back(loadImage("textures/Rock_02_UV_H_CM_1.jpg")); // Textures[1]
	Textures.push_back(loadImage("textures/wood_plank2.jpg")); // Textures[2]
	Textures.push_back(loadImage("textures/default_normal.jpg")); // Textures[3] DEFAULT NORMAL
	Textures.push_back(loadImage("textures/cartoon_floor_texture_nm.png")); // Textures[4] floor NORMAL
	Textures.push_back(loadImage("textures/Rock_02_UV_H_CM_1_nm.png")); // Textures[5] Wall normal
	Textures.push_back(loadImage("textures/wood_plank2_nm.png")); // Textures[6] Stair normal
}

void initObjects(GLuint tex_loc, GLuint nm_tex_loc, GLuint colorLoc, GLint matrix_loc) {
	//
	// Build all objects in scene
	//
	Cube = new Object("models/cube_5unit_allfaceuvs.obj", 0, tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Cube->GetVertices());
	combineVec2Vectors(uvs, Cube->GetUVs());
	combineVec4Vectors(normals, Cube->GetNormals());
	Cube->SetColorAlpha(0.0, 0.0, 0.0, 0.0);
	Cube->SetTexture(Textures[1]);
	Cube->SetNormalTexture(Textures[5]);

	floor_tile = new Object("models/plane_5unit.obj", incrementIndex(NUMVERTICES, Cube->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, floor_tile->GetVertices());
	combineVec2Vectors(uvs, floor_tile->GetUVs());
	combineVec4Vectors(normals, floor_tile->GetNormals());
	floor_tile->SetColorAlpha(0.0, 0.0, 0.0, 0.0);
	floor_tile->SetTexture(Textures[0]);
	floor_tile->SetNormalTexture(Textures[4]);
	
	Player = new Object("models/cube.obj", incrementIndex(NUMVERTICES, floor_tile->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Player->GetVertices());
	combineVec2Vectors(uvs, Player->GetUVs());
	combineVec4Vectors(normals, Player->GetNormals());
	Player -> SetColor(1.0,0.0,0.0);
	Player ->SetNormalTexture(Textures[3]);

	StairsUp = new Object("models/StairsUp.obj", incrementIndex(NUMVERTICES, Player->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, StairsUp->GetVertices());
	combineVec2Vectors(uvs, StairsUp->GetUVs());
	combineVec4Vectors(normals, StairsUp->GetNormals());
	StairsUp->SetColorAlpha(0.0, 0.0, 0.0, 0.0);
	StairsUp->SetTexture(Textures[2]);
	StairsUp->SetNormalTexture(Textures[6]);

	StairsDown = new Object("models/StairsUp.obj", incrementIndex(NUMVERTICES, StairsUp->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, StairsDown->GetVertices());
	combineVec2Vectors(uvs, StairsDown->GetUVs());
	combineVec4Vectors(normals, StairsDown->GetNormals());
	StairsDown->SetColorAlpha(0.0, 0.0, 0.0, 0.0);
	StairsDown->SetTexture(Textures[2]);
	StairsDown->SetNormalTexture(Textures[3]);

	//default object id, used 
	PlaceholderObject = new Object("models/cube.obj", incrementIndex(NUMVERTICES, StairsDown->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, PlaceholderObject->GetVertices());
	combineVec2Vectors(uvs, PlaceholderObject->GetUVs());
	combineVec4Vectors(normals, PlaceholderObject->GetNormals());
	PlaceholderObject -> SetColor(0.0,0.0,0.5);
	PlaceholderObject->SetNormalTexture(Textures[3]);


	Object1 = new Object("models/cube.obj", incrementIndex(NUMVERTICES, PlaceholderObject->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object1->GetVertices());
	combineVec2Vectors(uvs, Object1->GetUVs());
	combineVec4Vectors(normals, Object1->GetNormals());
	Object1 -> SetColor(0.0,0.3,0.4);
	Object1->SetNormalTexture(Textures[3]);

	Object2 = new Object("models/cube.obj", incrementIndex(NUMVERTICES,  Object1->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object2->GetVertices());
	combineVec2Vectors(uvs, Object2->GetUVs());
	combineVec4Vectors(normals, Object2->GetNormals());
	Object2 -> SetColor(0.0,0.5,0.6);
	Object2->SetNormalTexture(Textures[3]);

	Object3 = new Object("models/cube.obj", incrementIndex(NUMVERTICES, Object2->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object3->GetVertices());
	combineVec2Vectors(uvs, Object3->GetUVs());
	combineVec4Vectors(normals, Object3->GetNormals());
	Object3 -> SetColor(0.0,0.7,0.8);
	Object3->SetNormalTexture(Textures[3]);

	Object4 = new Object("models/cube.obj", incrementIndex(NUMVERTICES, Object3->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object4->GetVertices());
	combineVec2Vectors(uvs, Object4->GetUVs());
	combineVec4Vectors(normals, Object4->GetNormals());
	Object4 -> SetColor(0.0,0.9,1.0);
	Object4->SetNormalTexture(Textures[3]);

	Object5 = new Object("models/cube.obj", incrementIndex(NUMVERTICES, Object4->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, Object5->GetVertices());
	combineVec2Vectors(uvs, Object5->GetUVs());
	combineVec4Vectors(normals, Object5->GetNormals());
	Object5 -> SetColor(0.0,0.9,1.0);
	Object5->SetNormalTexture(Textures[3]);

	HostileUnit = new Object("models/cube.obj", incrementIndex(NUMVERTICES, Object5->GetVerticesSize()), tex_loc, nm_tex_loc, colorLoc, matrix_loc);
	combineVec4Vectors(vertices, HostileUnit->GetVertices());
	combineVec2Vectors(uvs, HostileUnit->GetUVs());combineVec4Vectors(normals, Cube->GetNormals());
	combineVec4Vectors(normals, HostileUnit->GetNormals());
	HostileUnit -> SetColor(1.0,1.0,0.0);
	HostileUnit->SetNormalTexture(Textures[3]);
}

// Function to grab all the tangents and bitangents for normal
// calculations
void getTangents(vector<vec4> &t, vector<vec4> &bt) {
	for ( int i=0; i<vertices.size(); i+=3){

		// Shortcuts for vertices
		vec4 &v0 = vertices[i+0];
		vec4 &v1 = vertices[i+1];
		vec4 &v2 = vertices[i+2];

		// Shortcuts for UVs
		vec2 &uv0 = uvs[i+0];
		vec2 &uv1 = uvs[i+1];
		vec2 &uv2 = uvs[i+2];

		// Edges of the triangle : postion delta
		vec4 deltaPos1 = v1-v0;
		vec4 deltaPos2 = v2-v0;

		// UV delta
		vec2 deltaUV1 = uv1-uv0;
		vec2 deltaUV2 = uv2-uv0;
		// We can now use our formula to compute the tangent and the bitangent :

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec4 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
		vec4 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
		// Finally, we fill the *tangents *and *bitangents *buffers. Remember, these buffers are not indexed yet, so each vertex has its own copy.

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for binormals
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

	}

	for (unsigned int i=0; i<vertices.size(); i+=1 )
	{
		vec4 &n = normals[i];
		vec4 &t = tangents[i];
		vec4 &b = bitangents[i];

		// Gram-Schmidt orthogonalize
		t = normalize(t - n * dot(n, t));

		// Calculate handedness
		if (dot(cross(n, t), b) < 0.0f){
			t = t * -1.0f;
		}
	}

}

void init() {


	loadTextures();

	GLint colorLoc;

	// get shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	
	/****************************************************************/
	/*						shader variables						*/
	/****************************************************************/
	loc = glGetAttribLocation(program, "vPosition");
	uv_loc = glGetAttribLocation(program, "vUV");
	normal_loc = glGetAttribLocation(program, "vNormal");
	tangents_loc = glGetAttribLocation(program, "vTangents");
	bitangents_loc = glGetAttribLocation(program, "vBitangents");

	
	colorLoc = glGetUniformLocation(program, "vcolor");
	if(colorLoc == -1) { // Handle some error checking
		std::cerr << "Unable to find the colorLoc parameter" << std::endl;
	}
	
	matrix_loc = glGetUniformLocation(program, "model_view");
	projection_loc = glGetUniformLocation(program, "projection");

	glEnable(GL_TEXTURE_2D);

	texID = glGetUniformLocation(program, "diffuse");
	if(texID == -1) {
		std::cerr << "Unable to find the texID parameter" << std::endl;
	}

	normalTexID = glGetUniformLocation(program, "normalTex");
	if(normalTexID == -1) {
		std::cerr << "Unable to find the normalTexID parameter" << std::endl;
	}

	/****************************************************************/
	/*						Light variables						*/
	/****************************************************************/
	lightPositionWorldSpaceLoc  = glGetUniformLocation(program, "lightPositionWorldSpace");
	if (lightPositionWorldSpaceLoc == -1) {
		std::cerr << "Unable to find lightPositionWorldSpace parameter" << std::endl;
	}

	// lightPos = vec4(0.0f, 100.0f,0.0f, 1.0f);
	// glUniform4f(lightPositionWorldSpaceLoc, lightPos.x, lightPos.y, lightPos.z, lightPos.w);
	
	/****************************/
	// Initialize the objects
	/****************************/
	initObjects(texID, normalTexID, colorLoc, matrix_loc);
	getTangents(tangents, bitangents);
	/****************************/
	/****************************/


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);


	// Initialization of all of the various data point in the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4)
								+ uvs.size()*sizeof(vec2)
								+ normals.size()*sizeof(vec4)
								+ tangents.size()*sizeof(vec4)
								+ bitangents.size()*sizeof(vec4)
								, NULL, GL_STATIC_DRAW);

	// vertices buffer section
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(vec4), &vertices[0][0]);
	// UVs buffer section
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), uvs.size()*sizeof(vec2), &uvs[0][0]);
	// normals buffer section
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4) + uvs.size()*sizeof(vec2), normals.size()*sizeof(vec4), &normals[0][0]);
	// tangents
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4) + uvs.size()*sizeof(vec2) + normals.size()*sizeof(vec4), tangents.size()*sizeof(vec4), &tangents[0][0]);
	// bitangens
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4) + uvs.size()*sizeof(vec2) + normals.size()*sizeof(vec4) + tangents.size()*sizeof(vec4), bitangents.size()*sizeof(vec4), &bitangents[0][0]);

	// attribute buffer for the vertex positions
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// attribute buffer for UVs 
	glEnableVertexAttribArray(uv_loc);
	glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(vec4)));

	// attribute buffer for normals
	glEnableVertexAttribArray(normal_loc);
	glVertexAttribPointer(normal_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(vec4)+uvs.size()*sizeof(vec2)));

	// attribute buffer for the tangents
	glEnableVertexAttribArray(tangents_loc);
	glVertexAttribPointer(tangents_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(vec4)+uvs.size()*sizeof(vec2)+normals.size()*sizeof(vec4)));

	// attribute buffer for the bitangents
	glEnableVertexAttribArray(bitangents_loc);
	glVertexAttribPointer(bitangents_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(vec4)+uvs.size()*sizeof(vec2)+normals.size()*sizeof(vec4)+tangents.size()*sizeof(vec4)));

	glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);

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