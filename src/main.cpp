#include <iostream>
#include <stdlib.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define HEIGHT 800
#define WIDTH 600

GLFWwindow* CreateWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render();

int main(int argc, char *argv[]) {

    std::cout << "Hello World!" << std::endl;

    GLFWwindow* window = CreateWindow();

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

    if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress )){
        std::cout << "Failed to intitialize GLAD" << std::endl;
        exit(-1);
    }

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    /* Gets called every time we resize the window */
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window){
    /* Controlls user input through the window */
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
