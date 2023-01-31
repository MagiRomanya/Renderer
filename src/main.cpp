#include <iostream>
#include <stdlib.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "shader.h"
#include "camara.h"
#include "object.h"

#define HEIGHT 800
#define WIDTH 600

GLFWwindow* CreateWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void render();

Object obj;
Camera camara;

int main(int argc, char *argv[]) {

    std::cout << "The renderer started" << std::endl;

    GLFWwindow* window = CreateWindow();

    ///////////////////////////////
    SimpleMesh mesh = SimpleMesh();

    CreateGrid(mesh, 5, 5, 1.0f);

    Shader shader_test = Shader("../shaders/test.v0.vert", "../shaders/test.v0.frag");

    obj = Object(mesh, shader_test);
    obj.model = glm::translate(obj.model, glm::vec3(0.0f , 0.0f, -4.0f));
    // obj.model = glm::rotate(obj.model, 3.14159f / 4.0f, glm::vec3(0, 1, 0));
    obj.view = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));
    obj.proj = glm::perspective(30.0f, 1.0f, 0.1f, 10.f);
    obj.loadTexture("gandalf", "../img/gandalf.png");

    // Render loop
    while (!glfwWindowShouldClose(window)){
        // Input
        processInput(window);

        // Render
        render();

        // Swap buffers + check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* CreateWindow(){
    /* Initializes openGL context and creates a resizable window with a callback */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Simulation", NULL, NULL);

    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    // Window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress )){
        std::cout << "Failed to intitialize GLAD" << std::endl;
        exit(-1);
    }

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    /* Gets called every time we resize the window */
    glViewport(0, 0, width, height);
    obj.proj = glm::perspective(30.0f, ( (float) width) / height, 0.1f, 10.f);
}


void processInput(GLFWwindow* window){
    /* Controlls user input through the window */
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    float deltaTime = 0.0f;
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camara.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camara.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camara.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camara.ProcessKeyboard(RIGHT, deltaTime);
    }
    obj.view = camara.GetViewMatrix();
}

void render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    obj.render();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static float lastX = WIDTH/2.0f;
    static float lastY = HEIGHT/2.0f;
    float deltaX = xpos - lastX;
    float deltaY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    camara.ProcessMouseMovement(deltaX, deltaY);
}
