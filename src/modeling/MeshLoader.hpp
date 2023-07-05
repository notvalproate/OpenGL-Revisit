#pragma once
#include "Mesh.hpp"
#include "../rendering/VertexLayout.hpp"

#include <assimp/scene.h>

class MeshLoader {
public:
	MeshLoader() = default;

	Mesh loadMesh(const aiMesh* mesh) const;
	void setLayout(const VertexLayout& layout);
private:
	VertexLayout m_Layout;

	void processVertex(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const;
	void processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices) const;
};