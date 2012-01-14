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
#include <cmath>

#include "mesh.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include "renderer.hpp"
#include "animation.hpp"
#include "scene_graph.hpp"

using namespace graingert;
bool running = true;
bool tour = false;
int speed = 0;
int pann = 0;
int up = 0;
glm::vec3 cam_position(10, 0, 0);
float prev_time;

typedef enum Scene{
	SPHERE,
	CONE,
	HOG_SPHERE,
	PHONG_SPHERE,
	EXTENSION
} Scene;

typedef struct KeyFrame{
	float time;
	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;
} KeyFrame;

void key_callback(int key, int state){
	if (state == GLFW_RELEASE){
		if (tour){
			switch (key){
				case 'Q':
				case GLFW_KEY_ESC:
					running = false;
					break;
				case 'E':
					tour = false;
					break;
			}
			return;		
		}
		switch(key){
			case 'Q':
			case GLFW_KEY_ESC:
				running = false;
				break;
			case 'T':
				tour = true;
				break;
			case GLFW_KEY_LEFT:
				pann++;
				break;
			case GLFW_KEY_RIGHT:
				pann--;
				break;
			case GLFW_KEY_PAGEUP:
				up++;
				break;
			case GLFW_KEY_PAGEDOWN:
				up--;
				break;
			case 'R':
				glfwSetTime(0);
				prev_time = 0.0f;
				cam_position = glm::vec3(10, 0, 0);
				break;
			case GLFW_KEY_DOWN:
				if (speed > 0){
					speed--;
				}
				break;
			case GLFW_KEY_UP:
				speed++;
		}
	}
}

glm::vec3 calc_tween(glm::vec3 a, glm::vec3 b, float ratio){
	return (a*ratio) + (b*(1-ratio));
}

float calc_ratio(float a, float b, float d){
	return (d-b)/(a-b);
}

glm::mat4 get_view(float time){
	time = fmod(time,20.0f);
	
	KeyFrame frames[] {
		{0.0f, glm::vec3(10,10,0), glm::vec3(0,0,0), glm::vec3(0.0f,1.0f,0.0f)},
		{5.0f, glm::vec3(10,20,0), glm::vec3(0,20,0), glm::vec3(0.0f,20.0f,0.0f)},
		{10.0f, glm::vec3(10,5,0), glm::vec3(0,0,0), glm::vec3(0.0f,1.0f,0.0f)},
		{20.0f, glm::vec3(10,10,0), glm::vec3(0,0,0), glm::vec3(0.0f,1.0f,0.0f)},
	};
	
	for (int i = 0; i<3; i++){
		if (time < frames[i].time){
			
			float ratio = calc_ratio(frames[i-1].time, frames[i].time, time);
			
			return glm::lookAt(
				calc_tween(frames[i-1].eye,frames[i].eye,ratio),
				calc_tween(frames[i-1].target,frames[i].target,ratio),
				calc_tween(frames[i-1].up,frames[i].up,ratio)
			);
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
	prev_time = glfwGetTime();

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
	
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	while (running)
	{
		float time = glfwGetTime();
		float delta = time - prev_time;
		float prev_time = time;
		
		model = glm::rotate( glm::mat4(1), (float)(50.0 * glfwGetTime()), glm::vec3(1,1,0));
		renderer._mv_matrix = view * model;
		
		if (tour){
			animation.view = get_view(glfwGetTime());
		} else {
			glm::vec3 target(0,up,pann);
			glm::vec3 direction = glm::normalize(target - cam_position);
			
			cam_position += (direction * ((delta*speed)/100.0f));
			animation.view = glm::lookAt(cam_position, target, glm::vec3(0,1,0));
		}
		
		animation.draw(glfwGetTime());

		glfwSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Exit on window close
		if (!glfwGetWindowParam(GLFW_OPENED))
			running = false;
	}

	glfwTerminate();
	return 0;
}
