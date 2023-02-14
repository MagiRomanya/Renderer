#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "mesh.h"

struct intersection_point{
    bool intersected;
    float t;
};


intersection_point ray_intersects_triangle(glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 direction, glm::vec3 position);

bool is_inside_triangle(glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 point);

bool is_inside(const SimpleMesh &m, glm::vec3 point);

#endif // INTERSECTION_H_
