#include "camera.h"

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime){

        float velocity = MovementSpeed * deltaTime;
        if(direction == FORWARD)
            Position += Front * velocity;
        if(direction == BACKWARD)
            Position -= Front * velocity;
        if(direction == RIGHT)
            Position += Right * velocity;
        if(direction == LEFT)
            Position -= Right * velocity;
        if(direction == UP)
            Position += glm::cross(glm::normalize(glm::cross(Front,Up)),Front) * velocity;
        if(direction == DOWN)
            Position -= glm::cross(glm::normalize(glm::cross(Front,Up)),Front) * velocity;
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(Position,Position + Front,Up);
}

void Camera::CameraUpdateVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw))*cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw))*cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front,WorldUp));
    Up = glm::normalize(glm::cross(Right,Front));
}

void Camera::ProcessMouseMovement(float xoffset,float yoffset, GLboolean constrainPitch){
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
    CameraUpdateVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY){
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    CameraUpdateVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY){
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        CameraUpdateVectors();
}

glm::vec3 Camera::GetPos()const{
    return Position;
}