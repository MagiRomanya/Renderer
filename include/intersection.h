#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "object.h"
#include "renderer.h"

struct intersection_point{
    bool intersected;
    float t;
};



bool is_inside_triangle(const Triangle &tri, glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 point);

intersection_point ray_intersects_triangle(Renderer& renderer, const Object &obj, const Triangle &tri, glm::vec3 direction, glm::vec3 position);

bool is_inside(Renderer &renderer, const Object &obj, glm::vec3 point);

#endif // INTERSECTION_H_
