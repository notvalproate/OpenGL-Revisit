#include "FrameBuffer.hpp"
#include "GL/glew.h"
#include "../textures/TextureLoader.hpp"

#include <iostream>

FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height, Shader* shader) : m_Shader(shader) {
    glGenFramebuffers(1, &m_BufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);

    m_TextureID = textureloader::loadEmptyTexture(width, height, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

    glGenRenderbuffers(1, &m_RenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<float> vquad = {
       -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
       -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    std::vector<unsigned int> vindices = {
        0, 1, 2,
        2, 3, 0
    }; 

    VertexBuffer vbo(vquad);
    m_VAO = std::make_unique<VertexArray>(&vbo, m_Shader->getLayout());
    m_IBO = std::make_unique<IndexBuffer>(vindices);
}


void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID); 

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}


void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::draw() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    m_Shader->setUniform1i("frame", 0); 

    m_Shader->bind(); 
    m_VAO->bind(); 
    m_IBO->bind(); 
    GLCall(glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr));
}