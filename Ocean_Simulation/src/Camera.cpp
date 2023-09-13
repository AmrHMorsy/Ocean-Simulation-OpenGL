#include "Camera.h"



Camera::Camera(): fov(60.0f), lookaround_speed(0.1f), angle(0.0f)
{
    up = glm::vec3(0.0f, 1.0f, 0.0f) ;
    front = glm::normalize(glm::vec3(0.0f, -0.3, -0.939364)) ;
    camera_position = glm::vec3(0.0f, 0.30f, 0.0f) ;
    right = glm::normalize(glm::cross(front,up)) ;
    model = glm::mat4(1.0f) ;
    projection = glm::perspective(glm::radians(fov), static_cast<float>(WINDOW_WIDTH)/static_cast<float>(WINDOW_HEIGHT), 0.1f, 10000000.0f) ;
    view = glm::lookAt(camera_position, camera_position+front, up ) ;
}

void Camera::submerge( float offset )
{
    const float lowerLimit = -0.5f;
    const float upperLimit = 100.0f ;
    float proposedPosition = camera_position.y + (offset / 4);
    if (proposedPosition <= lowerLimit && offset < 0)
        camera_position.y = lowerLimit;
    else if (proposedPosition >= upperLimit && offset > 0)
        camera_position.y = upperLimit;
    else
        camera_position.y = proposedPosition ;
    if( camera_position.y == -0.05 )
        camera_position.y = 0 ;
    view = glm::lookAt(camera_position, camera_position + front, up);
}

void Camera::look_around( bool rightwards, bool leftwards, bool upwards, bool downwards )
{
    glm::mat4 rotation ;
    if( rightwards )
        rotation = glm::rotate(glm::mat4(1.0f), lookaround_speed, up) ;
    else if( leftwards )
        rotation = glm::rotate(glm::mat4(1.0f), -lookaround_speed, up) ;
    else if( upwards ){
        angle += 0.005f ;
        if( angle > glm::radians(89.0f) ){
            angle = glm::radians(89.0f) ;
            return ;
        }
        else
            rotation = glm::rotate(glm::mat4(1.0f), lookaround_speed, right) ;
    }
    else if( downwards ){
        angle -= 0.005f ;
        if( angle < glm::radians(-89.0f) ){
            angle = glm::radians(-89.0f) ;
            return  ;
        }
        else
            rotation = glm::rotate(glm::mat4(1.0f), -lookaround_speed, right) ;
    }
    front = glm::vec3(rotation * glm::vec4(front, 1.0f));
    right = glm::normalize(glm::cross(front, glm::vec3(0,1,0 )));
    up = glm::normalize(glm::cross(right, front));
    view = glm::lookAt(camera_position, camera_position+front, up ) ;
}

void Camera::move( glm::vec3 offset )
{
    camera_position = camera_position + offset ;
    view = glm::lookAt(camera_position, camera_position+front, up ) ;
}

float Camera::get_fov()
{
    return glm::radians(fov) ;
}

glm::vec3 Camera::get_camera_front()
{
    return glm::normalize(front) ; 
}

glm::vec3 Camera::get_camera_position()
{
    return camera_position ;
}

glm::mat4 Camera::get_view_matrix()
{
    return view ;
}

glm::mat4 Camera::get_model_matrix()
{
    return model ;
}

glm::mat4 Camera::get_projection_matrix()
{
    return projection ;
}

Camera::~Camera(){}
