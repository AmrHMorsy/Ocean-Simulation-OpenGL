#include "Camera.h"




class Renderer{
    
private:
    
    Shadow * shadow ;
    int fps ;
    Ocean * ocean ;
    Light * light ; 
    Skybox * skybox ;
    Camera * camera ;
    Boat * boat ; 
    GLFWwindow * window ;
    float fps_time, time ;

public:
    
    Renderer( GLFWwindow * window_, progressbar * bar) ;
    void set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void play_music() ;
    void render_shadow() ;
    void render_scene() ;
    void start_game_loop() ;
    void handle_events() ;
    void update_view() ;
    void calculate_fps() ;
    void generate_error( std::string error_msg ) ;
    ~Renderer() ;

} ;
