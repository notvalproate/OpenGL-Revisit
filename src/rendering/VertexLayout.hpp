#pragma once
#include <vector>
#include <iostream>

enum class VertexAttribute : unsigned short {
	Position = 0x00,
	Normal = 0x01,
	TextureCoordinates = 0x02,
	MaterialIndex = 0x03
};

class VertexLayout {
public:
	VertexLayout() = default;
	VertexLayout(const std::vector<VertexAttribute>& layout) : m_Layout(layout) {
		for (const auto& e : layout) {
			m_Stride += getAttributeWidth(e);
		}
	}

	static std::size_t getAttributeWidth(VertexAttribute attribute) {
		if (attribute == VertexAttribute::Position || attribute == VertexAttribute::Normal) {
			return 3;
		}
		if (attribute == VertexAttribute::TextureCoordinates) {
			return 2;
		}
		if (attribute == VertexAttribute::MaterialIndex) {
			return 1;
		}
		return 0;
	}

	void push(VertexAttribute attribute) {
		m_Layout.push_back(attribute);
		m_Stride += getAttributeWidth(attribute);
		std::cout << m_Layout.size() << std::endl;
	}

	std::vector<VertexAttribute>::const_iterator begin() const { return m_Layout.begin(); }
	std::vector<VertexAttribute>::const_iterator end() const { return m_Layout.end(); }

	[[nodiscard]] std::size_t getStride() const { return m_Stride; }
private:
	std::vector<VertexAttribute> m_Layout{};
	std::size_t m_Stride{};
};