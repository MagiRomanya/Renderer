#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "object.h"
#include "renderer.h"
#include "object_manager.hpp"

struct intersection_point{
    bool intersected;
    float t;
};



bool is_inside_triangle(const Triangle &tri, glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 point);

intersection_point ray_intersects_triangle(Renderer& renderer, ObjectManager &m, const Object &obj, const Triangle &tri, glm::vec3 direction, glm::vec3 position);
intersection_point ray_intersects_triangle(const Object &obj, const Triangle &tri, glm::vec3 direction, glm::vec3 position);

bool is_inside(Renderer &renderer, ObjectManager &m, const Object &obj, glm::vec3 point);
bool is_inside(const Object &obj, glm::vec3 point);

#endif // INTERSECTION_H_
