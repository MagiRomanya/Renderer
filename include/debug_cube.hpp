#ifndef DEBUG_CUBE_H_
#define DEBUG_CUBE_H_

#include "mesh.h"
#include "object.h"
#include "shader_path.h"
#include "renderer.h"

/* Debug cube is an easy way to add a cube in your scene */
class DebugCube{
    public:
        SimpleMesh mesh;
        Object obj;
        Renderer* renderer;

        DebugCube(Renderer *in_renderer, glm::vec3 pos, float size){
            CreateBox(mesh, size, size, size);
            Shader shader_normal = Shader(SHADER_PATH"/test.v0.vert", SHADER_PATH"/normals.frag");

            obj = Object(&mesh, shader_normal);
            obj.translation = pos;
            obj.updateModelMatrix();
            this->renderer = in_renderer;
            renderer->addObject(&obj);
        }

        ~DebugCube() {
            renderer->deleteObject(&obj);
        }
};

#endif // DEBUG_CUBE_H_
