#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "object.h"

struct intersection_point{
    bool intersected;
    float t;
};



bool is_inside_triangle(const std::unordered_map<Edge, Edge> &map,const Triangle &tri, glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 point);
bool is_inside_triangle(const Object &obj, const Triangle &tri, glm::vec3 point);

bool is_inside(const Object &obj, glm::vec3 point);

#endif // INTERSECTION_H_
