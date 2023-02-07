#include "renderer.h"

Renderer::~Renderer(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
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

    // Window resize callback
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPosCallback(window, mouse_callback);

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
        changed = ImGui::SliderFloat("rY", &(obj->rotation.y), -180.0f, 180.0f) or changed;
        changed = ImGui::SliderFloat("rZ", &(obj->rotation.z), -180.0f, 180.0f) or changed;

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

        if (changed)
            obj->updateModelMatrix();

        ImGui::End();
    }

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

    this->renderGUI();
    this->resize_framebuffer();
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

    // Enable / disable orbital cam
    static bool escape_last_frame = false;
    bool escape_this_frame = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (escape_this_frame and (escape_this_frame != escape_last_frame)){
        camera.is_orbital = !camera.is_orbital;
        if (camera.is_orbital)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    escape_last_frame = escape_this_frame;
    glm::mat4 view = camera.GetViewMatrix();
    for (int i=0; i < objects.size(); i++){
        objects[i]->view = view;
    }
}
