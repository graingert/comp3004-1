#include "utils.hpp"

#include <vector>


using namespace std;
namespace graingert{
	class Mesh{

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
}
