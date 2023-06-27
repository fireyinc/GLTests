#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <custom/Shader.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

using namespace std;

float vpheight = 600.0f;
float vpwidth = 800.0f;

glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

//start camera stuff

float moveSpeed = 1.0f;
float sensitivity = 0.1f;

float deltaTime = 0.0f;
float prevFrameTime = 0.0f;

float mouseDeltaX = 0.0f;
float prevMouseX = 400.0f;
float mouseDeltaY = 0.0f;
float prevMouseY = 300.0f;

float yaw = 0.0f;
float pitch = 0.0f;

glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);

glm::vec3 cameraForward(
	cos( glm::radians(pitch)) * cos( glm::radians(yaw + -90.0f) ),
	sin(glm::radians(pitch)),
	cos(glm::radians(pitch)) * sin(glm::radians(yaw + -90.0f))
);

bool mouseExited = false;

//end camera stuff

void closeWindowHotkey(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void cameraActions(GLFWwindow* window, float sensitivity) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += cameraForward * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= cameraForward * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= glm::normalize(glm::cross(cameraForward, globalUp)) * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += glm::normalize(glm::cross(cameraForward, globalUp)) * moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		moveSpeed = 2.0f;
	}
	else {
		moveSpeed = 1.0f;
	}
}

void mousechangecallback(GLFWwindow* window, double xpos, double ypos) {
	if (!mouseExited) {
		float currentMouseX = xpos;
		float currentMouseY = ypos;
		mouseDeltaX = currentMouseX - prevMouseX;
		prevMouseX = currentMouseX;

		mouseDeltaY = currentMouseY - prevMouseY;
		prevMouseY = currentMouseY;


		yaw += mouseDeltaX * sensitivity;
		pitch -= mouseDeltaY * sensitivity;

		if (pitch > 90.0) {
			pitch = 90.0;
		}

		if (pitch < -90.0) {
			pitch = -90.0;
		}
	}
}

void mouseControl(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseExited = true;
	}
}

void resetMouse(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseExited = false;
	}
}

void resetViewport(GLFWwindow* window, int x, int y) {
	vpheight = (float)y;
	vpwidth = (float)x;


	glViewport(0, 0, x, y);

}

int main() {


	//this is all boring housekeeping
	std::cout << "Start initialize\n";
	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	

	

	std::cout << "Set window hints\n";

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Window failed to initialize";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD\n";
		return -1;
	}

	glViewport(0, 0, vpwidth, vpheight);

	//finish boring housekeeping




	float trivertices[] = {

		//back
		-0.5f, -0.5f, -0.5f,  	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  	1.0f, 0.0f,		0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  	0.0f, 1.0f,		0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,

		//front
		-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,		0.0f,  0.0f, 1.0f, 
		0.5f, -0.5f,  0.5f,  	1.0f, 0.0f,		0.0f,  0.0f, 1.0f, 
		0.5f,  0.5f,  0.5f,  	1.0f, 1.0f,		0.0f,  0.0f, 1.0f, 
		0.5f,  0.5f,  0.5f,  	1.0f, 1.0f,		0.0f,  0.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f,  	0.0f, 1.0f,		0.0f,  0.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,		0.0f,  0.0f, 1.0f, 

		//left
		-0.5f,  0.5f,  0.5f,  	1.0f, 0.0f,		-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  	0.0f, 1.0f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  	0.0f, 1.0f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,		-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  	1.0f, 0.0f,		-1.0f,  0.0f,  0.0f,

		//right
		0.5f,  0.5f,  0.5f,  	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  	1.0f, 1.0f,		1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,		1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,

		//bottom
		-0.5f, -0.5f, -0.5f,  	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  	1.0f, 1.0f,		0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,		0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,

		//top
		-0.5f,  0.5f, -0.5f,  	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  	1.0f, 1.0f,		0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  	0.0f, 0.0f,		0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,
	};

	glm::vec3 amigapositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -3.0f, -3.0f),		
		glm::vec3(0.0f, 3.0f, -3.0f),
		glm::vec3(3.0f, 0.0f, -3.0f),	
		glm::vec3(-3.0f, 0.0f, -3.0f),

		glm::vec3(0.0f, 3.0f, 3.0f),	
		glm::vec3(0.0f, -3.0f, 3.0f),
		glm::vec3(3.0f, 0.0f, 3.0f),	
		glm::vec3(-3.0f, 0.0f, 3.0f),
	
	};

	//objects
	unsigned int VBO; //vertex buffer object


	unsigned int VAO; //vertex array object
	unsigned int VAO2; //vertex array object for the lighting cube



	unsigned int EBO; //element buffer object

	unsigned int Texture; //texture object
	unsigned int Texture2;
	unsigned int Texture3;

	Shader shader = Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
	Shader lightshader = Shader("../shaders/lightvertex.glsl", "../shaders/lightfragment.glsl");


	

	//initialize vertex array
	//All commands below this are autoapplied to this vertex array until another is bound
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);




	//initialize vertex buffer

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//initialize element buffer
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_DYNAMIC_DRAW);




	//initialize texture
	int height, width, channels;

	//first tecture
	unsigned char* textureimage = stbi_load("../textures/amigatexture.png", &width, &height, &channels, 0);

	glGenTextures(1, &Texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureimage);
	glGenerateMipmap(GL_TEXTURE_2D);


	//second texture
	unsigned char* textureimage2 = stbi_load("../textures/amigatexturespecmap.png", &width, &width, &channels, 0);

	glGenTextures(1, &Texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureimage2);
	glGenerateMipmap(GL_TEXTURE_2D);

	unsigned char* textureimage3 = stbi_load("../textures/amigatextureemissionmap.png", &width, &height, &channels, 0);
	
	glGenTextures(1, &Texture3);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureimage3);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureimage);
	stbi_image_free(textureimage2);
	stbi_image_free(textureimage3);




	//each attrib in the vertex array needs one of these
	//the item in glenablevertexattribarray has to be the position defined in the vertex shader
	//same with the index on the first argument of glVertexAttribPointer
	//the start arg also changes, with the next part starting after a number of floats after the next, (which is where (3*(sizeof) float) comes from)
	//finally, the stride also changes to a number of floats which is where 9*(sizeof)float comes from

	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//colors
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texcoords
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//glTexParameter[type (i, ui, uiv, fv, f)](texture type, parameter, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//This is all applied to the vertex array defined above. The VBuffer only contains data however.

	//This is the start of the other vertex buffer. This has different shaders and everything. 
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//binding a vertex array changes data for that specific array 


	//this is our bufferdata. this is agnostic to varrays.
	glBufferData(GL_ARRAY_BUFFER, sizeof(trivertices), &trivertices, GL_DYNAMIC_DRAW);




	glEnable(GL_DEPTH_TEST);

	


















	//this is where all loops and callbacks go
	glfwSetCursorPosCallback(window, mousechangecallback);
	glfwSetWindowSizeCallback(window, resetViewport);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//light color
	glm::vec3 lightcolor(1.0f, 1.0f, 1.0f);
	glm::vec3 cubecolor(0.0f, 0.0f, 0.0f);
	float speclight = 32.0f;
	float brightness = 0.5f;


	while (!glfwWindowShouldClose(window)) {



		closeWindowHotkey(window);
		mouseControl(window);

		if (!io.WantCaptureMouse){
			resetMouse(window);
		}

		//this is how deltatime works
		float nowFrame = glfwGetTime();
		deltaTime = nowFrame - prevFrameTime;
		prevFrameTime = nowFrame;

		//render commands go after this comment
		//so dont be stupid and put them before and wonder why this aint workin
		//please dont also put setup commands here, thx



		glClearColor(0.1, 0.1, 0.15, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		
		ImGui::Begin("Window 1");


		ImGui::ColorPicker3("Light Color", glm::value_ptr(lightcolor));
		ImGui::SliderFloat("Specular", &speclight, 0.0f, 512.0f);
		ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);
		ImGui::End();


		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);



		//this specific snippet of code renders the thing with indices. this is not of much use to me but it is here.
		//glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, (void*)0);





		const float radius = 5.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		cameraActions(window, 1.0f);


		//projection and whatnot
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(45.0f), vpwidth / vpheight, 0.1f, 100.0f);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPosition, cameraPosition + cameraForward, globalUp);

		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));



		cameraForward = glm::vec3( 
			cos(glm::radians(pitch)) * cos(glm::radians(yaw + -90.0f)),
			sin(glm::radians(pitch)),
			cos(glm::radians(pitch)) * sin(glm::radians(yaw + -90.0f))
		);



		//apply first shader
		shader.load();





		shader.setVec3("cameraposition", cameraPosition);

		//setting material struct

		//shader.setVec3("mat.specular", glm::vec3(0.6));
		shader.setInt("mat.color", 0);
		shader.setInt("mat.specular", 1);
		shader.setInt("mat.emission", 2);
		shader.setFloat("mat.shininess", speclight);



		//setting light struct
		shader.setVec3("light.position", lightPosition);
		shader.setVec3("light.ambient", glm::vec3(brightness/6)*lightcolor);
		shader.setVec3("light.diffuse", glm::vec3(brightness)*lightcolor);
		shader.setVec3("light.specular", glm::vec3(1.0f)*lightcolor);



		//view projections & world space stuff
		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));


		for (int i = 0; i < 9; i++) {
			//transformations
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, amigapositions[i]);
			model = glm::rotate(model, (float)sin(glfwGetTime() * 0.3) * 5.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
			



			//apply model transforms
			glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

			//render first cube
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(trivertices));
		}


		//apply second shaders
		lightshader.load();

		lightshader.setVec3("lightboxcolor", lightcolor);

		//apply projections & world view stuff
		glUniformMatrix4fv(glGetUniformLocation(lightshader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightshader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		//reset model
		glm::mat4 lightmodel(1.0f);




		lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);

		//set transformations
		lightmodel = glm::translate(lightmodel, lightPosition);
		lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));

		//apply transformations
		glUniformMatrix4fv(glGetUniformLocation(lightshader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(lightmodel));

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(trivertices));

		//start of imgui ui commands for this section





		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//end of imgui ui commands


		//end of render commands
		//if every render command is in here, then u good
		//or else.

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO);

	std::cout << "i sleep\n";
	glfwTerminate();


}

