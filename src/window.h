#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void windowResize(GLFWwindow *window, int width, int height);

class Window {
	//
	// PUBLIC MEMBERS AND FUNCTIONS
	//
	public:
		Window(const char *name, int width, int height);
		~Window();
		bool Closed() const;
		void Clear() const;
		void Update();

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }

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