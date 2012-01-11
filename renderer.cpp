#include "renderer.hpp"
namespace graingert{
	
	Renderer::Renderer(GLuint program, glm::mat4 mv_matrix,glm::mat4 p_matrix){
		_program = program;
		_mv_matrix = mv_matrix;
		_p_matrix = p_matrix;
		
		glUseProgram(_program);
		mv_matrix_id = glGetUniformLocation(_program, "mv_matrix");
		p_matrix_id = glGetUniformLocation(_program, "p_matrix");
	}
	
	void Renderer::bind(){
		glUseProgram(_program);
		glUniformMatrix4fv(mv_matrix_id, 1, GL_FALSE, &_mv_matrix[0][0]);
		glUniformMatrix4fv(p_matrix_id, 1, GL_FALSE, &_p_matrix[0][0]);
	}
	
}
