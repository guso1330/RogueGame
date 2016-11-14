#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "src/window.h"

int main() {

	// Initialize the window
	Window window("Rogue Game", 800, 600);

	// Testing a vao to check if glew is working
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Testing GLM
	glm::mat4 matrix[16];
	std::cout << &matrix << std::endl;

	// Run the window while it is not closed
	while(!window.Closed()) {

		window.Clear();

		// // Draw OLDSCHOOL TRIANGLE
		// glBegin(GL_QUADS);
		// glVertex2f(-0.5f, -0.5f);
		// glVertex2f(-0.5f,  0.5f);
		// glVertex2f(0.5f, 0.5f);
		// glVertex2f(0.5f, -0.5f);
		// glEnd();

		glDrawArrays(GL_ARRAY_BUFFER, 0, 6);

		window.Update(); // Run update on the window
	}

	system("PAUSE");
	return 0;
}