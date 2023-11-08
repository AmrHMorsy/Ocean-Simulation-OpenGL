#include "Parser.h"




class Shader{
    
private:
    
    std::string vertex_shader_filepath, fragment_shader_filepath, tessellation_control_shader_filepath, tessellation_evaluation_shader_filepath ;
    unsigned int vertex_shader, fragment_shader, tessellation_control_shader, tessellation_evaluation_shader, shader_program ;

public:
    
    Shader( std::string vs_filepath, std::string fs_filepath ) ;
    Shader( std::string vs_filepath, std::string fs_filepath, std::string tcs_filepath, std::string tes_filepath) ;
    void create_vertex_shader() ;
    void create_fragment_shader() ;
    void create_tessellation_control_shader() ;
    void create_tessellation_evaluation_shader() ;
    void link_shaders() ;
    void link_tshaders() ;
    unsigned int get_shader_program() ;
    void generate_error( std::string msg ) ;
    ~Shader() ;
    
} ;

