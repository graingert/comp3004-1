#include "mesh.hpp"
#include <vector>
#include "models/gourd.h"
#include <iostream>
#include "libs/SOIL.h"


using namespace std;

namespace graingert{
	Mesh::Mesh(){
		vertices = vector<Vertex>();
		indices = vector<GLuint>();	
	}
	
	void Mesh::buffer(){
	
		glGenBuffers(1, &vertexbuffer);
		glGenBuffers(1, &indexbuffer);
		bind();
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW);
	}
	
	void Mesh::bind(){
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
		
		const size_t vertexSize = sizeof(Vertex);
		
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex,position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex,color));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex,normal));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
	
	void Mesh::unbind(){
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	
	void Mesh::draw(){
		bind();
		glDrawElements(	GL_TRIANGLES, //mode
			indices.size(),  //count, ie. how many indices
			GL_UNSIGNED_INT, //type of the index array
			(void*)0);
		unbind();
	}
	
	void Mesh::normalize(){
		//based on code from http://glm.g-truc.net/code.html
		for (int i=0; i<indices.size(); i+=3) {
			glm::vec3 const & a = vertices[indices[i]].position.xyz;
			glm::vec3 const & b = vertices[indices[(i+1)]].position.xyz;
			glm::vec3 const & c = vertices[indices[(i+2)]].position.xyz;
			
			glm::vec3 normal = glm::cross(c -a, b - a);
			  
			vertices[indices[i]].normal += normal;
			vertices[indices[(i+1)]].normal += normal;
			vertices[indices[(i+2)]].normal += normal;
		}
		
		for (int i=0; i<vertices.size(); i++) {
			vertices[i].normal = glm::normalize(vertices[i].normal);
		}
		
	}
	
	Cone::Cone(GLuint facets){
	
		Vertex v_top = {glm::vec4(.0f,1.0f,.0f,1.0f),glm::vec4(.0f),glm::vec3(.0f)};
		vertices.push_back(v_top);
		GLuint i_top = vertices.size() - 1;
		
		Vertex v_bottom = {glm::vec4(.0f,0.0f,.0f,1.0f),glm::vec4(.0f),glm::vec3(.0f)};
		vertices.push_back(v_bottom);
		GLuint i_bottom = vertices.size() - 1;
		
		GLuint idxs[facets];
		
		float slice_angle = TAU/ ((float) facets);
		
		for (int i=0; i<facets; i++){
			float theta = slice_angle * i;
			float x = 0.5f * cos (theta);
			float z = 0.5f * sin (theta);
			
			Vertex v = {glm::vec4(x,0.f,z,1.0f),GREEN,glm::vec3(1)};
			vertices.push_back(v);
			idxs[i] = vertices.size()-1;
		}
		
		wireCone(idxs, i_top, facets);
		wireCone(idxs, i_bottom, facets);
		
	}
	
	void Cone::wireCone(GLuint idxs[], GLuint center, GLuint n ){
		for (int i = 1; i<n; i++){
			indices.push_back(idxs[i-1]);
			indices.push_back(idxs[i]);
			indices.push_back(center);
		}
		
		//last face
		
		indices.push_back(idxs[0]);
		indices.push_back(idxs[n-1]);
		indices.push_back(center);
	}
	
	ISOSphere::ISOSphere(GLuint iterations){
		// Using IcoSphere method found at http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
		
		float t = (1.0f + sqrt(5.0f)) / 2;
		// create 12 vertices of a icosahedron
		Vertex isoVertices[] = {
			{glm::vec4( -1.0f,  t,  0.0f, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4(  1.0f,  t,  0.0f, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4( -1.0f, -t,  0.0f, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4(  1.0f, -t,  0.0f, 1.0f ), RED, glm::vec3(0.0f)},
	
			{glm::vec4(  0.0f, -1.0f,  t, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4(  0.0f,  1.0f,  t, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4(  0.0f, -1.0f, -t, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4(  0.0f,  1.0f, -t, 1.0f ), RED, glm::vec3(0.0f)},
	
			{glm::vec4(  t,  0.0f, -1.0f, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4(  t,  0.0f,  1.0f, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4( -t,  0.0f, -1.0f, 1.0f ), RED, glm::vec3(0.0f)},
			{glm::vec4( -t,  0.0f,  1.0f, 1.0f ), RED, glm::vec3(0.0f)},
		};
		// Normalize starting vertices
 for (int i = 0; i < 12; i++)
    {
        glm::vec4 v4;
        glm::vec3 v3;
        v4 = isoVertices[i].position;
        v3 = v4.xyz;
        isoVertices[i].position = glm::vec4(glm::normalize(v3), 1.0f);
    }
		vertices.insert(vertices.begin(), isoVertices, isoVertices + 12);
	
		// create 20 triangles of the icosahedron
		GLuint isoIndicesAr[] = {
			0, 11, 5, // 5 faces around point 0
			0, 5, 1,
			0, 1, 7,
			0, 7, 10,
			0, 10, 11,
			1, 5, 9, // 5 adjacent faces
			5, 11, 4,
			11, 10, 2,
			10, 7, 6,
			7, 1, 8,
			3, 9, 4, // 5 faces around point 3
			3, 4, 2,
			3, 2, 6,
			3, 6, 8,
			3, 8, 9,
			4, 9, 5, // 5 adjacent faces
			2, 4, 11,
			6, 2, 10,
			8, 6, 7,
			9, 8, 1,
		};
		std::vector<GLuint> isoIndices(isoIndicesAr, isoIndicesAr + (3 * 20)); 
		
		// refine triangles
		for (GLuint i = 0; i < iterations; i++) {
			std::vector<GLuint> newIndices;
			std::vector<GLuint>::iterator it;
			for (it = isoIndices.begin(); it < isoIndices.end(); std::advance(it, 3)) {
				GLuint idx[3];
				for (int j = 0; j < 3; j++) {
					idx[j] = *(it+j);
					if(idx[j] > vertices.size()) {
						assert(idx[j] < vertices.size());
					}
				}
				GLuint a = makeMiddlePoint(idx[0], idx[1]);
				GLuint b = makeMiddlePoint(idx[1], idx[2]);
				GLuint c = makeMiddlePoint(idx[2], idx[0]);
				GLuint idxToAdd[] = {
					idx[0], a, c,
					idx[1], b, a,
					idx[2], c, b,
					a, b, c,
				};
				for (int j = 0; j < 12; j++) {
					assert(idxToAdd[j] < vertices.size());
				}
				newIndices.insert(newIndices.begin(), idxToAdd, idxToAdd+12);
			}
			isoIndices = newIndices;
		}
		indices.insert(indices.begin(), isoIndices.begin(), isoIndices.end());
		
		//Because we are drawing a unit sphere the normals are the same as
		//the points.
		
		for (int i = 0; i < vertices.size(); i++){
			vertices[i].normal = vertices[i].position.xyz;
		}
	}
	
	// return index of point in the middle of p1 and p2
	GLuint ISOSphere::makeMiddlePoint(GLuint idx0, GLuint idx1) {
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
		// store it, return index
		vertices.push_back(middle);
		return vertices.size() - 1;
	}
	
	Gourd::Gourd(){
		//Loads objects generated using http://people.sc.fsu.edu/~jburkardt/pl_src/obj2opengl/obj2opengl.html
		
		for (int i=0; i<(numGourdVerts); i++){
			Vertex v;
			v.color = BLUE;
			v.position = glm::vec4(GourdVerts[i][0],GourdVerts[i][1], GourdVerts[i][2], 1);
			v.normal = glm::vec3(GourdVertNorms[i][0], GourdVerts[i][1], GourdVerts[i][2]);
			vertices.push_back(v);
		}
		
		
		for (int i=0; i<(numGourdFaces); i++){
			indices.push_back(GourdFaces[i][0]);
			indices.push_back(GourdFaces[i][1]);
			indices.push_back(GourdFaces[i][2]);
		}
	}
	
	int access(int x,int y,int width){
		return (y*width)+x;
	}
	
	Terrian::Terrian(){
		int height = 10;
		int width = 10;
		int channels;
		
		//from http://www.lonesock.net/soil.html
		unsigned char *ht_map = SOIL_load_image
			(
				"mountain.png",
				&width, &height, &channels,
				SOIL_LOAD_L
			);
		
		
		float f_height = float(height)/2.0f;
		float f_width = float(width)/2.0f;
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Vertex v;
				float z = ((float)ht_map[(y*width)+x]/255.0f);
				v.position = glm::vec4((y/f_height)-1, z, (x/f_width)-1, 1.0f);
				
				if (z<0.1){
					v.color = BLUE;
				} else if (z < 0.2){
					v.color = SAND;
				} else if (z > 0.6){
					v.color = WHITE;
				} else {
					v.color = GREEN;
				}
				v.normal = glm::vec3(1);
				vertices.push_back(v);
			}
		}
	
		for (int y = 1; y < height; y++) {
			for (int x = 0; x < width - 1; x++) {
				int a = access(x,y,width);
				int b = access(x,y-1,width);
				int c = access(x+1,y,width);
				int d = access(x+1,y-1,width);
				
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
	
				indices.push_back(b);
				indices.push_back(d);
				indices.push_back(c);
			}
		}
		normalize();
	}
}
