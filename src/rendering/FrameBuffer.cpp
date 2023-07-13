#include "FrameBuffer.hpp"
#include "GL/glew.h"
#include "../textures/TextureLoader.hpp"

#include <iostream>

static float QUAD[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
};

static unsigned int QUAD_INDICES[] = {
    0, 1, 2,
    2, 3, 0
};

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
    
    VertexBuffer vbo(QUAD);
    m_VAO = std::make_unique<VertexArray>(&vbo, m_Shader->getLayout());
    m_IBO = std::make_unique<IndexBuffer>(QUAD_INDICES);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept {
    *this = std::move(other);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    m_BufferID = other.m_BufferID;
    m_TextureID = other.m_TextureID;
    m_RenderBufferID = other.m_RenderBufferID;

    m_VAO = std::move(other.m_VAO);
    m_IBO = std::move(other.m_IBO);

    m_Shader = other.m_Shader;

    other.m_BufferID = 0;
    other.m_TextureID = 0;
    other.m_RenderBufferID = 0;
    other.m_Shader = nullptr;
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