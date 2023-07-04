#include <iostream>
#include <stdlib.h>

#include "renderer.h"
#include "object_manager.hpp"
#include "shader_path.h"
#include "intersection.h"

int main(int argc, char *argv[]) {
    std::cout << "The renderer started" << std::endl;

    Renderer renderer = Renderer();
    GLFWwindow* window = renderer.window;

    ///////////////////////////////
    /////////OBJECT CREATION///////
    ///////////////////////////////
    ObjectManager manager;
    SimpleMesh gridMesh = SimpleMesh();

    CreateGrid(gridMesh, 5, 5, 10.0f);

    manager.loadMesh("gridMesh", gridMesh);
    manager.loadShader("shader_test", SHADER_PATH"/test.v0.vert", SHADER_PATH"/test.v0.frag");
    manager.loadTexture("gandalf", TEXTURE_PATH"/gandalf.png");

    Object* obj = manager.createObject("gridMesh", "shader_test");
    obj->translation = glm::vec3(0 ,0, -100);
    obj->rotation = glm::vec3(90,0,0);
    obj->updateModelMatrix();
    obj->useTexture("gandalf", manager.getTextureID("gandalf"));
    renderer.addObject(obj);

    /////////// OBJECT TO KNOW IF INSIDE
    SimpleMesh meshCube;
    CreateBox(meshCube, 1, 1, 1);

    manager.loadMesh("meshCube", meshCube);
    manager.loadShader("normals", SHADER_PATH"/test.v0.vert", SHADER_PATH"/normals.frag");

    Object* obj2 = manager.createObject("meshCube", "normals");
    obj2->translation = glm::vec3(0,0,-10);
    obj2->scaling = glm::vec3(1);
    obj2->updateModelMatrix();
    obj2->useTexture("gandalf", manager.getTextureID("gandalf"));
    renderer.addObject(obj2);

    Object* obj3 = manager.createObject("meshCube", "normals");
    obj3->translation = glm::vec3(0,-5,-10);
    obj3->scaling = glm::vec3(0.1f);
    obj3->updateModelMatrix();
    obj3->useTexture("gandalf", manager.getTextureID("gandalf"));
    renderer.addObject(obj3);

    // Axis rendering
    Object* xaxis = manager.createObject("meshCube", "normals");
    xaxis->translation = obj3->translation;
    xaxis->scaling = glm::vec3(100.0f, 0.01f, 0.01f);
    xaxis->updateModelMatrix();
    renderer.addObject(xaxis);
    Object* yaxis = manager.createObject("meshCube", "normals");
    yaxis->translation = obj3->translation;
    yaxis->scaling = glm::vec3(0.01f, 100.0f, 0.01f);
    yaxis->updateModelMatrix();
    renderer.addObject(yaxis);
    Object* zaxis = manager.createObject("meshCube", "normals");
    zaxis->translation = obj3->translation;
    zaxis->scaling = glm::vec3(0.01f, 0.01f, 100.0f);
    zaxis->updateModelMatrix();
    renderer.addObject(zaxis);


    // Render loop
    while (!glfwWindowShouldClose(window)){
        // Input && controls
        renderer.cameraInput();

        // Render
        renderer.render();

        is_inside(renderer, manager, *obj2, obj3->toWorld(obj3->center()));
    }

    glfwTerminate();
    return 0;
}
