#include "Shadow.h"



Shadow::Shadow( GLFWwindow * window_, glm::mat4 light_proj, glm::mat4 light_view_ ): light_projection(light_proj), light_view(light_view_), window(window_)
{
    shader = new Shader( "../external/Shaders/Shadow/Shadow_vs.vs", "../external/Shaders/Shadow/Shadow_fs.fs" ) ;
    shader_program = shader->get_shader_program() ;
    glfwGetFramebufferSize(window, &depth_map_width, &depth_map_height);
    create_depth_map() ;
}

void Shadow::create_depth_map()
{
    glGenTextures(1, &depth_map_texture) ;
    glBindTexture(GL_TEXTURE_2D, depth_map_texture) ;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depth_map_width, depth_map_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    attach_depth_map() ;
}

void Shadow::attach_depth_map()
{
    glGenFramebuffers(1, &depth_map_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0) ;
    glDrawBuffer(GL_NONE) ;
    
    if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        generate_error("Shadow framebuffer is not complete") ;
    
    set_uniform_variables() ;
}

void Shadow::set_uniform_variables()
{
    glUseProgram(shader_program) ;
    GLint light_proj_location = glGetUniformLocation(shader_program, "light_projection") ;
    if( light_proj_location == -1 )
        generate_error( "ERROR: Unifom variable 'light_projection' not found in the shader program\n" ) ;
    glUniformMatrix4fv(light_proj_location, 1, GL_FALSE, &light_projection[0][0]) ;
    GLint light_view_location = glGetUniformLocation(shader_program, "light_view") ;
    if( light_view_location == -1 )
        generate_error( "ERROR: Unifom variable 'light_view' not found in the shader program\n" ) ;
    glUniformMatrix4fv(light_view_location, 1, GL_FALSE, &light_view[0][0]) ;
}

void Shadow::render()
{
    glUseProgram(shader_program);
    glViewport(0, 0, depth_map_width, depth_map_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::reset()
{
    bind_screen_framebuffer() ;
    clear_buffer() ;
    resize_viewport() ;
}

void Shadow::clear_buffer()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glClearColor(0.0f, 0.0f, 0.0f,1.0f) ;
}

void Shadow::resize_viewport()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Shadow::bind_screen_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::activate_texture()
{
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
}

unsigned int Shadow::get_shader_program()
{
    return shader_program ;
}

void Shadow::generate_error( std::string error_msg )
{
    std::cout << "\033[1;31m" << error_msg << "\033[0m" << std::endl;
    exit(1) ;
}

Shadow::~Shadow()
{
    glDeleteFramebuffers(1, &depth_map_fbo) ;
    delete shader ;
    shader = NULL ;
}
