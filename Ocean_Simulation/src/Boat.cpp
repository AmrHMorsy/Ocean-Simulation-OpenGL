#include "Boat.h"
#include "../external/Include/stb_image.h"



Boat::Boat( glm::mat4 v, glm::mat4 proj, glm::vec3 c, Diary * d, progressbar * b ): view(v), projection(proj), camera(c), light_radiance({0.5f, 0.5f, 0.5f}), translation({0.0f, 0.0f, 0.0f}), file_paths({"../assets/Boat/Albedo.jpg", "../assets/Boat/AO.jpg", "../assets/Boat/Normal.jpg", "../assets/Boat/Roughness.jpg", "../assets/Boat/Metallic.jpg"}), max_pitch(2.0f), max_roll(2.0f), time(0.0f), diary(d), lights({{0.484394f, 7.5f, 0.416144f},{-1.0f, 1.0f, -1.0f}}), model(glm::translate(glm::mat4(1.0), translation) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f))), bar(b)
{
    parser = new Parser(bar) ;
    parser->parse("../assets/Boat/Boat.obj" ) ;
    shader = new Shader( "../external/Shaders/Boat/Boat_vs.vs", "../external/Shaders/Boat/Boat_fs.fs" ) ;
    generate_buffers() ;
    load_textures() ;
    shader_program = shader->get_shader_program() ;
    glUseProgram(shader_program) ;
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
        else
            generate_error("Failure To Load Texture") ;
        stbi_image_free(data);
    }
}

void Boat::bind_vertices( std::vector<float> vertices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ARRAY_BUFFER, vbo) ;
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW) ;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)0) ;
    glEnableVertexAttribArray(0) ;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0) ;
}

void Boat::bind_indices( std::vector<unsigned int> indices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo) ;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_DYNAMIC_DRAW) ;
    glBindVertexArray(0) ;
    number_of_indices = indices.size() ;
}

void Boat::set_uniform_variables()
{
    glUniform1i(glGetUniformLocation(shader_program, "albedo_map"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "AO_map"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "roughness_map"), 3);
    glUniform1i(glGetUniformLocation(shader_program, "displacement_map"), 4);
    glUniform1i(glGetUniformLocation(shader_program, "irradiance_map"), 5) ;
    glUniform1i(glGetUniformLocation(shader_program, "prefilter_map"), 6) ;
    glUniform1i(glGetUniformLocation(shader_program, "brdf_integration_map"), 7) ;
    glUniform1i(glGetUniformLocation(shader_program, "height_map"), 8) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_radiance"), 1, &light_radiance[0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_color"), 1, &light_color[0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "lights"), lights.size(), glm::value_ptr(lights[0])) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &view[0][0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
}

void Boat::rock_boat()
{
    time += (glfwGetTime() - time) ;
    float pitchAngle = sin(time) * max_pitch;
    float rollAngle = sin(time + 1.0f) * max_roll ;
    glm::mat4 pitchMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(pitchAngle), glm::vec3(1, 0, 0)) ;
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rollAngle), glm::vec3(0, 0, 1));
    model = glm::translate(glm::mat4(1.0), translation) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * pitchMatrix * rollMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)) ;
    diary->move(model/glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f))) ;
}

void Boat::shade()
{
    glUseProgram(shader_program) ;
    rock_boat() ;
    activate_textures() ;
    glBindVertexArray(vao) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0 ) ;
    glBindVertexArray(0) ;
}

void Boat::activate_textures()
{
    for( int i = 0 ; i < textures.size() ; i++ ){
        glActiveTexture(GL_TEXTURE0+i) ;
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
    }
    skybox->activate_irradiance() ;
}

void Boat::update_view( glm::mat4 v, glm::vec3 camera )
{
    glUseProgram(shader_program) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &v[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
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
    delete parser ;
    parser = NULL ; 
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
