#include <GL/glew.h>
#include <GL/glu.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace graingert {
	class Vertex {
		public:
			glm::vec4 position;
			glm::vec4 color;
			glm::vec3 normal;
	};
}
