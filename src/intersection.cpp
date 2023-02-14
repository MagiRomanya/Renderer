#include "intersection.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

bool is_inside_triangle(glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 point){
    glm::vec3 normal = glm::normalize(glm::cross(xb-xa, xc-xa));

    if ( glm::dot(glm::cross(xb-xa, point -xa), normal) >= 0 ) return true;
    if ( glm::dot(glm::cross(xc-xb, point -xb), normal) >= 0 ) return true;
    if ( glm::dot(glm::cross(xa-xc, point -xc), normal) >= 0 ) return true;

    return false;
}

intersection_point ray_intersects_triangle(glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 direction, glm::vec3 position){
    /* Checks if a ray intersects with a triangle with sides xa, xb & xc */

    intersection_point p = {false, 0.0f};

    // First check the intersection between the plane and the ray
    glm::vec3 normal = glm::normalize(glm::cross(xb-xa, xc-xa));
    float n_dot_v = glm::dot(normal, direction);

    if (n_dot_v == 0.0f) return p; // If the plane and the line are parallel there is no intersection

    float t = glm::dot(normal, position - xa) / n_dot_v;
    glm::vec3 plane_ray_intersection = position + t * direction;

    // Check if the intersection point is inside the triangle
    p.intersected = is_inside_triangle(xa, xb, xc, plane_ray_intersection);
    p.t = t;
    return p;
}

// NOTE: Untested code, need to see if it makes sense.
// RESOURCES: https://en.wikipedia.org/wiki/Point_in_polygon
//            https://courses.cs.washington.edu/courses/cse457/09sp/lectures/triangle_intersection.pdf
//            https://www.usna.edu/Users/oceano/raylee/SM223/Ch12_5_Stewart(2016).pdf
bool is_inside(const SimpleMesh &m, glm::vec3 point){
    /* Checks weather or not a point is inside the volume of the mesh */
    bool inside = false;
    // Cast 3 rays and count how many intersactions are there
    unsigned int after;
    unsigned int before;
    glm::vec3 directions[] = {glm::vec3(1.0f, 0.0f, 0.0f),  // x direction
                              glm::vec3(0.0f, 1.0f, 0.0f),  // y direction
                              glm::vec3(0.0f, 0.0f, 1.0f)}; // z direction

    for (int d = 0; d < 3; d++) {
        after = 0;
        before = 0;
        for (int i = 0; i < m.triangles.size(); i++) {
            const Triangle &t = m.triangles[i];
            intersection_point p = ray_intersects_triangle(
                m.vertices[t.a].Position, m.vertices[t.b].Position,
                m.vertices[t.c].Position, directions[d],
                point);
            if (p.intersected){
                if (p.t < 0.0f){
                    before++;
                }
                else if (p.t > 0.0f){
                    after++;
                }
                else if (p.t == 0.0f){
                    return false;
                }
            }
        }
        if (before % 2 == 0) return false;
        if (after % 2 == 0)  return false;
    }
    // If the three rays have intersected an odd number of times before and after our point, we
    // can say the point is inside our mesh
    return true;
}
