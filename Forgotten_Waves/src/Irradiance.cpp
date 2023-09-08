#include "Irradiance.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/include/stb_image_write.h"


Irradiance::Irradiance( progressbar * bar ): num_progress_bars(35)
{
    this->bar = bar ;
    irradiance = new Shader( "../external/Shaders/Irradiance/Irradiance_vs.vs", "../external/Shaders/Irradiance/Irradiance_fs.fs" ) ;
    update_progress_bar(5) ;
    prefilter = new Shader( "../external/Shaders/Irradiance/PreFiltrdEnv_vs.vs", "../external/Shaders/Irradiance/PreFiltrdEnv_fs.fs" ) ;
    update_progress_bar(5) ;
    brdfShader = new Shader( "../external/Shaders/Irradiance/BRDF_IntegrationMap_vs.vs", "../external/Shaders/Irradiance/BRDF_IntegrationMap_fs.fs" ) ;
    irradiance_shader = irradiance->get_shader_program() ;
    prefilter_shader = prefilter->get_shader_program() ;
    brdf_shader = brdfShader->get_shader_program() ;
}

void Irradiance::set(unsigned int vao, unsigned int cubemap, unsigned int FBO, unsigned int RBO )
{
    this->vao = vao ;
    this->cubemap = cubemap ;
    this->FBO = FBO ;
    this->RBO = RBO ;
}

void Irradiance::create_irradiance_map()
{
    glGenTextures(1, &irradiance_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0,
                     GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    update_progress_bar(5) ;
    
    fill_irradiance_map() ;
}

void Irradiance::fill_irradiance_map()
{
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 views[6] = {
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f))} ;
    glUseProgram(irradiance_shader) ;
    glUniform1i(glGetUniformLocation(irradiance_shader, "irradiance_map"), 0) ;
    glUniformMatrix4fv(glGetUniformLocation(irradiance_shader, "projection"), 1, GL_FALSE, &projection[0][0]) ;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glViewport(0, 0, 512, 512) ;
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    update_progress_bar(5) ;
    for ( unsigned int i = 0; i < 6; i++ ){
        glUniformMatrix4fv(glGetUniformLocation(irradiance_shader, "view"), 1, GL_FALSE, &views[i][0][0]) ;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shade();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    update_progress_bar(5) ;

    glFinish();
    create_prefiltered_environment_map() ;
}

void Irradiance::create_prefiltered_environment_map()
{
    glGenTextures(1, &prefiltered_env_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefiltered_env_map);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    fill_prefiltered_environment_map() ;
    update_progress_bar(5) ;
}

void Irradiance::fill_prefiltered_environment_map()
{
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 views[6] = {
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f))} ;
    glUseProgram(prefilter_shader) ;
    glUniform1i(glGetUniformLocation(irradiance_shader, "environmentMap"), 0) ;
    glUniformMatrix4fv(glGetUniformLocation(prefilter_shader, "projection"), 1, GL_FALSE, &projection[0][0]) ;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO) ;
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip){
        unsigned int mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);
        float roughness = (float)mip / (float)(maxMipLevels - 1);
        glUniform1f(glGetUniformLocation(prefilter_shader, "roughness"), roughness) ;
        for (unsigned int i = 0; i < 6; ++i){
            glUniformMatrix4fv(glGetUniformLocation(prefilter_shader, "view"), 1, GL_FALSE, &views[i][0][0]) ;
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefiltered_env_map, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shade();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    update_progress_bar(5) ;

    generate_brdf_LUT() ;
}

void Irradiance::generate_brdf_LUT()
{
    glGenTextures(1, &brdfLUTTexture);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
    glViewport(0, 0, 512, 512);
    glUseProgram(brdf_shader) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Irradiance::renderQuad()
{
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Irradiance::shade()
{
    glBindVertexArray(vao) ;
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0) ;
    glDepthFunc(GL_LESS);
}

void Irradiance::update_progress_bar( int num_bars )
{
    if( num_bars > num_progress_bars )
        return ;
    for( int i = 0 ; i < num_bars  ; i++ )
        bar->update() ;
    num_progress_bars -= num_bars ;
}

void Irradiance::activate_irradiance()
{
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefiltered_env_map);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

Irradiance::~Irradiance()
{
    delete prefilter ;
    prefilter = NULL ;
    delete irradiance ;
    irradiance = NULL ; 
}
