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
	glm::vec4 position;
	glm::vec4 color;
	glm::vec3 normal;
} Vertex;

ushort makeMiddlePoint(ushort idx0, ushort idx1, std::vector<Vertex> *vertices) {
	glm::vec4 v0 = vertices->at(idx0).position;
	glm::vec4 v1 = vertices->at(idx1).position;
	vec3 normalizedVector = glm::normalize(
			vec3(
				(v0.x + v1.x) / 2.0f,
				(v0.y + v1.y) / 2.0f,
				(v0.z + v1.z) / 2.0f)
	);
	Vertex middle = { gml::vec4(normalizedVector, 1.0f), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ), glm::vec3(0) };
	vertices->push_back(middle);
	return vertices->size() - 1; // Hack to return index of most recently added element
}

void makeISOSphere(std::vector<Vertex> *vertices, std::vector<GLuint> *indexes, GLuint iterations) {
	// Code From Nick Hollat
	// Using IcoSphere method found at http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	// Make Icosahedron
	float t = (1.0f + sqrt(5.0f)) / 2;
	// Have to manually do the 12 vertices
	Vertex startingVertices[] = {
		{glm::vec4( -1.0f,  t,  0.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4(  1.0f,  t,  0.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4( -1.0f, -t,  0.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4(  1.0f, -t,  0.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},

		{glm::vec4(  0.0f, -1.0f,  t, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4(  0.0f,  1.0f,  t, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4(  0.0f, -1.0f, -t, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4(  0.0f,  1.0f, -t, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},

		{glm::vec4(  t,  0.0f, -1.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4(  t,  0.0f,  1.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4( -t,  0.0f, -1.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
		{glm::vec4( -t,  0.0f,  1.0f, 1.0f ), glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),glm::vec3(0)},
	};
	// Normalize starting vertices
	for (int i = 0; i < 12; i++) {
		vec3 normalizedVector = glm::normalize(vec3(
					startingVertices[i].position.x,
					startingVertices[i].position.y,
					startingVertices[i].position.z
					));
		startingVetices[i].position = glm::vec4(normalizedVector,1.0f)
	}
	vertices->insert(vertices->begin(), startingVertices, startingVertices + 12);

	// Make the 20 faces of the icosahedron
	GLuint startingIndexes[] = {
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1,
	};
	vector<GLuint> *localIndexes = new vector<GLuint>(startingIndexes, startingIndexes + (3 * 20)); // Have us a local version so we can create & delete for each iteration. Copy into result vector later
	for (int i = 0; i < iterations; i++) {
		vector<GLuint> *newIndexes = new vector<GLuint>();
		vector<GLuint>::iterator it;
		for (it = localIndexes->begin(); it < localIndexes->end(); it+=3) {
			ushort idx[3];

			// Get the indexes for this triangle
			for (int j = 0; j < 3; j++) {
				idx[j] = *(it+j);
				if(idx[j] > vertices->size()) {
					assert(idx[j] < vertices->size());
				}
			}

			ushort a = makeMiddlePoint(idx[0], idx[1], vertices);
			ushort b = makeMiddlePoint(idx[1], idx[2], vertices);
			ushort c = makeMiddlePoint(idx[2], idx[0], vertices);

			ushort idxToAdd[] = { // Indexes of the new triangles
				idx[0], a, c,
				idx[1], b, a,
				idx[2], c, b,
				a, b, c,
			};

			for (int j = 0; j < 12; j++) {
				assert(idxToAdd[j] < vertices->size());
			}

			newIndexes->insert(newIndexes->begin(), idxToAdd, idxToAdd+12);
		}
		delete localIndexes;
		localIndexes = newIndexes;

		for (it = localIndexes->begin(); it < localIndexes->end(); it++) {
			assert(*it < vertices->size());
		}
	}
	indexes->insert(indexes->begin(), localIndexes->begin(), localIndexes->end()); // startingIndexes+x where x is the number of items in startingIndexes
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
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), vertices.begin()), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint), indices.begin()), GL_STATIC_DRAW);

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
