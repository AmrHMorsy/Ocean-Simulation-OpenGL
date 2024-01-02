#include "Boat.h"
#include "../external/Include/stb_image.h"
#define EPSILON 1.8

//{0.484394f, 7.5f, 0.416144f}

Boat::Boat( glm::mat4 v, glm::mat4 proj, glm::vec3 c, glm::vec3 front, float fv, progressbar * b, glm::mat4 light_proj, glm::mat4 _light_view): num_progress_bars(10),light_position({0.0f, 10.0f, 0.0f}), light_color({100.0f, 100.0f, 100.0f}), view(v), projection(proj), camera(c), file_paths( {"../assets/Boat/Albedo.png", "../assets/Boat/AO.png", "../assets/Boat/Normal.png", "../assets/Boat/Roughness.png", "../assets/Boat/Displacement.png", "../assets/Boat/Metallic.png"} ), camera_front(front), fov(fv), number_sand_patches(0), bar(b), time(0.0f), light_projection(light_proj), light_view(_light_view)
{
    parser = new Parser(bar) ;
    parser->parse( "../assets/Boat/Boat2.obj" ) ;
    shader = new Shader("../external/Shaders/Boat/Boat_vs.vs", "../external/Shaders/Boat/Boat_fs.fs") ;
    generate_buffers() ;
    load_textures() ;
    shader_program = shader->get_shader_program() ;
    glUseProgram(shader_program) ;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f) ) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f)) ;
    bind_vertices( parser->get_vertices() ) ;
    bind_indices( parser->get_vertices_faces() ) ;
    set_uniform_variables() ;
}

void Boat::set( Skybox * skybox )
{
    this->skybox = skybox ;
}

void Boat::generate_buffers()
{
    textures.resize(file_paths.size()) ;
    glGenTextures(file_paths.size(),textures.data()) ;
    glGenVertexArrays(1, &vao) ;
    glGenBuffers(1, &vbo) ;
    glGenBuffers(1, &ebo) ;
    glGenBuffers(1, &texture_indices_vbo);
    update_progress_bar(10) ;
}

void Boat::shade( unsigned int shadow_shader_program )
{
    glBindVertexArray(vao) ;
    glUniformMatrix4fv(glGetUniformLocation(shadow_shader_program, "model"), 1, GL_FALSE, &model[0][0]);
    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0 ) ;
    glBindVertexArray(0) ;
}

void Boat::shade( Shadow * shadow )
{
    glUseProgram(shader_program) ;
    update_time() ;
    rock_boat() ;
    activate_textures() ;
    shadow->activate_texture() ;
    glBindVertexArray(vao) ;
    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0) ;
    glBindVertexArray(0) ;
}

//void Boat::shade()
//{
//    glUseProgram(shader_program) ;
//    update_time() ;
//    activate_textures() ;
//    rock_boat() ;
//    glBindVertexArray(vao) ;
//    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0) ;
//    glBindVertexArray(0) ;
//}

void Boat::rock_boat()
{
    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.00f, 1.0f) ) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(4.0f * sin(time)), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f)) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
}

void Boat::bind_vertices( std::vector<float> vertices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ARRAY_BUFFER, vbo) ;
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(),GL_STATIC_DRAW) ;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)0) ;
    glEnableVertexAttribArray(0) ;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0) ;
    update_progress_bar(10) ;
}

void Boat::update_time()
{
    float deltaTime = glfwGetTime() - time ;
    time += deltaTime ;
}

void Boat::bind_indices( std::vector<unsigned int> indices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo) ;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_DYNAMIC_DRAW) ;
    glBindVertexArray(0) ;
    number_of_indices = indices.size() ;
    update_progress_bar(8) ;
}

void Boat::set_uniform_variables()
{
    glUniform1i(glGetUniformLocation(shader_program, "albedo_map"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "ao_map"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "roughness_map"), 3);
    glUniform1i(glGetUniformLocation(shader_program, "displacement_map"), 4);
    glUniform1i(glGetUniformLocation(shader_program, "metallic_map"), 8) ;
    glUniform1i(glGetUniformLocation(shader_program, "opacity_map"), 9) ;
    glUniform1i(glGetUniformLocation(shader_program, "irradiance_map"), 5) ;
    glUniform1i(glGetUniformLocation(shader_program, "prefilter_map"), 6) ;
    glUniform1i(glGetUniformLocation(shader_program, "brdf_integration_map"), 7) ;
    glUniform1i(glGetUniformLocation(shader_program, "shadow_map"), 10) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE,&projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_color"), 1, &light_color[0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 1, &light_position[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_view"), 1, GL_FALSE,&light_view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_projection"), 1, GL_FALSE, &light_projection[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
    update_progress_bar(6) ;
}

void Boat::update_view( glm::vec3 camera, glm::mat4 v, glm::vec3 front )
{
    this->camera = camera ;
    camera_front = front ;
    glUseProgram(shader_program) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &v[0][0]) ;
}

void Boat::load_textures()
{
    GLenum format ;
    int w, h, nc ;
    unsigned char * data ;
    for( int i = 0 ; i < file_paths.size() ; i++ ){
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
        stbi_set_flip_vertically_on_load(true) ;
        data = stbi_load(file_paths[i].c_str(), &w, &h, &nc, 0);
        format = (nc==1)? GL_RED : (nc==3)? GL_RGB : (nc==4) ? GL_RGBA : GL_RGB ;
        if (data){
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data) ;
            glGenerateMipmap(GL_TEXTURE_2D) ;
        }
        else{
            std::cout << file_paths[i] << std::endl;
            generate_error("\nFailure To Load Texture") ;
        }
        stbi_image_free(data);
    }
    update_progress_bar(6) ;
}

void Boat::update_progress_bar( int num_bars )
{
    if( num_bars > num_progress_bars )
        return ;
    for( int i = 0 ; i < num_bars  ; i++ )
        bar->update() ;
    num_progress_bars -= num_bars ;
}

void Boat::activate_textures()
{
    for( int i = 0 ; i < textures.size()-2 ; i++ ){
        glActiveTexture(GL_TEXTURE0+i) ;
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
    }
    skybox->activate_irradiance() ;
    glActiveTexture(GL_TEXTURE8) ;
    glBindTexture(GL_TEXTURE_2D, textures[textures.size()-2]) ;
    glActiveTexture(GL_TEXTURE9) ;
    glBindTexture(GL_TEXTURE_2D, textures[textures.size()-1]) ;
}

std::vector<float> Boat::get_mesh()
{
    return parser->get_vertices() ;
}

void Boat::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Boat::~Boat()
{
    delete shader ;
    shader = NULL ;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(textures.size(), textures.data());
}
