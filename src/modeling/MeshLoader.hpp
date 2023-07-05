#pragma once
#include "Mesh.hpp"
#include "../rendering/VertexLayout.hpp"

#include <assimp/scene.h>

class MeshLoader {
public:
	MeshLoader() = default;

	Mesh loadMesh(const aiMesh* mesh) const;
	void setLayout(const VertexLayout& layout);
	void clean();
private:
	using FunctionPtrType = void(MeshLoader::*)(std::size_t, const aiMesh*, std::vector<float>&) const;

	std::vector<FunctionPtrType> m_LayoutFunctions;

	void processVertex(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const;
	void processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices) const;

	void getPosition(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const;
	void getNormal(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const;
	void getTextureCoordinates(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const;
	void getMaterialIndex(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const;
};