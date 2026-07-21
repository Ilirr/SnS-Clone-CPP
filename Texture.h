#pragma once
#include <string>
#include <glad/glad.h>
class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void bind(unsigned int) const;
	void unbind();

	bool isValid() const;
	GLuint getID() const { return ID; }

	int getWidth() const;
	int getHeight() const;

private: 
	GLuint ID = 0;
	const std::string m_file_path;
	int m_width, m_height, m_bpp;

};