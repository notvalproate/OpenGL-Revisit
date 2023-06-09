#pragma once
#include <GL/glew.h>
#include <span>

class BufferBase {
public:
	template<typename T>
	BufferBase(const std::span<T>& p_Data, unsigned int p_Type) : m_BufferType(p_Type) {
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(m_BufferType, m_BufferID);
		glBufferData(m_BufferType, sizeof(T) * p_Data.size(), p_Data.data(), GL_STATIC_DRAW);
		glBindBuffer(m_BufferType, 0);
	}
	~BufferBase();

	BufferBase(const BufferBase& other) = delete;
	BufferBase& operator=(const BufferBase& other) = delete;

	BufferBase(BufferBase&& other) noexcept;
	BufferBase& operator=(BufferBase&& other) noexcept;

	void Bind();
	void Unbind();

	[[nodiscard]] unsigned int GetID() const { return m_BufferID; }
protected:
	unsigned int m_BufferID;
	unsigned int m_BufferType;
};

class VertexBuffer : public BufferBase {
public:
	VertexBuffer(const std::span<float>& p_Data) : BufferBase(p_Data, GL_ARRAY_BUFFER) { }
private:
};

class IndexBuffer : public BufferBase {
public:
	IndexBuffer(const std::span<unsigned int>& p_Data) 
		: BufferBase(p_Data, GL_ELEMENT_ARRAY_BUFFER)
		, m_IndexCount(p_Data.size()) { }
	[[nodiscard]] unsigned int GetCount() const { return m_IndexCount; }
private:
	size_t m_IndexCount;
};