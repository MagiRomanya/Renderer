#include "intersection.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

/*
RESOURCES:
https://en.wikipedia.org/wiki/Point_in_polygon
https://courses.cs.washington.edu/courses/cse457/09sp/lectures/triangle_intersection.pdf
https://www.usna.edu/Users/oceano/raylee/SM223/Ch12_5_Stewart(2016).pdf
*/

bool is_inside_triangle(const Triangle &tri,
                        glm::vec3 xa, glm::vec3 xb, glm::vec3 xc, glm::vec3 point){

  glm::vec3 normal = glm::normalize(glm::cross(xb - xa, xc - xa));
  float condition;
  Edge edge;
  glm::vec3 coord[] = {xb, xc, xa};
  glm::vec3 coord2[] = {xa, xb, xc};

  int nodes[] =  {tri.b, tri.c, tri.a};
  int nodes2[] =  {tri.a, tri.b, tri.c};

  for (int i = 0; i < 3; i++){
      condition = glm::dot(glm::cross(coord[i] - coord2[i], point - coord2[i]), normal);
      edge = Edge(nodes[i], nodes2[i], -1);
      if (condition < 0.0f) return false;
      if (condition == 0.0f) { // Intersection
          if (edge.a > edge.b)
              return false;
      }
  }

  return true;
  // How this used to work
  return glm::dot(glm::cross(xb - xa, point - xa), normal) >= 0.0f &&
         glm::dot(glm::cross(xc - xb, point - xb), normal) >= 0.0f &&
         glm::dot(glm::cross(xa - xc, point - xc), normal) >= 0.0f;
}

intersection_point ray_intersects_triangle(Renderer& renderer, ObjectManager &m, const Object &obj, const Triangle &tri, glm::vec3 direction, glm::vec3 position){
    /* Checks if a ray intersects with a triangle with sides xa, xb & xc */
    // Triangle vertex in worlds position
    const glm::vec3& xa = obj.toWorld(obj.mesh->vertices[tri.a].Position);
    const glm::vec3& xb = obj.toWorld(obj.mesh->vertices[tri.b].Position);
    const glm::vec3& xc = obj.toWorld(obj.mesh->vertices[tri.c].Position);

    intersection_point p = {false, 0.0f};

    // First check the intersection between the plane and the ray
    glm::vec3 normal = glm::normalize(glm::cross(xb-xa, xc-xa));
    float n_dot_v = glm::dot(normal, direction);

    if (n_dot_v == 0.0f) return p; // If the plane and the line are parallel there is no intersection

    float t = -glm::dot(normal, position - xa) / n_dot_v;
    glm::vec3 plane_ray_intersection = position + t * direction;

    // Check if the intersection point is inside the triangle
    p.intersected = is_inside_triangle(tri, xa, xb, xc, plane_ray_intersection);
    p.t = t;

    if (p.intersected){
        // Draw a cube in the position where de ray hits the triangle
        Object* obj = m.createObject("meshCube", "normals");
        renderer.addObjectQueue(*obj, plane_ray_intersection, 0.3f);
    }
    return p;
}

bool is_inside(Renderer& renderer, ObjectManager &manager, const Object &obj, glm::vec3 point){
    /* Checks weather or not a point (in world coordinates) is inside the volume of the mesh */
    // Cast 3 rays and count how many intersactions with the mesh are there
    const SimpleMesh& m = *obj.mesh;
    unsigned int after;
    unsigned int before;
    glm::vec3 directions[] = {glm::vec3(1.0f, 0.0f, 0.0f),  // x direction
                              glm::vec3(0.0f, 1.0f, 0.0f),  // y direction
                              glm::vec3(0.0f, 0.0f, 1.0f)}; // z direction

    const char chardir[3] = {'x', 'y', 'z'};
    for (int d = 0; d < 3; d++) {
        after = 0;
        before = 0;
        for (int i = 0; i < m.triangles.size(); i++) {
            const Triangle &t = m.triangles[i];
            intersection_point p = ray_intersects_triangle(renderer, manager, obj, t, directions[d], point);

            if (p.intersected) {
              if (p.t < 0.0f) before++;
              else if (p.t > 0.0f) after++;
              else if (p.t == 0.0f) return false;
            }
        }
        // std::cout << chardir[d] << " A: " << after << " B: "<< before << std::endl;
        if (before % 2 == 0) return false;
        if (after % 2 == 0)  return false;
    }
    // If the three rays have intersected an odd number of times before and after our point, we
    // can say the point is inside our mesh
    return true;
}
bool is_inside(const Object &obj, glm::vec3 point){
    /* Checks weather or not a point is inside the volume of the mesh */
    // Cast 3 rays and count how many intersactions with the mesh are there
    const SimpleMesh& m = *obj.mesh;
    unsigned int after;
    unsigned int before;
    glm::vec3 directions[] = {glm::vec3(1.0f, 0.0f, 0.0f),  // x direction
                              glm::vec3(0.0f, 1.0f, 0.0f),  // y direction
                              glm::vec3(0.0f, 0.0f, 1.0f)}; // z direction

    const char chardir[3] = {'x', 'y', 'z'};
    for (int d = 0; d < 3; d++) {
        after = 0;
        before = 0;
        for (int i = 0; i < m.triangles.size(); i++) {
            const Triangle &t = m.triangles[i];
            intersection_point p = ray_intersects_triangle(obj, t, directions[d], point);

            if (p.intersected) {
              if (p.t < 0.0f) before++;
              else if (p.t > 0.0f) after++;
              else if (p.t == 0.0f) return false;
            }
        }
        // std::cout << chardir[d] << " A: " << after << " B: "<< before << std::endl;
        if (before % 2 == 0) return false;
        if (after % 2 == 0)  return false;
    }
    // If the three rays have intersected an odd number of times before and after our point, we
    // can say the point is inside our mesh
    return true;
}

intersection_point ray_intersects_triangle(const Object &obj, const Triangle &tri, glm::vec3 direction, glm::vec3 position){
    /* Checks if a ray intersects with a triangle with sides xa, xb & xc */
    // Triangle vertex in worlds position
    const glm::vec3& xa = obj.toWorld(obj.mesh->vertices[tri.a].Position);
    const glm::vec3& xb = obj.toWorld(obj.mesh->vertices[tri.b].Position);
    const glm::vec3& xc = obj.toWorld(obj.mesh->vertices[tri.c].Position);

    intersection_point p = {false, 0.0f};

    // First check the intersection between the plane and the ray
    glm::vec3 normal = glm::normalize(glm::cross(xb-xa, xc-xa));
    float n_dot_v = glm::dot(normal, direction);

    if (n_dot_v == 0.0f) return p; // If the plane and the line are parallel there is no intersection

    float t = -glm::dot(normal, position - xa) / n_dot_v;
    glm::vec3 plane_ray_intersection = position + t * direction;

    // Check if the intersection point is inside the triangle
    p.intersected = is_inside_triangle(tri, xa, xb, xc, plane_ray_intersection);
    p.t = t;

    return p;
}
