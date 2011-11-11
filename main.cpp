#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <vector>

#include "common/shader.hpp"

bool running = true;

void key_callback(int key, int state){
	if (key == GLFW_KEY_ESC && state == GLFW_RELEASE){
		running = false;
	}
}

#define PI 3.14159265359
//simple struct which contains the position and color of a vertex
typedef struct {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
} Vertex;

void makeCone(std::vector<Vertex> *vs, stf::vector<GLuint> *is, float top_radius,
	float bottom_radius, float height, uint slices){
	// create cap vertices
	
	std::vector<Vertex> cap_vertex;
	stf::vector<GLuint> cap_index;
	
	float r = top_radius;
	//zero is a special case
	
	
	for (int i = 1, i++, i<slices){
		float theta = (2.0f*PI*i)/slices);
		float x = r std::cos(theta);
		float y = r * std::sin(theta);
		float z = 0.0f;
	}
		
	// join caps
		
}




int main()
{
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
	
	std::vector<Vertex> vertices;
	stf::vector<GLuint> indices;
	
	makeUVSphere(&vertices, &indices, 16,16);

	GLuint vertexbuffer,indexbuffer;
 
	
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);
 
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW);

	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	double oldTime, currentTime, deltaTime;
	oldTime = glfwGetTime();
	
	while (running)
	{
		
		glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(vec3(10,0,0), vec3(0,0,0), vec3(0,1,0));
		glm::mat4 model = glm::rotate( glm::mat4(1), (float)(50.0 * glfwGetTime()), glm::vec3(1,1,0));
		glm::mat4 viewProjection = projection * view * model;

		GLuint matrixId = glGetUniformLocation(programID, "camera");
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &viewProjection[0][0]);
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(programID);
		const size_t vertexSize = sizeof(Vertex);
		const size_t colorOffset = sizeof(float) * 4;
		glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
		glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)colorOffset);
		glEnableVertexAttribArray(0);
		
		glDrawElements(	GL_TRIANGLES, //mode
						indices.size(),  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						(void*)0);
						
		glDisableVertexAttribArray(0);
		glfwSwapBuffers();

		// Exit on window close
		if (!glfwGetWindowParam(GLFW_OPENED))
			running = false;
	}

	glfwTerminate();
	return 0;
}
