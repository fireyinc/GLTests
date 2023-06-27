#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>


class Shader {

private:
	unsigned int ID;
	char* fragerrors;
	char* verterrors;
	const char* vpath;
	const char* fpath;


public:


	Shader(const char* vertexPath, const char* fragmentPath);

	void load();
	unsigned int getID();
	void setVec3f(const char* uniformname, float val1, float val2, float val3); 
	void setVec3(const char* uniformname, glm::vec3 vector);
	void setFloat(const char* uniformname, float val1);
	void setInt(const char* uniformname, int val1);

};


#endif