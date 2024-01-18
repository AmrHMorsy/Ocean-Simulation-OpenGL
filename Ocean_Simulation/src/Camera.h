#include "Ocean.h"




class Camera{
    
private:
    
    bool is_first_person ;
    glm::vec3 camera_position, front, up, right, direction ;
    glm::mat4 projection, view, model, rotation, lookaround_rotation ;
    float angle, fov, lookaround_speed, hfov, near_plane, far_plane, projection_width, projection_height, z_offset ;
    
public:
    
    Camera() ;
    void set_projection() ;
    void set_view() ;
    void look_around( bool rightwards, bool leftwards, bool upwards, bool downwards ) ;
    void move() ;
    void turn_right( glm::vec3 boat_direction ) ;
    void turn_left( glm::vec3 boat_direction ) ;
    float get_fov() ;
    void switch_view() ; 
    glm::mat4 get_view() ;
    glm::mat4 get_projection() ;
    glm::vec3 get_camera_position() ;
    glm::vec3 get_camera_front() ;
    ~Camera() ;
    
} ;
