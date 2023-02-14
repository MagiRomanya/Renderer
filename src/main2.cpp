#include <iostream>
#include <stdlib.h>

#include "renderer.h"

Renderer renderer = Renderer();

int main(int argc, char *argv[]) {

    std::cout << "The renderer started" << std::endl;

    GLFWwindow* window = renderer.window;
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    ///////////////////////////////
    SimpleMesh mesh = SimpleMesh();

    CreateGrid(mesh, 5, 5, 10.0f);

    Shader shader_test = Shader("../shaders/test.v0.vert", "../shaders/test.v0.frag");

    Object obj = Object(&mesh, shader_test);
    obj.translation = glm::vec3(0 ,0, -100);
    obj.rotation = glm::vec3(90,0,0);
    obj.updateModelMatrix();
    obj.loadTexture("gandalf", "../img/gandalf.png");
    renderer.addObject(&obj);

    SimpleMesh mesh2;
    Shader shader_normal = Shader("../shaders/test.v0.vert", "../shaders/normals.frag");
    // mesh2.loadFromFile("../img/bunny.obj");
    CreateBox(mesh2, 1, 1, 1);
    Object obj2 = Object(&mesh2, shader_normal);
    obj2.translation = glm::vec3(0,0,-10);
    obj2.scaling = glm::vec3(1);
    obj2.updateModelMatrix();
    obj2.loadTexture("gandalf", "../img/gandalf.png");
    renderer.addObject(&obj2);

    // Render loop
    while (!glfwWindowShouldClose(window)){
        // Input
        renderer.cameraInput();

        // Render
        renderer.render();

        // Swap buffers + check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
