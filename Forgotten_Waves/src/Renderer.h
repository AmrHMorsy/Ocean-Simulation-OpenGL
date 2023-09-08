#include "Camera.h"



class Renderer{
    
private:
    
    Diary * diary ;
    int frames ;
    Ocean * ocean ;
    Skybox* skybox ;
    Camera * camera ;
    Boat * boat ; 
    GLFWwindow * window ;
    float FPS_time, time ;

public:
    
    Renderer( progressbar * bar) ;
    void set( GLFWwindow * window, cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void start_game_loop() ;
    void play_music() ;
    void handle_events() ;
    void update_view() ;
    void calculate_FPS() ;
    void generate_error(std::string msg) ; 
    ~Renderer() ;
    
} ;
