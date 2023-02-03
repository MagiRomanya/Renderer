#ifndef RENDERER_H_
#define RENDERER_H_

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "object.h"

#define HEIGHT 800
#define WIDTH 600


class Renderer{
    public:
    GLFWwindow* window;

    std::vector<Object*> objects;
    Camera camera;

    Renderer(){
        camera = Camera();
        window = CreateWindow();
    }

    void renderGUI();

    void render();


    private:
    GLFWwindow* CreateWindow();

};

#endif // RENDERER_H_
