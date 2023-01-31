#ifndef CAMARA_H_
#define CAMARA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camara{
    public:
    // camera Attributes
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // euler angles:
    float phi = 0.0f;
    float theta = 0.0f;

    // Movement parameters
    float movement_speed = 1.0f;
    float mouse_sensitivity = 1.0f;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj;

    Camara(){
        updateCameraVectors();
        view = glm::mat4(1.0f);
        const float fov = 60; // in degrees
        const float aspect = 1.0f;
        const float near = 0.3f;
        const float far = 10.0f;
        proj = glm::perspective(fov, aspect, near, far);
        view = glm::lookAt(Position, Front - Position, Up);
    }

    glm::mat4 calculate_view(){
        view = glm::lookAt(Position, Front - Position, Up);
        return view;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = movement_speed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= mouse_sensitivity;
        yoffset *= mouse_sensitivity;

        phi   += xoffset;
        theta += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (theta > 89.0f)
                theta = 89.0f;
            if (theta < -89.0f)
                theta = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(phi)) * cos(glm::radians(theta));
        front.y = sin(glm::radians(theta));
        front.z = sin(glm::radians(phi)) * cos(glm::radians(theta));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

};

#endif // CAMARA_H_
