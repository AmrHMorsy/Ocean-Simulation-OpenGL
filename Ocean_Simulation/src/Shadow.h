#include "Shader.h"




class Shadow{
    
private:
    
    Shader * shader ;
    GLFWwindow * window ;
    unsigned int shader_program ;
    int depth_map_width, depth_map_height ;
    glm::mat4 light_projection, light_view ;
    GLuint depth_map_texture, depth_map_fbo ;
    
public:
    
    Shadow( GLFWwindow * window, glm::mat4 light_proj, glm::mat4 light_view_ ) ;
    void create_depth_map() ;
    void attach_depth_map() ;
    void set_uniform_variables() ;
    void render() ;
    void reset() ;
    void clear_buffer() ;
    void resize_viewport() ;
    void bind_screen_framebuffer() ;
    void activate_texture() ;
    unsigned int get_shader_program() ;
    void generate_error( std::string error_msg ) ;
    ~Shadow() ;
} ;
