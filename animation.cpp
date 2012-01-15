#include "animation.hpp"
#include <iostream>
#include <cmath>
#include "utils.hpp"

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
		
		
		glm::mat4 spaceship(1);
		spaceship = glm::scale(spaceship, glm::vec3(1.,0.5,1.));
		spaceship = glm::translate(spaceship, glm::vec3(-1,0,1));
		_renderer->_mv_matrix = view * spaceship;
		_renderer->bind();
		sphereb->draw();
		
		glm::mat4 abduct(1);
		abduct = glm::scale(abduct, glm::vec3(1.,5, 1));
		abduct = glm::translate(abduct, glm::vec3(-1,-1,1));
		_renderer->_mv_matrix = view * abduct;
		_renderer->bind();
		cone->draw();
		
		_renderer->_mv_matrix = view * glm::translate(glm::mat4(1), glm::vec3(0,-5,0)) * glm::scale(glm::mat4(1), glm::vec3(20, 2, 20));
		_renderer->bind();
		terrian->draw();
		
		float miss_time = fmod(time, 10.0f);
		
		PosFrame frames[] {
			{0.0f, glm::vec3(10,-20,0)},
			{1.0f, glm::vec3(-2,0,1)},
			{1.0f, glm::vec3(0,-20,10)},
			{2.0f, glm::vec3(-2,0,1)},
			{2.0f, glm::vec3(10,-20,10)},
			{3.0f, glm::vec3(-2,0,1)},
		};
	
		for (int i = 0; i<6; i++){
			if (miss_time < frames[i].time){
				
				float ratio = calc_ratio(frames[i-1].time, frames[i].time, time);
				
				glm::mat4 missile(1);
				missile = glm::scale(missile, glm::vec3(0.3, 0.3, 0.3));
				
				glm::vec3 position = calc_tween(frames[i-1].pos,frames[i].pos,ratio);
				missile = glm::translate(missile, position);
				
				_renderer->_mv_matrix = view * missile;
				_renderer->bind();
				spherea->draw();	
			}
		}
		
		
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
		
		//_renderer->_mv_matrix = view * glm::scale(spherea_m, glm::vec3(3));
		//_renderer->bind();
		//spherea->draw();

		//_renderer->_mv_matrix = view * sphereb_m;
		//_renderer->bind();
		//sphereb->draw();
		
		
	}
}

