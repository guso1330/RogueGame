#include "window.h"

Window::Window(const char *title, int width, int height) {
	m_Title = title;
	m_Width = width;
	m_Height = height;

	if(!init()) { // initialize the window
		glfwTerminate();
	}
}

bool Window::init() {
	// Initialize glfw and check if it is working
	if(!glfwInit()) {
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return false;
	}
	// Create our window	
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	
	if(!m_Window) { // Check if the window did open and initialize
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
		return false;
	}
	
	glfwMakeContextCurrent(m_Window); // Set the window context
	return true;
}

Window::~Window() {
	glfwTerminate(); // Close the window
}

bool Window::Closed() const {
	return glfwWindowShouldClose(m_Window);
}

void Window::Update() const {
	glClearColor(0.0f, 0.0f, 0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_Window);
	glfwPollEvents(); // pull the events
}