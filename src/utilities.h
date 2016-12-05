#ifndef UTILITIES_H
#define UTILITIES_H

#include "camera.h"
#include "mesh.h"
#include <vector>

void ControlCamera(Camera &camera, bool key[], float camera_speed, float camera_rotate_speed) {
	if(key['w'] || key['W']) {
		camera.MoveForward(camera_speed);
	}
	if(key['s'] || key['S']) {
		camera.MoveForward(-camera_speed);
	}
	if(key['d'] || key['D']) {
		camera.MoveRight(-camera_speed);
	}
	if(key['a'] || key['A']) {
		camera.MoveRight(camera_speed);
	}
	if(key['l'] || key['L']) {
		camera.RotateYaw(-camera_rotate_speed);
	}
	if(key['k'] || key['K']) {
		camera.RotateYaw(camera_rotate_speed);
	}
}

int incrementIndex(int &total, int incr) {
	return total += incr;
}

void combineVec4Vectors(std::vector<vec4> &a, std::vector<vec4> b) {
	for(std::vector<vec4>::iterator it=b.begin(); it < b.end(); ++it) {
		a.push_back(*it);
	}
}

void combineVec2Vectors(std::vector<vec2> &a, std::vector<vec2> b) {
	for(std::vector<vec2>::iterator it=b.begin(); it < b.end(); ++it) {
		a.push_back(*it);
	}
}

#endif