#include "Shader.h"



Shader::Shader( std::string vs_filepath, std::string fs_filepath, std::string tcs_filepath, std::string tes_filepath)
{
    vertex_shader_filepath = vs_filepath ;
    fragment_shader_filepath = fs_filepath ;
    tessellation_control_shader_filepath = tcs_filepath ;
    tessellation_evaluation_shader_filepath = tes_filepath ;
    create_vertex_shader() ;
    create_fragment_shader() ;
    create_tessellation_control_shader() ;
    create_tessellation_evaluation_shader() ;
    link_tshaders() ;
}

Shader::Shader( std::string vs_filepath, std::string fs_filepath )
{
    vertex_shader_filepath = vs_filepath ;
    fragment_shader_filepath = fs_filepath ;
    create_vertex_shader() ;
    create_fragment_shader() ;
    link_shaders() ;
}

void Shader::create_vertex_shader()
{
    int success ;
    std::ifstream vs ;
    std::stringstream VS ;
    vs.open(vertex_shader_filepath, std::fstream::in) ;
    VS << vs.rdbuf();
    vs.close() ;
    std::string st = VS.str() ;
    const char * vertex_shader_source = st.c_str() ;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER) ;
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL) ;
    glCompileShader(vertex_shader) ;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if( !success ){
        char infoLog[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        generate_error(std::string("ERROR::VERTEX SHADER COMPILATION FAILED\n") + infoLog) ;
    }
}

void Shader::create_fragment_shader()
{
    int success ;
    std::ifstream fs ;
    std::stringstream FS ;
    fs.open(fragment_shader_filepath, std::fstream::in) ;
    FS << fs.rdbuf();
    fs.close() ;
    std::string st = FS.str() ;
    const char * fragment_shader_source = st.c_str() ;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER) ;
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if( !success ){
        char infoLog[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        generate_error(std::string("ERROR::FRAGMENT SHADER COMPILATION FAILED\n") + infoLog) ;
    }
}

void Shader::create_tessellation_control_shader()
{
    int success;
    std::ifstream tcs;
    std::stringstream TCS;
    tcs.open(tessellation_control_shader_filepath, std::fstream::in);
    TCS << tcs.rdbuf();
    tcs.close();
    std::string st = TCS.str();
    const char * tessellation_control_shader_source = st.c_str();
    tessellation_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessellation_control_shader, 1, &tessellation_control_shader_source, NULL);
    glCompileShader(tessellation_control_shader);
    glGetShaderiv(tessellation_control_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(tessellation_control_shader, 512, NULL, infoLog);
        generate_error(std::string("ERROR::TESSELLATION CONTROL SHADER COMPILATION FAILED\n") + infoLog);
    }
}

void Shader::create_tessellation_evaluation_shader()
{
    int success;
    std::ifstream tes;
    std::stringstream TES;
    tes.open(tessellation_evaluation_shader_filepath, std::fstream::in);
    TES << tes.rdbuf();
    tes.close();
    std::string st = TES.str();
    const char * tessellation_evaluation_shader_source = st.c_str();
    tessellation_evaluation_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessellation_evaluation_shader, 1, &tessellation_evaluation_shader_source, NULL);
    glCompileShader(tessellation_evaluation_shader);
    glGetShaderiv(tessellation_evaluation_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(tessellation_evaluation_shader, 512, NULL, infoLog);
        generate_error(std::string("ERROR::TESSELLATION EVALUATION SHADER COMPILATION FAILED\n") + infoLog);
    }
}

void Shader::link_shaders()
{
    int success ;
    shader_program = glCreateProgram() ;
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        generate_error(std::string("ERROR::LINKING SHADERS FAILED\n") + infoLog) ;
    }
}

void Shader::link_tshaders()
{
    int success;
    shader_program = glCreateProgram() ;
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, tessellation_control_shader) ;
    glAttachShader(shader_program, tessellation_evaluation_shader) ;
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        generate_error(std::string("ERROR::LINKING SHADERS FAILED\n") + infoLog);
    }
}

unsigned int Shader::get_shader_program()
{
    return shader_program ;
}

void Shader::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Shader::~Shader()
{
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}
