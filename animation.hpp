#include "utils.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <ctime>

using namespace std;
namespace graingert{
	class Animation{
		private:
			Cone *cone;
			ISOSphere *spherea, *sphereb;
			Renderer *_renderer;
			glm::mat4 perspective,view,cone_m,spherea_m,sphereb_m;
			void setmatrix(float time);
			float oldtime;
		public:
			Animation(Renderer& renderer);
			void buffer();
			void draw(float time);
	};
}
