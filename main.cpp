#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <vector>
#include <stddef.h>

#include "shader.hpp"
#include "utils.hpp"

using namespace graingert;
bool running = true;

void key_callback(int key, int state){
	if (key == GLFW_KEY_ESC && state == GLFW_RELEASE){
		running = false;
	}
}

#define PI 3.14159265359
//simple struct which contains the position and color of a vertex

GLint link_shaders(const std::vector<GLint>& shaders){
	GLuint program = glCreateProgram();
	for (uint i = 0; i<shaders.size(); i++){
		glAttachShader(program, shaders[i]);
	}
	glLinkProgram(program);
	
	return program;
}

ushort makeMiddlePoint(ushort idx0, ushort idx1, std::vector<Vertex> & vertices) {
	Vertex v0 = vertices.at(idx0);
	Vertex v1 = vertices.at(idx1);
	glm::vec3 normalizedVector = glm::normalize(
			glm::vec3(
				(v0.position.x + v1.position.x) / 2.0f,
				(v0.position.y + v1.position.y) / 2.0f,
				(v0.position.z + v1.position.z) / 2.0f)
	);
	
	glm::vec4 color = glm::vec4(
		(v0.color.r + v1.color.r) / 2.0f,
		(v0.color.g + v1.color.g) / 2.0f,
		(v0.color.b + v1.color.b) / 2.0f,
		(v0.color.a + v1.color.a) / 2.0f
	);
	Vertex middle = { glm::vec4(normalizedVector, 1.0f),  color , glm::vec3(0.0f) };
	vertices.push_back(middle);
	return vertices.size() - 1; // Hack to return index of most recently added element
}

void makeISOSphere(std::vector<Vertex> & vertices, std::vector<GLuint> & indexes, GLuint iterations) {
	// Code From Nick Hollat
	// Using IcoSphere method found at http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	// Make Icosahedron
	float t = (1.0f + sqrt(5.0f)) / 2;
	// Have to manually do the 12 vertices
	Vertex startingVertices[] = {
		{glm::vec4( -1.0f,  t,  0.0f, 1.0f ), glm::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4(  1.0f,  t,  0.0f, 1.0f ), glm::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4( -1.0f, -t,  0.0f, 1.0f ), glm::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4(  1.0f, -t,  0.0f, 1.0f ), glm::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},

		{glm::vec4(  0.0f, -1.0f,  t, 1.0f ), glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4(  0.0f,  1.0f,  t, 1.0f ), glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4(  0.0f, -1.0f, -t, 1.0f ), glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4(  0.0f,  1.0f, -t, 1.0f ), glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ), glm::vec3(0.0f)},

		{glm::vec4(  t,  0.0f, -1.0f, 1.0f ), glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4(  t,  0.0f,  1.0f, 1.0f ), glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4( -t,  0.0f, -1.0f, 1.0f ), glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ), glm::vec3(0.0f)},
		{glm::vec4( -t,  0.0f,  1.0f, 1.0f ), glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ), glm::vec3(0.0f)},
	};
	// Normalize starting vertices
	for (int i = 0; i < 12; i++) {
		startingVertices[i].position = glm::vec4(
			glm::normalize(startingVertices[i].position.xyz),
			1.0f
		);
	}
	vertices.insert(vertices.begin(), startingVertices, startingVertices + 12);

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
	std::vector<GLuint> *localIndexes = new std::vector<GLuint>(startingIndexes, startingIndexes + (3 * 20)); // Have us a local version so we can create & delete for each iteration. Copy into result vector later
	for (GLuint i = 0; i < iterations; i++) {
		std::vector<GLuint> *newIndexes = new std::vector<GLuint>();
		std::vector<GLuint>::iterator it;
		for (it = localIndexes->begin(); it < localIndexes->end(); std::advance(it, 3)) {
			ushort idx[3];

			// Get the indexes for this triangle
			for (int j = 0; j < 3; j++) {
				idx[j] = *(it+j);
				if(idx[j] > vertices.size()) {
					assert(idx[j] < vertices.size());
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
				assert(idxToAdd[j] < vertices.size());
			}

			newIndexes->insert(newIndexes->begin(), idxToAdd, idxToAdd+12);
		}
		delete localIndexes;
		localIndexes = newIndexes;

		for (it = localIndexes->begin(); it < localIndexes->end(); it++) {
			assert(*it < vertices.size());
		}
	}
	indexes.insert(indexes.begin(), localIndexes->begin(), localIndexes->end()); // startingIndexes+x where x is the number of items in startingIndexes
}

void normalizeMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices){
		
	for (int i=0; i<indices.size(); i+=3) {
		glm::vec3 const & a = vertices[indices[i]].position.xyz;
		glm::vec3 const & b = vertices[indices[(i+1)]].position.xyz;
		glm::vec3 const & c = vertices[indices[(i+2)]].position.xyz; 
		
		glm::vec3 normal = glm::cross(b - a, c - a);
  
		vertices[indices[i]].normal += normal;
		vertices[indices[(i+1)]].normal += normal;
		vertices[indices[(i+2)]].normal += normal;
	}
	
	for (int i=0; i<vertices.size(); i++) {
		vertices[i].normal = glm::normalize(vertices[i].normal);
	}
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
	
	std::vector<Vertex> vertices(0);
	std::vector<GLuint> indices(0);
	
	makeISOSphere(vertices, indices, 1);
	normalizeMesh(vertices,indices);
	GLuint vertexbuffer,indexbuffer;
 
	
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);
 
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW);

	//create program from shaders
	
	std::vector<GLint> shaders;
	shaders.push_back(create_shader("SimpleVertexShader.vertexshader", GL_VERTEX_SHADER));
	shaders.push_back(create_shader("SimpleFragmentShader.fragmentshader", GL_FRAGMENT_SHADER));
	
	GLuint standard_program = link_shaders(shaders);
	
	shaders.push_back(create_shader("Normals.geometryshader", GL_GEOMETRY_SHADER));
	GLuint normals_program = link_shaders(shaders);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	double oldTime, currentTime, deltaTime;
	oldTime = glfwGetTime();
	
	while (running)
	{
		
		glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(10,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
		glm::mat4 model = glm::rotate( glm::mat4(1), (float)(50.0 * glfwGetTime()), glm::vec3(1,1,0));
		glm::mat4 viewProjection = projection * view * model;
		const size_t vertexSize = sizeof(Vertex);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex,position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex,color));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex,normal));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		
		glUseProgram(normals_program);
		GLuint matrixId = glGetUniformLocation(normals_program, "camera");
		GLuint normalLengthPtr = glGetUniformLocation(normals_program, "normalsLength");
		glUniform1f(normalLengthPtr, 0.5f);
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &viewProjection[0][0]);
		
		
		glDrawElements(	GL_TRIANGLES, //mode
						indices.size(),  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						(void*)0);
		
		glUseProgram(standard_program);
		matrixId = glGetUniformLocation(standard_program, "camera");
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &viewProjection[0][0]);
		

		glDrawElements(	GL_TRIANGLES, //mode
						indices.size(),  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						(void*)0);
						
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glfwSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Exit on window close
		if (!glfwGetWindowParam(GLFW_OPENED))
			running = false;
	}

	glfwTerminate();
	return 0;
}
