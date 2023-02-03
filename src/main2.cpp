#include <iostream>
#include <stdlib.h>

#include "renderer.h"

Renderer renderer = Renderer();

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char *argv[]) {

    std::cout << "The renderer started" << std::endl;

    GLFWwindow* window = renderer.window;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    ///////////////////////////////
    SimpleMesh mesh = SimpleMesh();

    CreateGrid(mesh, 5, 5, 10.0f);

    Shader shader_test = Shader("../shaders/test.v0.vert", "../shaders/test.v0.frag");

    Object obj = Object(&mesh, shader_test);
    obj.translation = glm::vec3(0,0,-10);
    obj.rotation = glm::vec3(0,3.14159f / 2,0);
    obj.scaling = glm::vec3(1.0f);
    obj.updateModelMatrix();
    obj.view = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));
    obj.proj = glm::perspective(30.0f, 1.0f, 0.1f, 100.f);

    obj.loadTexture("gandalf", "../img/gandalf.png");
    Object obj2 = obj;
    obj2.name = "xdd";
    obj2.model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f , 0.0f, -4.0f));
    obj2.model = glm::rotate(obj2.model, 3.14159f / 2.0f, glm::vec3(0, 0, 1));
    obj2.model = glm::rotate(obj2.model, 3.14159f / 2.0f, glm::vec3(1, 0, 0));

    renderer.objects.push_back(&obj);
    renderer.objects.push_back(&obj2);

    // Render loop
    while (!glfwWindowShouldClose(window)){
        // Input
        processInput(window);

        // Render
        renderer.render();

        // Swap buffers + check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    /* Gets called every time we resize the window */
    for (int i = 0; i < renderer.objects.size(); i++)
        renderer.objects[i]->proj = glm::perspective(30.0f, ( (float) width) / height, 0.1f, 100.f);
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window){
    /* Controlls user input through the window */
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    float deltaTime = 0.0f;
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        renderer.camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        renderer.camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        renderer.camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        renderer.camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Enable / disable orbital cam
    static bool escape_last_frame = false;
    bool escape_this_frame = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (escape_this_frame and (escape_this_frame != escape_last_frame)){
        renderer.camera.is_orbital = !renderer.camera.is_orbital;
        if (renderer.camera.is_orbital)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    escape_last_frame = escape_this_frame;
    glm::mat4 view = renderer.camera.GetViewMatrix();
    for (int i=0; i < renderer.objects.size(); i++){
        renderer.objects[i]->view = view;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static float lastX = WIDTH/2.0f;
    static float lastY = HEIGHT/2.0f;
    float deltaX = xpos - lastX;
    float deltaY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    renderer.camera.ProcessMouseMovement(deltaX, deltaY);
}
