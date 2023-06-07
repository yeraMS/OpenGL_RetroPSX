#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../man/utils/types.hpp"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  70.0f; //tambien FOV


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    bool movement;
    glm::vec3 Position;
    glm::vec3 Front; 
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 LastPosition;
    //COLISION TESTS
    int w_;
    int s_;
    int a_;
    int d_;
    int last_q;
    int last_last_q;
    int pressed_q;
    int pressed_space;
    int last_pressed_space;
    int granjero_quemado{0};
    int reposition_lvl_camera;
    int marco_lvl4=0;
    int anillo_rec=0;
    int pulsation_release=0;
    int charge_plane = 0;
    int charge_status = 1;
    bool colision = false;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float velocity;
    bool LanternIsHeld;
    //float Zoom;


    //PLANOS PARA EL FRUSTRUM y funciones de ayuda
    struct Plane
    {
        float a, b, c, d;
    };

    Plane frustumPlanes[6];

    void NormalizePlane(Plane & plane)
    {
        float mag;
        mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);
        plane.a = plane.a / mag;
        plane.b = plane.b / mag;
        plane.c = plane.c / mag;
        plane.d = plane.d / mag;
    }
    
    float DistanceToPoint(const Plane & plane, glm::vec3 pt)
    {
        return plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d; //si dist < 0, P esta en espacio negativo, si = 0 en plano, si >0 espacio positivo
    }
    

    bool ClassifyPoint(const Plane & plane, glm::vec3 pt)
    {
        float d;
        d = DistanceToPoint(plane, pt);
        if (d < 0) return false;
        if (d > 0) return true;
        return true;
    }

    void ExtractPlanesGL(const glm::mat4 & comboMatrix,bool normalize)
    {   
        /*
        //left clipping 
        frustumPlanes[0].a = comboMatrix[3][0] + comboMatrix[0][0];
        frustumPlanes[0].b = comboMatrix[3][1] + comboMatrix[0][1];
        frustumPlanes[0].c = comboMatrix[3][2] + comboMatrix[0][2];
        frustumPlanes[0].d = comboMatrix[3][3] + comboMatrix[0][3];
        // Right clipping plane
        frustumPlanes[1].a = comboMatrix[3][0] - comboMatrix[0][0];
        frustumPlanes[1].b = comboMatrix[3][1] - comboMatrix[0][1];
        frustumPlanes[1].c = comboMatrix[3][2] - comboMatrix[0][2];
        frustumPlanes[1].d = comboMatrix[3][3] - comboMatrix[0][3];
        // Top clipping plane
        frustumPlanes[2].a = comboMatrix[3][0] - comboMatrix[1][0];
        frustumPlanes[2].b = comboMatrix[3][1] - comboMatrix[1][1];
        frustumPlanes[2].c = comboMatrix[3][2] - comboMatrix[1][2];
        frustumPlanes[2].d = comboMatrix[3][3] - comboMatrix[1][3];
        // Bottom clipping plane
        frustumPlanes[3].a = comboMatrix[3][0] + comboMatrix[1][0];
        frustumPlanes[3].b = comboMatrix[3][1] + comboMatrix[1][1];
        frustumPlanes[3].c = comboMatrix[3][2] + comboMatrix[1][2];
        frustumPlanes[3].d = comboMatrix[3][3] + comboMatrix[1][3];
        // Near clipping plane
        frustumPlanes[4].a = comboMatrix[3][0] + comboMatrix[2][0];
        frustumPlanes[4].b = comboMatrix[3][1] + comboMatrix[2][1];
        frustumPlanes[4].c = comboMatrix[3][2] + comboMatrix[2][2];
        frustumPlanes[4].d = comboMatrix[3][3] + comboMatrix[2][3];
        // Far clipping plane
        frustumPlanes[5].a = comboMatrix[3][0] - comboMatrix[2][0];
        frustumPlanes[5].b = comboMatrix[3][1] - comboMatrix[2][1];
        frustumPlanes[5].c = comboMatrix[3][2] - comboMatrix[2][2];
        frustumPlanes[5].d = comboMatrix[3][3] - comboMatrix[2][3]; */
        // Normalize the plane equations, if requested
        //left clipping 
        frustumPlanes[0].a = comboMatrix[0][3] + comboMatrix[0][0];
        frustumPlanes[0].b = comboMatrix[1][3] + comboMatrix[1][0];
        frustumPlanes[0].c = comboMatrix[2][3] + comboMatrix[2][0];
        frustumPlanes[0].d = comboMatrix[3][3] + comboMatrix[3][0];
        // Right clipping plane
        frustumPlanes[1].a = comboMatrix[0][3] - comboMatrix[0][0];
        frustumPlanes[1].b = comboMatrix[1][3] - comboMatrix[1][0];
        frustumPlanes[1].c = comboMatrix[2][3] - comboMatrix[2][0];
        frustumPlanes[1].d = comboMatrix[3][3] - comboMatrix[3][0];
        // Top clipping plane
        frustumPlanes[2].a = comboMatrix[0][3] - comboMatrix[0][1];
        frustumPlanes[2].b = comboMatrix[1][3] - comboMatrix[1][1];
        frustumPlanes[2].c = comboMatrix[2][3] - comboMatrix[2][1];
        frustumPlanes[2].d = comboMatrix[3][3] - comboMatrix[3][1];
        // Bottom clipping plane
        frustumPlanes[3].a = comboMatrix[0][3] + comboMatrix[0][1];
        frustumPlanes[3].b = comboMatrix[1][3] + comboMatrix[1][1];
        frustumPlanes[3].c = comboMatrix[2][3] + comboMatrix[2][1];
        frustumPlanes[3].d = comboMatrix[3][3] + comboMatrix[3][1];
        // Near clipping plane
        frustumPlanes[4].a = comboMatrix[0][3] + comboMatrix[0][2];
        frustumPlanes[4].b = comboMatrix[1][3] + comboMatrix[1][2];
        frustumPlanes[4].c = comboMatrix[2][3] + comboMatrix[2][2];
        frustumPlanes[4].d = comboMatrix[3][3] + comboMatrix[3][2];
        // Far clipping plane
        frustumPlanes[5].a = comboMatrix[0][3] - comboMatrix[0][2];
        frustumPlanes[5].b = comboMatrix[1][3] - comboMatrix[1][2];
        frustumPlanes[5].c = comboMatrix[2][3] - comboMatrix[2][2];
        frustumPlanes[5].d = comboMatrix[3][3] - comboMatrix[3][2];
        if (normalize == true)
        {
            NormalizePlane(frustumPlanes[0]);
            NormalizePlane(frustumPlanes[1]);
            NormalizePlane(frustumPlanes[2]);
            NormalizePlane(frustumPlanes[3]);
            NormalizePlane(frustumPlanes[4]);
            NormalizePlane(frustumPlanes[5]);
        }
    }

    bool isOnFrustum(glm::vec3 point)
    {
        int total = 0;
        for(uint8_t i=0; i<6; i++)
        {   
            //std::cout << " For the plane number " << i << " point is on the " << ClassifyPoint(frustumPlanes[i],point) << "\n";
            if(ClassifyPoint(frustumPlanes[i],point)) 
            {
                total++;
            }
        }
        //std::cout << "total " << total << "\n";
        if(total==6) return true;
        return false;
    }



    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        velocity=0;
        a_ = 0;
        d_ = 0;
        w_ = 0;
        s_ = 0;
        pressed_space =last_pressed_space= 0;
        last_last_q =last_q=pressed_q=0;
        movement = false;
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        LastPosition = glm::vec3(0, 0, 0);
        LanternIsHeld = false;
        updateCameraVectors();
        //ExtractPlanesGL(GetViewMatrix(),false);
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        velocity = 0;
        a_ = 0;
        d_ = 0;
        s_=0;
        w_=0;
        pressed_space =last_pressed_space= 0;
        last_last_q =last_q=pressed_q=0;
        LastPosition = glm::vec3(0, 0, 0);
        movement = false;
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        LanternIsHeld = false;
        updateCameraVectors();
        //ExtractPlanesGL(GetViewMatrix(),false);
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    glm::mat4 GetProjectionMatrix(){
        return glm::perspective(glm::radians(ZOOM), 800.0f/600.0f, 0.1f, 100.0f);
    }
    
    void ProcessKeyboardDouble(Camera_Movement direction,Camera_Movement direction2, float deltaTime, CPhysics& phy,  CCollision& c, CPhysics& candil_light_physics){
        LastPosition = Position;
        movement = false;
        velocity = 0.024;
        if(colision == false)
            LastPosition = Position;
            
        if (direction == FORWARD && direction2 == LEFT){
            movement = true;
            Position += Front * velocity;
            Position -= Right * velocity;
        }
        if (direction == FORWARD && direction2 == RIGHT){
            movement = true;
            Position += Front * velocity;
            Position += Right * velocity;
        }
        if (direction == BACKWARD && direction2 == LEFT){
            movement = true;
            Position -= Front * velocity;
            Position -= Right * velocity;
        }
        if (direction == BACKWARD && direction2 == RIGHT){
            movement = true;
            Position -= Front * velocity;
            Position += Right * velocity;
        }

        if(direction == FORWARD && direction2 == FORWARD){
            if(!colision){
                movement = true; 
                velocity += 0.01;
                Position += Front * velocity;  
            }
        }
        
        Position.y = 1.5f;

        phy.x = Position.x;
        phy.y = Position.y;
        phy.z = Position.z;
        //candil_light_physics.x = Position.x;
        //candil_light_physics.y = Position.y;
        //candil_light_physics.z = Position.z;
        if(LanternIsHeld)
        {
            candil_light_physics.x = Position.x;
            candil_light_physics.y = Position.y;
            candil_light_physics.z = Position.z;
        }

    
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, CPhysics& phy,  CCollision& c, CPhysics& candil_light_physics)
    {
        movement = false;
        //float velocity = MovementSpeed * deltaTime;
        velocity = 0.037;
        //velocity *= 6;
        if(colision == false){
            LastPosition = Position;   
        }
        //std::cout << " VELOCITY MOV: " << velocity <<" \n"; 
        if (direction == FORWARD){ 
            movement = true;
            Position += Front * velocity;
        }
        if (direction == BACKWARD){ 
            movement = true;
            Position -= Front * velocity;
        }
        if (direction == LEFT){ 
            movement = true;
            Position -= Right * velocity;
        }
        if (direction == RIGHT){ 
            movement = true;
            Position += Right * velocity;
        }

        //Position = glm::vec3(phy.x, phy.y, phy.z);

        Position.y = 1.5f;
        phy.x = Position.x;
        phy.y = Position.y;
        phy.z = Position.z;
        if(LanternIsHeld)
        {
            candil_light_physics.x = Position.x;
            candil_light_physics.y = Position.y;
            candil_light_physics.z = Position.z;
        }
    //
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
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
        updateCameraVectors();
    }

    float getVelocity(){
        return velocity;
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
   /* void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    } */

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif

