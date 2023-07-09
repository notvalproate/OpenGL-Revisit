#pragma once
#include "VertexArray.hpp"
#include "Buffers.hpp"
#include "Shader.hpp"

class FrameBuffer {
public:
	FrameBuffer(const unsigned int width, const unsigned int height, Shader* shader);

	void bind() const;
	void unbind() const;
	void draw() const;
private:
	unsigned int m_BufferID;
	unsigned int m_TextureID;
	unsigned int m_RenderBufferID;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;

	Shader* m_Shader;
};