#ifndef G_RENDERER
#define G_RENDERER
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
		
}
#endif
