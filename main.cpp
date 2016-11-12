#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "src/window.h"

int main() {

	// Initialize the window
	Window window("Rogue Game", 800, 600);

	// Run the window while it is not closed
	while(!window.Closed()) {
		window.Update(); // Run update on the window
	}

	system("PAUSE");
	return 0;
}