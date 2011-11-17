#ifndef G_UTILS
#define G_UTILS

#include <GL/glew.h>
#include <GL/glu.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define PI  (3.14159265359)
#define TAU (6.28318530718)
#define RED (glm::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ))
#define BLUE (glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ))
#define GREEN (glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ))

namespace graingert {
	class Vertex {
		public:
			glm::vec4 position;
			glm::vec4 color;
			glm::vec3 normal;
	};
}

#endif
