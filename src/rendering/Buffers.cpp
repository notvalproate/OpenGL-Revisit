#include "Buffers.hpp"

BufferBase::~BufferBase() {
	GLCall(glDeleteBuffers(1, &m_BufferID));
}

BufferBase::BufferBase(BufferBase&& other) noexcept {
	*this = std::move(other);
}

BufferBase& BufferBase::operator=(BufferBase&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_BufferID = other.m_BufferID;
	m_BufferType = other.m_BufferType;

	other.m_BufferID = 0;
	other.m_BufferType = 0;
}

void BufferBase::Bind() const {
	GLCall(glBindBuffer(m_BufferType, m_BufferID));
}

void BufferBase::Unbind() const {
	GLCall(glBindBuffer(m_BufferType, 0));
}