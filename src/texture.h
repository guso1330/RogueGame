#ifndef TEXTURE_H
#define TEXTURE_H

#include "Angel.h"
#include <string>

using namespace std;

class Texture {

	public:
		Texture(const std::string& filename);
		~Texture();

		inline GLuint GetID() { return m_texID; }

	private:
		Texture(const Texture& other) {} // copy constructor
		void operator=(const Texture& other) {}

		GLuint m_texID;
};

#endif // TEXTURE_H