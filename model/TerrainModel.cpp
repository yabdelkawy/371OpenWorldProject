//
// Created by danseremet on 2020-04-04.
//

#include "TerrainModel.h"

#include <utility>

TerrainModel::TerrainModel(std::vector<std::vector<float>> heights, std::vector<std::vector<glm::vec3>> colors,
        std::map<std::string, Shader *> shadersMap, std::map<std::string, Texture *> texturesMap) :
        Model(std::move(shadersMap), std::move(texturesMap)), heights(std::move(heights)), colors(std::move(colors)) {
}

void TerrainModel::loadMeshes() {
    auto *bMesh = new TerrainMesh{heights, colors, shadersMap, texturesMap};
    bMesh->loadMesh();
    meshes.push_back(bMesh);
}
