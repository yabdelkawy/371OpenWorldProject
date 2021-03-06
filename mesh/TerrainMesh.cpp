//
// Created by danseremet on 2020-04-04.
//

#include "TerrainMesh.h"


#include <utility>

using namespace glm;

TerrainMesh::TerrainMesh(int chunkX, int chunkZ, std::vector<std::vector<float>> heights, std::vector<std::vector<glm::vec3>> colors,
                         std::map<std::string, Shader *> shadersMap, std::map<std::string, Texture *> texturesMap)
        : Mesh(std::move(shadersMap), std::move(texturesMap)), chunkX(chunkX), chunkZ(chunkZ), heights(std::move(heights)), colors(std::move(colors)) {
}

void TerrainMesh::loadVertices() {
    int lastIndex = heights.size() - 1;

    int terrainSize = heights.size() - 1;
    int dx = chunkX * terrainSize;
    int dz = chunkZ * terrainSize;

    for (int z{0}; z < lastIndex; z += 2) {

        for (int x{0}; x < lastIndex; x += 2) {

            // 9 position vertices based on height
            std::vector<vec3> positions{};
            positions.push_back({x + dx, heights[z][x], z + dz});
            positions.push_back({x + dx + 1, heights[z][x + 1], z + dz});
            positions.push_back({x + dx + 2, heights[z][x + 2], z + dz});

            positions.push_back({x + dx, heights[z + 1][x], z + dz + 1});
            positions.push_back({x + dx + 1, heights[z + 1][x + 1], z + dz + 1});
            positions.push_back({x + dx + 2, heights[z + 1][x + 2], z + dz + 1});

            positions.push_back({x + dx, heights[z + 2][x], z + dz + 2});
            positions.push_back({x + dx + 1, heights[z + 2][x + 1], z + dz + 2});
            positions.push_back({x + dx + 2, heights[z + 2][x + 2], z + dz + 2});


            // 6 colors for the 8 triangles
            std::vector<vec3> orderedColors{};
            // 0  1 1  2
            // 3  4 4  5
            orderedColors.push_back({colors[z][x]});            // 0
            orderedColors.push_back({colors[z][x + 1]});        // 1
            orderedColors.push_back({colors[z][x + 1]});        // 1
            orderedColors.push_back({colors[z][x + 2]});        // 2
            orderedColors.push_back({colors[z + 1][x]});        // 3
            orderedColors.push_back({colors[z + 1][x + 1]});    // 4
            orderedColors.push_back({colors[z + 1][x + 1]});    // 4
            orderedColors.push_back({colors[z + 1][x + 2]});    // 5

            // 24 ordered indices for drawing 8 triangles
            std::vector<int> indices = {
                    0, 3, 4,
                    0, 4, 1,
                    1, 4, 2,
                    4, 5, 2,
                    3, 6, 4,
                    6, 7, 4,
                    4, 7, 8,
                    4, 8, 5,
            };

            // 8 ordered normals for 8 triangles
            std::vector<vec3> orderedNormals{};

            orderedNormals.push_back(normal(positions[indices[0]], positions[indices[1]], positions[indices[2]]));
            orderedNormals.push_back(normal(positions[indices[3]], positions[indices[4]], positions[indices[5]]));
            orderedNormals.push_back(normal(positions[indices[6]], positions[indices[7]], positions[indices[8]]));
            orderedNormals.push_back(normal(positions[indices[9]], positions[indices[10]], positions[indices[11]]));
            orderedNormals.push_back(normal(positions[indices[12]], positions[indices[13]], positions[indices[14]]));
            orderedNormals.push_back(normal(positions[indices[15]], positions[indices[16]], positions[indices[17]]));
            orderedNormals.push_back(normal(positions[indices[18]], positions[indices[19]], positions[indices[20]]));
            orderedNormals.push_back(normal(positions[indices[21]], positions[indices[22]], positions[indices[23]]));

            // 24 ordered positions for the 8 vertices
            std::vector<vec3> orderedPositions{};
            for (auto i : indices) {
                orderedPositions.push_back(positions[i]);
            }

            // For every 3 of each data combine into a Vertex in order
            for (int i{0}; i < indices.size(); i += 3) {

                Vertex v0 = {orderedPositions[i], orderedColors[i / 3], orderedNormals[i / 3]};
                Vertex v1 = {orderedPositions[i + 1], orderedColors[i / 3], orderedNormals[i / 3]};
                Vertex v2 = {orderedPositions[i + 2], orderedColors[i / 3], orderedNormals[i / 3]};

                vertices.push_back(v0);
                vertices.push_back(v1);
                vertices.push_back(v2);
            }
        }
    }

}

void TerrainMesh::setupVertices() { // cube mesh
    // vao
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex vbo
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    // vertex normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);
}

void TerrainMesh::draw() {
    shadersMap["basic"]->use();
    mat4 model{1.0f};
    shadersMap["basic"]->setMat4("worldMatrix", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void TerrainMesh::toggleShowTexture() {
    showTexture = !showTexture;
}

vec3 TerrainMesh::normal(vec3 a, vec3 b, vec3 c) {
    vec3 vec1 = a - b;
    vec3 vec2 = b - c;
    return cross(vec1, vec2);
}