#include "utils.hpp"
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;
namespace graingert{
	class Camera{
		public:
			float speed;
			void step(float time);
			glm::mat4 view;
	};
}
