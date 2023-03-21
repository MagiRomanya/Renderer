#include "renderer.h"
#include "object_manager.hpp"
#include "utilities.h"

Renderer::~Renderer(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    delete littleAxis;
}

GLFWwindow* Renderer::CreateWindow(){
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

    // Mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress )){
        std::cout << "Failed to intitialize GLAD" << std::endl;
        exit(-1);
    }
    // Z testing
    glEnable(GL_DEPTH_TEST);
    return window;
}

void Renderer::renderGUI(){
    if (objects.size() == 0) return;
    // Start the frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Transform");

        std::vector<std::string> items;
        for (int i= 0; i < objects.size(); i++){
            items.push_back(objects[i]->name);
        }
        static int selected_object = 0;

        if (ImGui::BeginCombo("Object", items[selected_object].c_str())){
            for (int i=0; i < items.size(); i++){
                bool is_selected = (selected_object == i);
                if (ImGui::Selectable(items[i].c_str(), is_selected)){
                    selected_object = i;
                }
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        Object* obj = objects[selected_object];

        ImGui::Checkbox("Wireframe", &obj->render_as_wireframe);
        // Object transformation sliders
        bool changed = false;
        ImGui::Text("Translation");
        changed = ImGui::SliderFloat("tX", &(obj->translation.x), -30.0f, 30.0f) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("tX_direct", &(obj->translation.x)) or changed;
        changed = ImGui::SliderFloat("tY", &(obj->translation.y), -30.0f, 30.0f) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("tY_direct", &(obj->translation.y)) or changed;
        changed = ImGui::SliderFloat("tZ", &(obj->translation.z), -30.0f, 30.0f) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("tZ_direct", &(obj->translation.z)) or changed;

        ImGui::Text("Rotation");
        changed = ImGui::SliderFloat("rX", &(obj->rotation.x), -180.0f, 180.0f) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("rX_direct", &(obj->rotation.x)) or changed;
        changed = ImGui::SliderFloat("rY", &(obj->rotation.y), -180.0f, 180.0f) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("rY_direct", &(obj->rotation.y)) or changed;
        changed = ImGui::SliderFloat("rZ", &(obj->rotation.z), -180.0f, 180.0f) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("rZ_direct", &(obj->rotation.z)) or changed;

        ImGui::Text("Scaling");
        static const float maxSlider = obj->scaling.x * 30.0f;
        changed = ImGui::SliderFloat("sX", &(obj->scaling.x), -maxSlider, maxSlider) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("sX_direct", &(obj->scaling.x)) or changed;
        changed = ImGui::SliderFloat("sY", &(obj->scaling.y), -maxSlider, maxSlider) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("sY_direct", &(obj->scaling.y)) or changed;
        changed = ImGui::SliderFloat("sZ", &(obj->scaling.z), -maxSlider, maxSlider) or changed;
        ImGui::SameLine();
        changed = ImGui::InputFloat("sZ_direct", &(obj->scaling.z)) or changed;
        static float global_scaling;
        if (ImGui::InputFloat("Global Scaling", &global_scaling)){
            obj->scaling.x = global_scaling;
            obj->scaling.y = global_scaling;
            obj->scaling.z = global_scaling;
            obj->updateModelMatrix();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (changed)
            obj->updateModelMatrix();
        ImGui::End();

        // Draw gui elements
        for (size_t i = 0; i < m_gui_elements.size(); i++){
            m_gui_elements[i]->draw();
        }
    }


    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < objects.size(); i++){
        Object* obj = objects[i];
        if (obj->render_as_wireframe)
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        obj->render();
    }

    for (int i = 0; i < debugQueue.size(); i++){
        debugQueue[i].render();
    }
    debugQueue.clear();

    glClear(GL_DEPTH_BUFFER_BIT);
    littleAxis->render();
    this->renderGUI();
    this->resize_framebuffer();

    swapAndPoll();
}

void Renderer::resize_framebuffer(){
    /* Checks wether the window has changed resolution and updates
     * the aspect ratio and viewport with the new resolution and aspect ratio */
    int height, width;
    glfwGetWindowSize(window, &width, &height);

    // If the resolution has not changed do nothing
    if ((screenWidth != width) && (screenHeight == height)) return;

    screenWidth = width;
    screenHeight = height;

    // Updates viewpoint
    glViewport(0, 0, width, height);
    for (int i = 0; i < objects.size(); i++)
        objects[i]->proj = camera.GetProjMatrix(width, height);
}

void Renderer::addObject(Object* obj){
    obj->view = camera.GetViewMatrix();
    obj->proj = camera.GetProjMatrix(WIDTH, HEIGHT);
    objects.push_back(obj);
}

void Renderer::deleteObject(Object *obj){
    std::vector<Object*> new_objects;
    new_objects.reserve(objects.size() - 1);
    for (int i =0; i < objects.size(); i++){
        Object* other = objects[i];
        if (other != obj){
            new_objects.push_back(other);
        }
    }
    objects = new_objects;
}

void Renderer::cameraInput(){
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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.MovementSpeed = 17.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
        camera.MovementSpeed = 2.5f;
    }

    // Enable / disable orbital cam
    static SimpleTrigger<bool> trigger(false);
    if (trigger.changed_to_true( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )){
        camera.is_orbital = !camera.is_orbital;
        if (camera.is_orbital)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    glm::mat4 view = camera.GetViewMatrix();
    for (int i=0; i < objects.size(); i++){
        objects[i]->view = view;
    }

    // Mouse controlls
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    static float lastX = xpos;
    static float lastY = ypos;
    float deltaX = xpos - lastX;
    float deltaY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(deltaX, deltaY);
}

void Renderer::swapAndPoll(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}
void Renderer::addObjectQueue(Object &obj, glm::vec3 pos, float size) {
    // TODO: Understand the sign of this position
    obj.translation = -pos;
    obj.scaling = glm::vec3(size);
    obj.view = camera.GetViewMatrix();
    int height, width;
    obj.proj = camera.GetProjMatrix();
    obj.updateModelMatrix();

    debugQueue.push_back(obj);
}
