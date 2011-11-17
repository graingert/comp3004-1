#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <vector>
#include <stddef.h>
#include <iostream>

#include "mesh.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include "renderer.hpp"
#include "animation.hpp"

using namespace graingert;
bool running = true;

typedef enum Scene{
	SPHERE,
	CONE,
	HOG_SPHERE,
	PHONG_SPHERE,
	EXTENSION
} Scene;

Scene scene = SPHERE;

void key_callback(int key, int state){
	if (state == GLFW_RELEASE){
		switch (key){
			case 'A':
				scene = SPHERE;
				break;
			case 'B':
				scene = CONE;
				break;
			case 'C':
				scene = HOG_SPHERE;
				break;
			case 'D':
				scene = PHONG_SPHERE;
				break;
			case 'E':
				scene = EXTENSION;
				break;
			case GLFW_KEY_ESC:
				running = false;
				break;
		}
	}

}


GLint link_shaders(const std::vector<GLint>& shaders){
	GLuint program = glCreateProgram();
	for (GLuint i = 0; i<shaders.size(); i++){
		glAttachShader(program, shaders[i]);
	}
	glLinkProgram(program);
	
	return program;
}

int main()
{
	std::cout << "A OK";
	std::flush(std::cout);
	glfwInit();

	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(640, 480, 0, 0, 0, 0, 32, 0, GLFW_WINDOW);

	glewInit();

	glfwSetWindowTitle("OpenGL");

	glClearColor(0x07/255.0f, 0x76/255.0f, 0xA0/255.0f, 1.0f);
	
	//Enable VSync
	glfwSwapInterval(1);
	
	glfwSetKeyCallback(&key_callback);
	
	
	
	Cone cone(10);
	UVSphere uvSphere(2);
	
	cone.buffer();
	cone.bind();
	uvSphere.buffer();
	uvSphere.bind();

	//create program from shaders
	
	std::vector<GLint> shaders;
	shaders.push_back(create_shader("SimpleVertexShader.vertexshader", GL_VERTEX_SHADER));
	shaders.push_back(create_shader("PhongShader.fragmentshader", GL_FRAGMENT_SHADER));
	
	GLuint standard_program = link_shaders(shaders);
	
	
	glm::mat4 p_matrix = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(10,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 model = glm::rotate( glm::mat4(1), (float)(50.0 * glfwGetTime()), glm::vec3(1,1,0));
	glm::mat4 mv_matrix = view * model;
	
	Renderer renderer(standard_program, mv_matrix, p_matrix);
	
	Animation animation(renderer);
	animation.buffer();
	
	shaders.push_back(create_shader("Normals.geometryshader", GL_GEOMETRY_SHADER));
	GLuint normals_program = link_shaders(shaders);
	
	NormalsRenderer normalsRenderer(normals_program, mv_matrix, p_matrix, 0.5f);
	
	glEnable(GL_DEPTH_TEST);
	
	while (running)
	{
		view = glm::lookAt(glm::vec3(10,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
		model = glm::rotate( glm::mat4(1), (float)(50.0 * glfwGetTime()), glm::vec3(1,1,0));
		renderer._mv_matrix = normalsRenderer._mv_matrix = view * model;
		
		switch(scene){
			case SPHERE:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				renderer.bind();
				uvSphere.draw();
				break;
			case CONE:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				renderer.bind();
				cone.draw();
				break;
			case HOG_SPHERE:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				renderer.bind();
				uvSphere.draw();
				normalsRenderer.bind();
				uvSphere.draw();
				break;
			case PHONG_SPHERE:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				renderer.bind();
				uvSphere.draw();
				break;
			case EXTENSION:
				animation.draw(glfwGetTime());
				break;
		}

		glfwSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Exit on window close
		if (!glfwGetWindowParam(GLFW_OPENED))
			running = false;
	}

	glfwTerminate();
	return 0;
}
