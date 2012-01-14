#include "animation.hpp"
#include <iostream>
using namespace std;
namespace graingert{
	Animation::Animation(Renderer& renderer){
		_renderer = &renderer;
		perspective = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
		view = glm::lookAt(glm::vec3(10,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
		cone = new Cone(10);
		spherea = new ISOSphere(0);
		sphereb = new ISOSphere(3);
		gourd = new Gourd();
		terrian = new Terrian();
		oldtime = 0.0f;
		setmatrix(0.0f);
	}
	
	void Animation::setmatrix(float time){
		float t = time - oldtime;
		oldtime = time;
		
		float theta = time;
		
		float x = cos(theta);
		float y = sin(theta);
		glm::mat4 rotate = glm::rotate( glm::mat4(1), (float)(50.0 * time), glm::vec3(1,1,0));
		cone_m = glm::translate(rotate,glm::vec3(0.0f,3*y,3*x));
		spherea_m = glm::translate(rotate,glm::vec3(0.5*x,0.5*x,0.5*y));
		sphereb_m = glm::translate(glm::vec3(0.0,2*x,2*y));
	}
	
	void Animation::buffer(){
		cone->buffer();
		spherea->buffer();
		sphereb->buffer();
		gourd->buffer();
		terrian->buffer();
	}
	
	void Animation::draw(float time){
		setmatrix(time);
		_renderer->_p_matrix = perspective;
		
		_renderer->_mv_matrix = view;
		_renderer->bind();
		sphereb->draw();
		
		
		glm::mat4 moon(1);
		moon = glm::scale(moon, glm::vec3(0.3));
		moon = glm::translate(moon, glm::vec3(10,0,0));
		glm::mat4 rotate = glm::rotate(glm::mat4(1), (float)(50.0 * time), glm::vec3(0,1,0));
		_renderer->_mv_matrix = view * rotate * moon;
		_renderer->bind();
		sphereb->draw();
		
		_renderer->_mv_matrix = view * cone_m;
		_renderer->bind();
		gourd->draw();
		terrian->draw();
		
		//_renderer->_mv_matrix = view * glm::scale(spherea_m, glm::vec3(3));
		//_renderer->bind();
		//spherea->draw();

		//_renderer->_mv_matrix = view * sphereb_m;
		//_renderer->bind();
		//sphereb->draw();
		
		
	}
}

