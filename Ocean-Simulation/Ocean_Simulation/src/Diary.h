#include "Skybox.h"



class Diary{
    
private:
    
    Skybox * skybox ;
    Parser * parser ;
    Shader * shader ;
    progressbar * bar ;
    std::vector<glm::vec3> lights ;
    glm::mat4 model, projection, view ;
    std::vector<unsigned int> textures ;
    std::vector<std::string> file_paths ;
    unsigned int vao, vbo, ebo, shader_program, number_of_indices ;
    glm::vec3 light_radiance, camera, light_color, translation ;
    
public:
    
    Diary( glm::mat4 v, glm::mat4 proj, glm::vec3 c, progressbar * b ) ;
    void set( Skybox * skybox ) ; 
    void generate_buffers() ;
    void load_textures() ;
    void bind_vertices( std::vector<float> vertices ) ;
    void bind_indices( std::vector<unsigned int> indices ) ;
    void set_uniform_variables() ;
    void shade() ;
    void activate_textures() ;
    void move( glm::mat4 boat_translation ) ;
    void update_view( glm::mat4 v, glm::vec3 camera, glm::vec3 front ) ;
    void generate_error( std::string msg ) ;
    ~Diary() ;
} ;

