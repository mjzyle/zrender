
#ifndef SHADER_H
#define SHADER_H

#include <gl/GL.h>

#pragma once
class shader {
public:
	unsigned int id;

	shader(char *vertFile, char *fragFile);
	~shader();

	void activate();
	void readShaderFile(char *filename);
	void setBool();
	void setInt();
	void setFloat();
};

#endif
