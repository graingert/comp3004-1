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
#define BLUE (glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ))
#define GREEN (glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ))
#define WHITE (glm::vec4 ( 1.0f, 1.0f, 1.0f, 1.0f ))
#define LIGHT_BLUE (glm::vec4 (0x07/255.0f, 0x76/255.0f, 0xA0/255.0f, 1.0f))
#define SAND (glm::vec4 (0xcd/255.0f, 0xa8/255.0f, 0x64/255.0f, 1.0f))

namespace graingert {
	class Vertex {
		public:
			glm::vec4 position;
			glm::vec4 color;
			glm::vec3 normal;
	};
}

glm::vec3 calc_tween(glm::vec3 a, glm::vec3 b, float ratio);
float calc_ratio(float a, float b, float d);

typedef struct Camera{
	glm::vec3 pos;
	glm::vec3 dir;
} Camera;

typedef struct KeyFrame{
	float time;
	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;
} KeyFrame;

typedef struct PosFrame{
	float time;
	glm::vec3 pos;
} PosFrame;

#endif
