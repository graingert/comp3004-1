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
			void draw();
	};
	
	class Cone: public Mesh {
		public:
			Cone(int facets);
		private:
			void wireCone(GLuint idxs[], GLuint center, int n );
	};
	
	class UVSphere: public Mesh {
		public:
			UVSphere(int iterations);
		private:
			ushort makeMiddlePoint(ushort idx0, ushort idx1);
	};
}
