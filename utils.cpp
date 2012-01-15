#include "utils.hpp"

glm::vec3 calc_tween(glm::vec3 a, glm::vec3 b, float ratio){
	return (a*ratio) + (b*(1-ratio));
}

float calc_ratio(float a, float b, float d){
	return (d-b)/(a-b);
}
