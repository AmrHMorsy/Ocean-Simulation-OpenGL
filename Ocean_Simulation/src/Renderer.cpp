#include "Renderer.h"
#define PROGRESS_BARS_STEP 10



Renderer::Renderer( GLFWwindow * window_, progressbar * bar): fps(0), fps_time(0), window(window_)
{
    camera = new Camera() ;
    ocean = new Ocean( camera->get_view(), camera->get_projection(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar) ;
    skybox = new Skybox( camera->get_view(), camera->get_projection(), camera->get_camera_position(), bar) ;
}

void Renderer::set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    ocean->set(context, FA, IFT, queue, skybox) ;
}

void Renderer::play_music()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        generate_error("Failed to open audio") ;
    Mix_Music * music = Mix_LoadMUS("../assets/music/waves.mp3");
    if (!music)
        generate_error("Failed to load music") ;
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8) ;
    Mix_PlayMusic(music, -1) ;
    std::cout << std::endl ;
}

void Renderer::start_game_loop()
{
    time = glfwGetTime() ;
    play_music() ;
    update_view() ;
    while( !glfwWindowShouldClose(window) ){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
        glClearColor(0.0f, 0.0f, 0.0f,1.0f) ;
        skybox->activate() ;
        skybox->render() ;
        ocean->render() ;
        glfwSwapBuffers(window) ;
        handle_events() ;
        calculate_fps() ;
    }
}

void Renderer::handle_events()
{
    glfwPollEvents() ;
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->move() ;
        update_view() ;
    }
}

void Renderer::update_view()
{
    ocean->update_view( camera->get_camera_position(), camera->get_view(), camera->get_camera_front()) ;
    skybox->update_view( camera->get_view(), camera->get_camera_position() ) ;
}

void Renderer::calculate_fps()
{
    fps++ ;
    if( glfwGetTime() - static_cast<float>(fps_time) >= 1.0 ){
        std::cout << "FPS: \033[1;31m" << fps << "\033[0m" << '\r' << std::flush ;
        fps = 0 ;
        fps_time += 1.0 ;
    }
}

void Renderer::generate_error( std::string error_msg )
{
    std::cout << "\033[1;31m" << error_msg << "\033[0m" << std::endl;
    exit(1) ;
}

Renderer::~Renderer()
{
    delete camera ;
    camera = NULL ;
    delete ocean ;
    ocean = NULL ;
    delete skybox ;
    skybox = NULL ;
}
