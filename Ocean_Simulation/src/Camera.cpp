#include "Camera.h"



// 0.315f
Camera::Camera(): fov(60.0f), lookaround_speed(0.05f), angle(0.0f), is_first_person(true), projection_width(1500), projection_height(1000), up(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::normalize(glm::vec3(0.0f, -0.3, -0.939364))), direction(glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))), camera_position(glm::vec3(0.0f, 0.20f, 0.3f)), right(glm::normalize(glm::cross(front,up))), model(glm::mat4(1.0f)), near_plane(0.1f), far_plane(10000000.0f), z_offset(0.2f), rotation(glm::mat4(1.0f))
{
    set_projection() ;
    set_view() ;
}

void Camera::set_projection()
{
    projection = glm::perspective(glm::radians(fov), static_cast<float>(1500)/static_cast<float>(1000), near_plane, far_plane) ;
}

void Camera::set_view()
{
    view = glm::lookAt(camera_position, camera_position+front, up ) ;
}

void Camera::look_around( bool rightwards, bool leftwards, bool upwards, bool downwards )
{
    if( rightwards ){
        rotation = rotation * glm::rotate(glm::mat4(1.0f), lookaround_speed, up) ;
        lookaround_rotation = glm::rotate(glm::mat4(1.0f), lookaround_speed, up) ;
    }
    else if( leftwards ){
        rotation = rotation * glm::rotate(glm::mat4(1.0f), -lookaround_speed, up) ;
        lookaround_rotation = glm::rotate(glm::mat4(1.0f), -lookaround_speed, up) ;
    }
    else if( upwards ){
        angle += 0.005f ;
        if( angle > glm::radians(89.0f) ){
            angle = glm::radians(89.0f) ;
            return ;
        }
        else{
            rotation = rotation * glm::rotate(glm::mat4(1.0f), lookaround_speed, right) ;
            lookaround_rotation = glm::rotate(glm::mat4(1.0f), lookaround_speed, right) ;
        }
    }
    else if( downwards ){
        angle -= 0.005f ;
        if( angle < glm::radians(-89.0f) ){
            angle = glm::radians(-89.0f) ;
            return  ;
        }
        else{
            rotation = rotation * glm::rotate(glm::mat4(1.0f), -lookaround_speed, right) ;
            lookaround_rotation = glm::rotate(glm::mat4(1.0f), -lookaround_speed, right) ;
        }
    }
    front = glm::vec3(lookaround_rotation * glm::vec4(front, 1.0f)) ;
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f,1.0f,0.0f))) ;
    up = glm::normalize(glm::cross(right, front)) ;
    view = glm::lookAt(camera_position, camera_position+front, up ) ;
}

void Camera::move()
{
    camera_position += (front*0.1f) ;
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

glm::mat4 Camera::get_view()
{
    return view ;
}

glm::mat4 Camera::get_projection()
{
    return projection ;
}

Camera::~Camera(){}
