#include "MeshLoader.hpp"

Mesh MeshLoader::loadMesh(const aiMesh* mesh) const {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
		processVertex(i, mesh, vertices);
	}

	processIndices(mesh, indices);

	return std::move(Mesh(vertices, indices));
}

void MeshLoader::setLayout(const VertexLayout& layout) {
	m_Layout = layout;
}

void MeshLoader::processVertex(std::size_t index, const aiMesh* mesh, std::vector<float>& vertices) const {
	for (const auto& attribute : m_Layout) {
		if (attribute == VertexAttribute::Position) {
			vertices.insert(vertices.end(), { mesh->mVertices[index].x,  mesh->mVertices[index].y, mesh->mVertices[index].z });
		}
		else if (attribute == VertexAttribute::Normal) {
			if (mesh->HasNormals()) {
				vertices.insert(vertices.end(), { mesh->mNormals[index].x,  mesh->mNormals[index].y, mesh->mNormals[index].z });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });
			}
		}
		else if (attribute == VertexAttribute::TextureCoordinates) {
			if (mesh->mTextureCoords[0]) {
				vertices.insert(vertices.end(), { mesh->mTextureCoords[0][index].x,  mesh->mTextureCoords[0][index].y });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f });
			}
		}
		else if (attribute == VertexAttribute::MaterialIndex) {
			vertices.push_back(((mesh->mMaterialIndex - 1) % 8) + 0.1f);
		}
	}
}

void MeshLoader::processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices) const {
	for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (std::size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]); 
		}
	}
}