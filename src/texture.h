#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "GL/glew.h"

class Texture {

	public:
		Texture();
	
		void Bind(unsigned int unit);

	private:
		Texture(const Texture& other) {} // copy constructor
		void operator=(const Texture& other) {}

		GLuint m_texture;
};

#endif // TEXTURE_H