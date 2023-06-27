#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include <string>
#include <custom/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>


std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	vpath = vertexPath;
	fpath = fragmentPath;

	std::string vertexString = readFile(vertexPath);

	const char* vertexShaderSource = vertexString.c_str();

	std::string fragmentString = readFile(fragmentPath);

	const char* fragmentShaderSource = fragmentString.c_str();

	unsigned int vShader, fShader;

	//vertex shader init
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vShader);

	int success;
	char error[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

	//check for errors
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, error);
		std::cout << "Error compiling Vertex Shader \n" << error << "\n";
		verterrors = error;

	}

	//fragment shader init
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fShader);

	//check for errors
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, error);
		std::cout << "Error compiling Fragment Shader \n" << error << "\n";
		fragerrors = error;
	}

	//combine shaders into Program
	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);

	//and clear the now useless shaders (useless because they're redundant to the already linked program
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

void Shader::load() {
	glUseProgram(ID);
}

unsigned int Shader::getID() {
	return ID;
}

void Shader::setVec3f(const char* uniformname, float val1, float val2, float val3) {
	glUniform3f(
		glGetUniformLocation(ID, uniformname),
		val1,
		val2, 
		val3
	);
}

void Shader::setVec3(const char* uniformname, glm::vec3 vector){
	glUniform3fv(glGetUniformLocation(ID, uniformname), 1, glm::value_ptr(vector));
}

void Shader::setFloat(const char* uniformname, float val1) {
	glUniform1f(glGetUniformLocation(ID, uniformname), val1);
}

void Shader::setInt(const char* uniformname, int val1) {
	glUniform1i(glGetUniformLocation(ID, uniformname), val1);
}

