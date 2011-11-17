#include "animation.hpp"
#include "utils.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include <GL/glew.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>




using namespace std;
namespace graingert{
	Animation::Animation(Renderer& renderer){
		_renderer = &renderer;
		perspective = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
		view = glm::lookAt(glm::vec3(10,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
		cone = new Cone(10);
		spherea = new UVSphere(0);
		sphereb = new UVSphere(3);
		setmatrix();
	}
	
	void Animation::setmatrix(){
		cone_m = glm::translate( 0,0,2 );
		spherea_m = glm::translate( 0,1,0);
		sphereb_m = glm::translate( 0,-1,0);
	}
	
	void Animation::buffer(){
		cone->buffer();
		spherea->buffer();
		sphereb->buffer();
	}
	
	void Animation::draw(){
		setmatrix();
		_renderer->_p_matrix = perspective;
		_renderer->_mv_matrix = view * cone_m;
		_renderer->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cone->draw();
		_renderer->_mv_matrix = view * spherea_m;
		_renderer->bind();
		spherea->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		_renderer->_mv_matrix = view * sphereb_m;
		_renderer->bind();
		sphereb->draw();
	}
}

