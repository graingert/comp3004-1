#include "utils.hpp";

using namespace std;
namespace graingert{
	class Mesh{

		
		GLuint vbo[];
		protected:
			vector<Vertex> vertices;
			vector<GLuint> indices;
			void calculate_normals();
			void buffer();
		
		public:
			void bind();
			void unbind();
	}
}
