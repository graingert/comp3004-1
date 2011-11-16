#include "utils.hpp"
#include <vector>


using namespace std;
namespace graingert{
	
	class Renderer{
		public:
			Renderer(GLuint program, glm::mat4 mv_matrix,
				glm::mat4 p_matrix);
			GLuint _program;
			glm::mat4 _mv_matrix, _p_matrix;
			void bind();
		protected:
			GLuint mv_matrix_id, p_matrix_id;
	};
	
	class NormalsRenderer: public Renderer{
		public:
			NormalsRenderer(GLuint program, glm::mat4 mv_matrix,
				glm::mat4 p_matrix, float normals_length);
			float _normals_length;
			void bind();
		private:
			GLuint mv_matrix_id, p_matrix_id, normals_length_id;
	};
		
}
