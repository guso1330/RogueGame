#ifndef TEXTURE_H
#define TEXTURE_H

#include "Angel.h"
#include <string>

using namespace std;

class Texture {

	public:
		Texture(const std::string& filename);
		~Texture();
	
		void Bind(unsigned int unit);

	private:
		Texture(const Texture& other) {} // copy constructor
		void operator=(const Texture& other) {}

		GLuint m_texture;
};

#endif // TEXTURE_H