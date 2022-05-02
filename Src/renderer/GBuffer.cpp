#include "GBuffer.h"

GBuffer::GBuffer()
{
    this->window_width = 1024;
    this->window_height = 768;
    this->uniform_helper = UniformHelper();
}

GBuffer::GBuffer(GLint window_width, GLint window_height)
{

    this->window_width = window_width;
    this->window_height = window_height;
    this->uniform_helper = UniformHelper();
}

void GBuffer::create()
{

    glGenFramebuffers(1, &g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

    glGenTextures(1, &g_position);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, g_buffer_attachment[0], GL_TEXTURE_2D, g_position, 0);

    glGenTextures(1, &g_normal);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, g_buffer_attachment[1], GL_TEXTURE_2D, g_normal, 0);

    glGenTextures(1, &g_albedo);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, g_buffer_attachment[2], GL_TEXTURE_2D, g_albedo, 0);

    glGenTextures(1, &g_material_id);
    glBindTexture(GL_TEXTURE_2D, g_material_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, g_buffer_attachment[3], GL_TEXTURE_2D, g_material_id, 0);

    glDrawBuffers(G_BUFFER_SIZE , g_buffer_attachment);

    // create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &g_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, g_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width, window_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_depth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::read( GLuint g_buffer_lighting_uniform_position_location,
                    GLuint g_buffer_lighting_uniform_normal_location,
                    GLuint g_buffer_lighting_uniform_albedo_location,
                    GLuint g_buffer_material_id_location)
{

    GLuint texture_index = GBUFFER_TEXTURES_SLOT;
    uniform_helper.setUniformInt(texture_index, g_buffer_lighting_uniform_position_location);
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, g_position);
    texture_index++;
    uniform_helper.setUniformInt(texture_index, g_buffer_lighting_uniform_normal_location);
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    texture_index++;
    uniform_helper.setUniformInt(texture_index, g_buffer_lighting_uniform_albedo_location);
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    texture_index++;
    uniform_helper.setUniformInt(texture_index, g_buffer_material_id_location);
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, g_material_id);


}

void GBuffer::update_window_params(GLfloat window_width, GLfloat window_height)
{
    this->window_width = window_width;
    this->window_height = window_height;
}

GLint GBuffer::get_id()
{
    return g_buffer;
}

GBuffer::~GBuffer()
{
}
