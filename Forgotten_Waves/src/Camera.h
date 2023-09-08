#include "Ocean.h"



class Camera{
    
private:
    
    glm::mat4 projection, view, model ;
    float angle, fov, lookaround_speed, hfov ;
    glm::vec3 camera_position, front, up, right ;
    
public:
    
    Camera() ;
    void submerge( float offset ) ;
    void look_around( bool rightwards, bool leftwards, bool upwards, bool downwards ) ;
    void move( glm::vec3 offset ) ;
    float get_fov() ;
    glm::mat4 get_view_matrix() ;
    glm::mat4 get_model_matrix() ;
    glm::mat4 get_projection_matrix() ;
    glm::vec3 get_camera_position() ;
    glm::vec3 get_camera_front() ;
    ~Camera() ;
    
} ;
