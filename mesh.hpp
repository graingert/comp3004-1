#include "utils.hpp";

using namespace std;
namespace graingert{
	class Mesh{
		vector<Vertex> vertices;
		vector<GLuint> indices;
		
		GLuint vbo[];
		
		public:
			void bind();
			void unbind();
	}
}
