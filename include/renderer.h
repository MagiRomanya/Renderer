#ifndef RENDERER_H_
#define RENDERER_H_

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "object.h"
#include "shader_path.h"
#include "gui_element.hpp"
#include "little_axis.hpp"

#define HEIGHT 800
#define WIDTH 600


class Renderer{
    public:
        GLFWwindow* window;

        std::vector<Object*> objects;
        std::vector<Object> debugQueue;

        Camera camera;

        Renderer() {
            // Create window and opengl context
            window = CreateWindow();
            camera = Camera();
            camera.Position = glm::vec3(0,0,1);
            littleAxis = new LittleAxis(&camera);
        }

        ~Renderer();

        inline bool windowShouldClose() { return glfwWindowShouldClose(window); }

        void cameraInput();

        void addObject(Object *obj);

        void addObjectQueue(Object &obj, glm::vec3 pos, float size);

        void deleteObject(Object *obj);

        // void addDebugCube(glm::vec3 pos, float size);

        void renderGUI();

        void render();

        void swapAndPoll();

        inline void add_GUI_element(GUI_element* element) { m_gui_elements.push_back(element); };


    private:
        void destroyQueue();

        void resize_framebuffer();

        GLFWwindow* CreateWindow();

        int screenWidth, screenHeight;

        std::vector<GUI_element*> m_gui_elements;

        LittleAxis* littleAxis;
};

#endif // RENDERER_H_
