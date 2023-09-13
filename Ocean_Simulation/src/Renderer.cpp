#include "Renderer.h"
#define PROGRESS_BARS_STEP 10



Renderer::Renderer( progressbar * bar): frames(0), FPS_time(0)
{
    camera = new Camera() ;
    ocean = new Ocean( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar) ;
    diary = new Diary(camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), bar) ;
    boat = new Boat(camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), diary,  bar) ;
    skybox = new Skybox( camera->get_view_matrix(), camera->get_projection_matrix(), bar) ;
}

void Renderer::set( GLFWwindow * window, cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    this->window = window ;
    ocean->set(context, FA, IFT, queue, skybox) ;
    boat->set(skybox) ;
    diary->set(skybox) ;
}

void Renderer::start_game_loop()
{
    time = glfwGetTime() ;
    play_music() ;
    update_view() ;
    while( !glfwWindowShouldClose(window) ){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
        glClearColor(0.0f, 0.0f, 0.0f,1.0f) ;
        skybox->shade() ;
        ocean->shade() ;
        boat->shade() ;
        diary->shade() ;
        glfwSwapBuffers(window);
        glfwPollEvents() ;
        handle_events() ;
        calculate_FPS() ;
    }
}

void Renderer::play_music()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        generate_error("Failed to open audio") ;
    Mix_Music * music = Mix_LoadMUS("../assets/music/waves.mp3");
    if (!music)
        generate_error("Failed to load music") ;
    Mix_VolumeMusic(MIX_MAX_VOLUME / 6) ;
    Mix_PlayMusic(music, -1) ;
    Mix_Chunk* soundEffect = Mix_LoadWAV("../assets/music/Whispers.mp3");
    if (!soundEffect)
        generate_error("Failed to load sound effect") ;
    int channel = Mix_PlayChannel(-1, soundEffect, -1) ;
    if (channel == -1)
        generate_error("Failed to play sound effect") ;
    Mix_Volume(channel, MIX_MAX_VOLUME / 2) ;
    std::cout << std::endl << std::endl ;
}

void Renderer::handle_events()
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        camera->look_around(false,false,true,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        camera->look_around(false,false,false,true) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        camera->look_around(true,false,false,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        camera->look_around(false,true,false,false) ;
        update_view() ;
    }
    skybox->update_view( camera->get_view_matrix(), camera->get_camera_position() ) ;
}

void Renderer::update_view()
{
    boat->update_view( camera->get_view_matrix(), camera->get_camera_position()) ;
    ocean->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    diary->update_view(camera->get_view_matrix(), camera->get_camera_position(), camera->get_camera_front() ) ;
    skybox->update_view( camera->get_view_matrix(), camera->get_camera_position() ) ;
}

void Renderer::calculate_FPS()
{
    frames++ ;
    if( glfwGetTime() - static_cast<float>(FPS_time) >= 1.0 ){
        std::cout << "FPS: \033[1;31m" << frames << "\033[0m" << '\r' << std::flush ;
        frames = 0 ;
        FPS_time += 1.0 ;
    }
}

void Renderer::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Renderer::~Renderer()
{
    delete diary ;
    diary = NULL ;
    delete camera ;
    camera = NULL ;
    delete ocean ;
    ocean = NULL ;
    delete skybox ;
    skybox = NULL ;
}
