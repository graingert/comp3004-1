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

using namespace std;
using namespace glm;
#define PI 3.14159265359
//simple struct which contains the position and color of a vertex
typedef struct {
	float XYZW[4];
	float RGBA[4];
} Vertex;

ushort makeMiddlePoint(ushort idx0, ushort idx1, vector<Vertex> *vertices) {
	Vertex v0 = vertices->at(idx0);
	Vertex v1 = vertices->at(idx1);
	vec3 normalizedVector = normalize(
			vec3(
				(v0.XYZW[0] + v1.XYZW[0]) / 2.0f,
				(v0.XYZW[1] + v1.XYZW[1]) / 2.0f,
				(v0.XYZW[2] + v1.XYZW[2]) / 2.0f)
	);
	Vertex middle = { {
		normalizedVector[0],
		normalizedVector[1],
		normalizedVector[2],
		1.0f,
	}, { 0.5f, 0.5f, 0.5f, 1.0f } };
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
		{{ -1.0f,  t,  0.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{  1.0f,  t,  0.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{ -1.0f, -t,  0.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{  1.0f, -t,  0.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},

		{{  0.0f, -1.0f,  t, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{  0.0f,  1.0f,  t, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{  0.0f, -1.0f, -t, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{  0.0f,  1.0f, -t, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},

		{{  t,  0.0f, -1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{  t,  0.0f,  1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{ -t,  0.0f, -1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
		{{ -t,  0.0f,  1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }},
	};
	// Normalize starting vertices
	for (int i = 0; i < 12; i++) {
		vec3 normalizedVector = normalize(vec3(
					startingVertices[i].XYZW[0],
					startingVertices[i].XYZW[1],
					startingVertices[i].XYZW[2]
					));
		for (int j = 0; j < 3; j++) {
			startingVertices[i].XYZW[j] = normalizedVector[j];
		}
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

//http://www.codecolony.de/opengl.htm#VertexArrays
void makeUVSphere(std::vector<Vertex> *vertices, std::vector<GLuint> *indices,
	int PointRows, int PointsPerRow)
{
	vector <Vertex> localVertices;
	vector <GLuint> localIndices;
	
	int NumVertices = (PointRows-2)*PointsPerRow + 2;
	localVertices.reserve(NumVertices);
	

	float x,y,z;
	int i,j;
	double r;
	for (i = 1; i < (PointRows-1); i++)
	{
		for (j = 0; j < PointsPerRow; j++)
		{
			y = 1.0 - float(i) / float(PointRows-1)*2.0;
			r = sin (acos(y));  //radius of the row
			x = r * sin(float(j) / float(PointsPerRow)*PI*2.0);

			z = r * cos(float(j) / float(PointsPerRow)*PI*2.0);
			localVertices[(i-1)*PointsPerRow+j].XYZW[0] = x;
			localVertices[(i-1)*PointsPerRow+j].XYZW[1] = y;
			localVertices[(i-1)*PointsPerRow+j].XYZW[2] = z;
			localVertices[(i-1)*PointsPerRow+j].XYZW[3] = 0;
			localVertices[(i-1)*PointsPerRow+j].RGBA[0] = (float)(i) / float(PointRows);
			localVertices[(i-1)*PointsPerRow+j].RGBA[1] = 0.7;
			localVertices[(i-1)*PointsPerRow+j].RGBA[2] = (float)(j) / float(PointsPerRow);
			localVertices[(i-1)*PointsPerRow+j].RGBA[3] = 1.0;
		}

	}
	//The highest and deepest vertices:
	localVertices[(PointRows-2)*PointsPerRow].XYZW[0] = 0.0;
	localVertices[(PointRows-2)*PointsPerRow].XYZW[1] = 1.0;
	localVertices[(PointRows-2)*PointsPerRow].XYZW[2] = 0.0;
	localVertices[(PointRows-2)*PointsPerRow].XYZW[3] = 0.0;
	localVertices[(PointRows-2)*PointsPerRow].RGBA[0] = 1.0;
	localVertices[(PointRows-2)*PointsPerRow].RGBA[1] = 0.7;
	localVertices[(PointRows-2)*PointsPerRow].RGBA[2] = 1.0;
	localVertices[(PointRows-2)*PointsPerRow].RGBA[3] = 1.0;
	localVertices[(PointRows-2)*PointsPerRow+1].XYZW[0] = 0.0;
	localVertices[(PointRows-2)*PointsPerRow+1].XYZW[1] = -1.0;
	localVertices[(PointRows-2)*PointsPerRow+1].XYZW[2] = 0.0;
	localVertices[(PointRows-2)*PointsPerRow+1].XYZW[3] = 0.0;
	localVertices[(PointRows-2)*PointsPerRow+1].RGBA[0] = 1.0;
	localVertices[(PointRows-2)*PointsPerRow+1].RGBA[1] = 0.7;
	localVertices[(PointRows-2)*PointsPerRow+1].RGBA[2] = 1.0;
	localVertices[(PointRows-2)*PointsPerRow+1].RGBA[3] = 1.0;

	//create the index array:
	for (i = 1; i < (PointRows-2); i++)
	{
		for (j = 0; j < (PointsPerRow-1); j++)
		{
			localIndices.push_back((i-1)*PointsPerRow+j);
			localIndices.push_back((i-1)*PointsPerRow+j+1);
			localIndices.push_back((i)*PointsPerRow+j);

			localIndices.push_back((i-1)*PointsPerRow+j+1);
			localIndices.push_back((i)*PointsPerRow+j+1);
			localIndices.push_back((i)*PointsPerRow+j);
		}

		localIndices.push_back((i-1)*PointsPerRow+PointsPerRow-1);
		localIndices.push_back((i-1)*PointsPerRow);
		localIndices.push_back((i)*PointsPerRow+j);

		localIndices.push_back((i)*PointsPerRow);
		localIndices.push_back((i-1)*PointsPerRow);
		localIndices.push_back((i)*PointsPerRow+j);
	}		

	//The triangles to the highest and deepest vertices:
	for (j = 0; j< (PointsPerRow-1); j++)
	{
		localIndices.push_back(j);
		localIndices.push_back(j+1);
		localIndices.push_back((PointRows-2)*PointsPerRow);
	}
	localIndices.push_back(j);
	localIndices.push_back(0);
	localIndices.push_back((PointRows-2)*PointsPerRow);

	for (j = 0; j< (PointsPerRow-1); j++)
	{
		localIndices.push_back((PointRows-3)*PointsPerRow+j);
		localIndices.push_back((PointRows-3)*PointsPerRow+j+1);
		localIndices.push_back((PointRows-2)*PointsPerRow+1);
	}
	localIndices.push_back((PointRows-3)*PointsPerRow+j);
	localIndices.push_back((PointRows-3)*PointsPerRow);
	localIndices.push_back((PointRows-2)*PointsPerRow+1);
	
	indices->insert(indices->begin(), localIndices.begin(), localIndices.end());
	vertices->insert(vertices->begin(), localVertices.begin(), localVertices.end());
}

void makeCone(std::vector<Vertex> *vertices, std::vector<GLuint> *indices,
	int radius, int height, int slices){
	//Based on http://programming4.us/multimedia/1192.aspx
	
	m_cone.resize((coneSlices + 1) * 2);

	// Initialize the vertices of the triangle strip.
	vector<Vertex>::iterator vertex = m_cone.begin();
	const float dtheta = TwoPi / coneSlices;
	for (float theta = 0; vertex != m_cone.end(); theta += dtheta) {
		// Grayscale gradient
		float brightness = abs(sin(theta));
		vec4 color(brightness, brightness, brightness, 1);
		
		// Apex vertex
		vertex->Position = vec3(0, 1, 0);
		vertex->Color = color;
		vertex++;
		
		// Rim vertex
		vertex->Position.x = coneRadius * cos(theta);
		vertex->Position.y = 1 - coneHeight;
		vertex->Position.z = coneRadius * sin(theta);
		vertex->Color = color;
		vertex++;
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
	
	vector<Vertex> vertices;
	vector<GLuint> indices;
	
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
