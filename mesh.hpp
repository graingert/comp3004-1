#ifndef G_MESH
#define G_MESH
#include "utils.hpp"

#include <vector>


using namespace std;
namespace graingert{
	class Mesh{
		private:
			GLuint vertexbuffer,indexbuffer;
		protected:
			Mesh();
		public:
			vector<Vertex> vertices;
			vector<GLuint> indices;
			void buffer();
			void bind();
			void unbind();
			void draw();
	};
	
	class Cone: public Mesh {
		public:
			Cone(GLuint facets);
		private:
			void wireCone(GLuint idxs[], GLuint center, GLuint n );
	};
	
	class ISOSphere: public Mesh {
		public:
			ISOSphere(GLuint iterations);
		private:
			GLuint makeMiddlePoint(GLuint idx0, GLuint idx1);
	};
	
	class Gourd: public Mesh {
		public:
			Gourd();
	};
	
	class Terrian: public Mesh{
		public:
			Terrian();
	};
}
#endif
