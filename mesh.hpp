#include "utils.hpp"

#include <vector>


using namespace std;
namespace graingert{
	class Mesh{
		protected:
			Mesh();
		public:
			vector<Vertex> vertices;
			vector<GLuint> indices;
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
