#ifndef LITTLE_AXIS_H_
#define LITTLE_AXIS_H_

#include "mesh.h"
#include "shader.h"
#include "object_manager.hpp"
#include "camera.h"

#include "shader_path.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* Handles a little axis in the right top of the screen to
 * provide the user a constant sense of orientation */
class LittleAxis {
    public:
        LittleAxis(Camera* camera);
        void render();

    private:
        Shader shader;
        SimpleMesh mesh;
        Camera* camera;

        glm::mat4 calculateTransform();
};

#endif // LITTLE_AXIS_H_
