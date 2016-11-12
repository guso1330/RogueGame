#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>

class Window {
	//
	// PUBLIC MEMBERS AND FUNCTIONS
	//
	public:
		Window(const char *name, int width, int height);
		~Window();
		bool Closed() const;
		void Update() const;

	//
	// PRIVATE MEMBERS AND FUNCTIONS
	//
	private:
		const char *m_Title;
		bool m_Closed;
		int m_Width, m_Height;
		GLFWwindow *m_Window;

		// PRIVATE METHODS
		bool init();
};

#endif