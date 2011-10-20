#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
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
#define PI 3.14159265359
//simple struct which contains the position and color of a vertex
struct SVertex
{
	GLfloat x,y,z;
	GLfloat r,g,b;
};

//Data for the sphere vertices:
SVertex * Vertices;
int NumVertices;  //size of the vertex array
GLuint * Indices;  //indices which define how to connect the sphere vertices
int NumIndices;   //size of the index array
GLfloat ChangeY = 0.025;//Indicates how much the y values of the highest and deepest vertex
						//in the sphere are changed each time it is rendered
float yRotated = 0.0;

void CreateSphere(int PointRows, int PointsPerRow)
{
	vector <GLuint> IndexVect;  //we first put the indices into this vector, then copy them to the array below

	//calculate the total number of vertices
	NumVertices = (PointRows-2)*PointsPerRow + 2;
	Vertices = new SVertex[NumVertices];
	IndexVect.clear();  //to be sure it is empty
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
			Vertices[(i-1)*PointsPerRow+j].x = x;
			Vertices[(i-1)*PointsPerRow+j].y = y;
			Vertices[(i-1)*PointsPerRow+j].z = z;
			Vertices[(i-1)*PointsPerRow+j].r = (float)(i) / float(PointRows);
			Vertices[(i-1)*PointsPerRow+j].g = 0.7;
			Vertices[(i-1)*PointsPerRow+j].b = (float)(j) / float(PointsPerRow);
		}

	}
	//The highest and deepest vertices:
	Vertices[(PointRows-2)*PointsPerRow].x = 0.0;
	Vertices[(PointRows-2)*PointsPerRow].y = 1.0;
	Vertices[(PointRows-2)*PointsPerRow].z = 0.0;
	Vertices[(PointRows-2)*PointsPerRow].r = 1.0;
	Vertices[(PointRows-2)*PointsPerRow].g = 0.7;
	Vertices[(PointRows-2)*PointsPerRow].b = 1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].x = 0.0;
	Vertices[(PointRows-2)*PointsPerRow+1].y = -1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].z = 0.0;
	Vertices[(PointRows-2)*PointsPerRow+1].r = 1.0;
	Vertices[(PointRows-2)*PointsPerRow+1].g = 0.7;
	Vertices[(PointRows-2)*PointsPerRow+1].b = 1.0;
	
	//create the index array:
	for (i = 1; i < (PointRows-2); i++)
	{
		for (j = 0; j < (PointsPerRow-1); j++)
		{
			IndexVect.push_back((i-1)*PointsPerRow+j);
			IndexVect.push_back((i-1)*PointsPerRow+j+1);
			IndexVect.push_back((i)*PointsPerRow+j);

			IndexVect.push_back((i-1)*PointsPerRow+j+1);
			IndexVect.push_back((i)*PointsPerRow+j+1);
			IndexVect.push_back((i)*PointsPerRow+j);
		}

		IndexVect.push_back((i-1)*PointsPerRow+PointsPerRow-1);
		IndexVect.push_back((i-1)*PointsPerRow);
		IndexVect.push_back((i)*PointsPerRow+j);

		IndexVect.push_back((i)*PointsPerRow);
		IndexVect.push_back((i-1)*PointsPerRow);
		IndexVect.push_back((i)*PointsPerRow+j);
	}		

	//The triangles to the highest and deepest vertices:
	for (j = 0; j< (PointsPerRow-1); j++)
	{
		IndexVect.push_back(j);
		IndexVect.push_back(j+1);
		IndexVect.push_back((PointRows-2)*PointsPerRow);
	}
	IndexVect.push_back(j);
	IndexVect.push_back(0);
	IndexVect.push_back((PointRows-2)*PointsPerRow);

	for (j = 0; j< (PointsPerRow-1); j++)
	{
		IndexVect.push_back((PointRows-3)*PointsPerRow+j);
		IndexVect.push_back((PointRows-3)*PointsPerRow+j+1);
		IndexVect.push_back((PointRows-2)*PointsPerRow+1);
	}
	IndexVect.push_back((PointRows-3)*PointsPerRow+j);
	IndexVect.push_back((PointRows-3)*PointsPerRow);
	IndexVect.push_back((PointRows-2)*PointsPerRow+1);

	//copy the indices into the array:
	Indices = new GLuint[IndexVect.size()];  //allocate the required memory
	for (i = 0; i < IndexVect.size(); i++)
	{
		Indices[i] = IndexVect[i];
	}
	NumIndices = IndexVect.size();
	IndexVect.clear();  //no longer needed, takes only memory
}


int main()
{
	glfwInit();

	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(1280, 720, 0, 0, 0, 0, 32, 0, GLFW_WINDOW);

	glewInit();

	glfwSetWindowTitle("OpenGL");

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	
	//Enable VSync
	glfwSwapInterval(1);
	
	glfwSetKeyCallback(&key_callback);
	
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	
	CreateSphere(16,16);
	//Enable the vertex array functionality:
	glEnableClientState(GL_VERTEX_ARRAY);
	//Enable the color array functionality (so we can specify a color for each vertex)
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(	3,   //3 components per vertex (x,y,z)
						GL_FLOAT,
						sizeof(SVertex),
						Vertices);
	//pass the color pointer
	glColorPointer(		3,   //3 components per vertex (r,g,b)
						GL_FLOAT,
						sizeof(SVertex),
						&Vertices[0].r);  //Pointer to the first color

	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(programID);
		glDrawElements(	GL_TRIANGLES, //mode
						NumIndices,  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						Indices);
		
		glfwSwapBuffers();
		glFlush();

		// Exit on window close
		if (!glfwGetWindowParam(GLFW_OPENED))
			running = false;
	}

	glfwTerminate();
	return 0;
}
