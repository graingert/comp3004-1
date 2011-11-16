#include "mesh.hpp"
#include <vector>

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
	}
	
	void Mesh::draw(){
		glDrawElements(	GL_TRIANGLES, //mode
			indices.size(),  //count, ie. how many indices
			GL_UNSIGNED_INT, //type of the index array
			(void*)0);
	}
	
	
	Cone::Cone(int facets){
	
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
			
			Vertex v = {glm::vec4(x,0.f,z,1.0f),glm::vec4(.0f),glm::vec3(.0f)};
			vertices.push_back(v);
			idxs[i] = vertices.size()-1;
		}
		
		wireCone(idxs, i_top, facets);
		wireCone(idxs, i_bottom, facets);
		
	}
	
	void Cone::wireCone(GLuint idxs[], GLuint center, int n ){
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
	
	UVSphere::UVSphere(int iterations){
		// Using IcoSphere method found at http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
		// Make Icosahedron
		float t = (1.0f + sqrt(5.0f)) / 2;
		// Have to manually do the 12 vertices
		Vertex startingVertices[] = {
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
		for (int i = 0; i < 12; i++) {
			startingVertices[i].position = glm::vec4(
				glm::normalize(startingVertices[i].position.xyz),
				1.0f
			);
		}
		vertices.insert(vertices.begin(), startingVertices, startingVertices + 12);
	
		// Make the 20 faces of the icosahedron
		GLuint startingIndices[] = {
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
		std::vector<GLuint> *localIndices = new std::vector<GLuint>(startingIndices, startingIndices + (3 * 20)); // Have us a local version so we can create & delete for each iteration. Copy into result vector later
		for (GLuint i = 0; i < iterations; i++) {
			std::vector<GLuint> *newIndices = new std::vector<GLuint>();
			std::vector<GLuint>::iterator it;
			for (it = localIndices->begin(); it < localIndices->end(); std::advance(it, 3)) {
				ushort idx[3];
	
				// Get the indices for this triangle
				for (int j = 0; j < 3; j++) {
					idx[j] = *(it+j);
					if(idx[j] > vertices.size()) {
						assert(idx[j] < vertices.size());
					}
				}
	
				ushort a = makeMiddlePoint(idx[0], idx[1]);
				ushort b = makeMiddlePoint(idx[1], idx[2]);
				ushort c = makeMiddlePoint(idx[2], idx[0]);
	
				ushort idxToAdd[] = { // Indices of the new triangles
					idx[0], a, c,
					idx[1], b, a,
					idx[2], c, b,
					a, b, c,
				};
	
				for (int j = 0; j < 12; j++) {
					assert(idxToAdd[j] < vertices.size());
				}
	
				newIndices->insert(newIndices->begin(), idxToAdd, idxToAdd+12);
			}
			delete localIndices;
			localIndices = newIndices;
	
			for (it = localIndices->begin(); it < localIndices->end(); it++) {
				assert(*it < vertices.size());
			}
		}
		indices.insert(indices.begin(), localIndices->begin(), localIndices->end()); // startingIndices+x where x is the number of items in startingIndices
		
		//Because we are drawing a unit sphere the normals are the same as
		//the points.
		
		for (int i = 0; i < vertices.size(); i++){
			vertices[i].normal = vertices[i].position.xyz;
		}
	}
	
	ushort UVSphere::makeMiddlePoint(ushort idx0, ushort idx1) {
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
		return vertices.size() - 1;
	}
		
}
