#include <iostream>
#include <stdlib.h>

#include "renderer.h"
#include "shader_path.h"
#include "intersection.h"



int main(int argc, char *argv[]) {
    std::cout << "The renderer started" << std::endl;

    Renderer renderer = Renderer();
    GLFWwindow* window = renderer.window;

    ///////////////////////////////
    SimpleMesh mesh = SimpleMesh();

    CreateGrid(mesh, 5, 5, 10.0f);

    Shader shader_test = Shader(SHADER_PATH"/test.v0.vert", SHADER_PATH"/test.v0.frag");

    Object obj = Object(&mesh, shader_test);
    obj.translation = glm::vec3(0 ,0, -100);
    obj.rotation = glm::vec3(90,0,0);
    obj.updateModelMatrix();
    obj.loadTexture("gandalf", TEXTURE_PATH"/gandalf.png");
    renderer.addObject(&obj);

    SimpleMesh mesh2;
    Shader shader_normal = Shader(SHADER_PATH"/test.v0.vert", SHADER_PATH"/normals.frag");
    // mesh2.loadFromFile("../img/bunny.obj");
    CreateBox(mesh2, 1, 1, 1);
    Object obj2 = Object(&mesh2, shader_normal);
    obj2.translation = glm::vec3(0,0,-10);
    obj2.scaling = glm::vec3(1);
    obj2.updateModelMatrix();
    obj2.loadTexture("gandalf", TEXTURE_PATH"/gandalf.png");
    renderer.addObject(&obj2);

    SimpleMesh mesh3;
    CreateBox(mesh3, 1, 1, 1);
    Object obj3 = Object(&mesh3, shader_normal);
    obj3.translation = glm::vec3(0,-5,-10);
    obj3.scaling = glm::vec3(0.1f);
    obj3.updateModelMatrix();
    obj3.loadTexture("gandalf", TEXTURE_PATH"/gandalf.png");
    renderer.addObject(&obj3);

    // Axis rendering
    Object xaxis = Object(&mesh3, shader_normal);
    xaxis.translation = obj3.translation;
    xaxis.scaling = glm::vec3(100.0f, 0.01f, 0.01f);
    xaxis.updateModelMatrix();
    renderer.addObject(&xaxis);
    Object yaxis = Object(&mesh3, shader_normal);
    yaxis.translation = obj3.translation;
    yaxis.scaling = glm::vec3(0.01f, 100.0f, 0.01f);
    yaxis.updateModelMatrix();
    renderer.addObject(&yaxis);
    Object zaxis = Object(&mesh3, shader_normal);
    zaxis.translation = obj3.translation;
    zaxis.scaling = glm::vec3(0.001f, 0.01f, 100.0f);
    zaxis.updateModelMatrix();
    renderer.addObject(&zaxis);

    // Render loop
    while (!glfwWindowShouldClose(window)){
        // Input
        renderer.cameraInput();

        // Render
        renderer.render();

        if (is_inside(obj2,
                      glm::vec3(obj3.model * glm::vec4(obj3.mesh->vertices[0].Position, 1.0f))))
            std::cout << "The object is inside" << std::endl;

        // Swap buffers + check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
