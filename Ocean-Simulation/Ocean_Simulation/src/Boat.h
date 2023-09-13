#include "Diary.h"



class Boat{
    
private:
    
    Diary * diary ;
    Skybox * skybox ;
    Parser * parser ;
    Shader * shader ;
    progressbar * bar ;
    std::vector<glm::vec3> lights ;
    float time, max_pitch, max_roll ;
    glm::mat4 model, projection, view ;
    std::vector<unsigned int> textures ;
    std::vector<std::string> file_paths ;
    unsigned int vao, vbo, ebo, shader_program, number_of_indices ;
    glm::vec3 light_radiance, camera, light_color, translation, translation2, translation3 ;
    
public:
    
    Boat( glm::mat4 v, glm::mat4 proj, glm::vec3 c, Diary * d, progressbar * b ) ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void load_textures() ;
    void bind_vertices( std::vector<float> vertices ) ;
    void bind_indices( std::vector<unsigned int> indices ) ;
    void set_uniform_variables() ;
    void rock_boat() ;
    void shade() ;
    void update_view( glm::mat4 v, glm::vec3 camera ) ;
    void activate_textures() ;
    void generate_error( std::string msg ) ;
    ~Boat() ;
} ;
