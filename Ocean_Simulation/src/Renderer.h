#include "Camera.h"




class Renderer{
    
private:
    
    int fps ;
    Ocean * ocean ;
    Skybox * skybox ;
    Camera * camera ;
    GLFWwindow * window ;
    float fps_time, time ;

public:
    
    Renderer( GLFWwindow * window_, progressbar * bar) ;
    void set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void play_music() ;
    void start_game_loop() ;
    void handle_events() ;
    void update_view() ;
    void calculate_fps() ;
    void generate_error( std::string error_msg ) ;
    ~Renderer() ;

} ;
