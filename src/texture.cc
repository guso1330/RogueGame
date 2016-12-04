#include "texture.h"
#include <cassert>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION // needed to use the stb_image library
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filename) {

	GLuint m_texture;
	glGenTextures(1, &m_texture); // Generate space for the texture
	glBindTexture(GL_TEXTURE_2D, m_texture);

	int width, height, numComponents; // Image properties
	unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, &numComponents, 4); // Load the image

	if(image_data == NULL) { // error checking
		std::cerr << "Texture loading failed for texture: " << filename << std::endl;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	stbi_image_free(image_data);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_texture); // delete the texture
}

void Texture::Bind(unsigned int unit) {
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit); // changes the working texture
	glBindTexture(GL_TEXTURE_2D, m_texture); // bind the texture to the current unit
}