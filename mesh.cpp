#include "mesh.hpp"
#include <vector>

using namespace std;

namespace graingert{
	Cone::Cone(int facets){
		vertices = vector<Vertex>();
		indices = vector<GLuint>();
	
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
}
