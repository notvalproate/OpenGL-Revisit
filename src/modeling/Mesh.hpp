#pragma once
#include <vector>

class Mesh {
public:
	Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) :
		m_Vertices(std::move(vertices)), m_Indices(std::move(indices)) { }
private:
	std::vector<float> m_Vertices{};
	std::vector<unsigned int> m_Indices{};

	friend class Batch;
};