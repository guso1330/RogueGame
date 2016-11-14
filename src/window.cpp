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
	// Create the GLFW context for the window
	glfwMakeContextCurrent(m_Window); // Set the window context
	glfwSetWindowSizeCallback(m_Window, windowResize);


	// Initialize GLEW
	if(glewInit() != GLEW_OK) {
		std::cout << "GLEW failed to initialize!" << std::endl;
		return false;
	}

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
	
	return true;
}

Window::~Window() {
	glfwTerminate(); // Close the window
}

bool Window::Closed() const {
	return glfwWindowShouldClose(m_Window) == 1;
}

void Window::Clear() const {
	glClearColor(0.0f, 0.2f, 0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update() {
	glfwPollEvents(); // pull the events
	glfwSwapBuffers(m_Window);
}

void windowResize(GLFWwindow *window, int width, int height) {
	// Does not respect aspect ratio right this moment
	glViewport(0, 0, width, height); // Does not resize the m_Width and m_Height properties
}