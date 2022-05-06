#include "render/cloth.hpp"

#include <iostream>
#include <numbers>

RectClothRenderer::
RectClothRenderer(
    Shader* shader,
    FirstPersonCamera* camera,
    RectCloth* cloth,
    bool draw_sphere
) {
    this->shader = shader;
    this->camera = camera;
    this->cloth = cloth;
    this->draw_sphere = draw_sphere;

    this->initVertices();
    this->initIndices();
    this->updateNormals();

    this->glo.initData();
    /*******************************my code*************************************/
    this->createSphere(0.5,0,-1.0,0); // create the sphere
    this->sphere.initData(); // render the sphere
    /*******************************my code*************************************/
}

void RectClothRenderer::
draw() {
    this->updatePositions();
    this->updateNormals();

    // Update Data
    glBindBuffer(GL_ARRAY_BUFFER, glo.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * this->glo.vertices.size(), this->glo.vertices.data(), GL_STREAM_DRAW);

    // GLint previous;
    // glGetIntegerv(GL_POLYGON_MODE, &previous);

    this->shader->use();
    this->shader->setMat4("Projection", this->camera->getProjection());
    this->shader->setMat4("View", this->camera->getView());
    this->shader->setVec3("CameraPos", this->camera->getCameraPos());

    glBindVertexArray(glo.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, glo.VBO);

    // this->shader->setBool("DrawLine", false);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT); // We want flat mode
    // glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(glo.indices.size()), GL_UNSIGNED_INT, 0);

    this->shader->setBool("DrawLine", true);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // We want line mode
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(glo.indices.size()), GL_UNSIGNED_INT, 0);

    /*******************************my code*************************************/
    // render the sphere
    if(this->draw_sphere)
    {
        glBindVertexArray(sphere.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, sphere.VBO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(sphere.indices.size()), GL_UNSIGNED_INT, 0);
    }
    /*******************************my code*************************************/


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // glPolygonMode(GL_FRONT_AND_BACK, previous); // restore previous mode
}

void RectClothRenderer::
initVertices() {
    const int nh = this->cloth->nh;
    const int nw = this->cloth->nw;
    const size_t total = nh * nw;

    this->glo.vertices.clear();
    this->glo.vertices.resize(total);

    for (size_t i = 0; i < total; ++i) {
        this->glo.vertices[i].position = this->cloth->getPosition(i);
    }
}


void RectClothRenderer::
initIndices() {
    this->glo.indices.clear();

    const int nh = this->cloth->nh;
    const int nw = this->cloth->nw;

    this->glo.indices.reserve((nh - 1) * (nw - 1) * 6);
    for (int ih = 0; ih < nh - 1; ++ih) {
        for (int iw = 0; iw < nw - 1; ++iw) {
            size_t leftDownIdx = this->cloth->idxFromCoord(iw, ih);
            size_t rightDownIdx = this->cloth->idxFromCoord(iw + 1, ih);
            size_t leftUpIdx = this->cloth->idxFromCoord(iw, ih + 1);
            size_t rightUpIdx = this->cloth->idxFromCoord(iw + 1, ih + 1);

            this->glo.indices.push_back(leftDownIdx);
            this->glo.indices.push_back(rightDownIdx);
            this->glo.indices.push_back(rightUpIdx);

            this->glo.indices.push_back(leftDownIdx);
            this->glo.indices.push_back(rightUpIdx);
            this->glo.indices.push_back(leftUpIdx);
        }
    }
}

void RectClothRenderer::
updatePositions() {
    const int nh = this->cloth->nh;
    const int nw = this->cloth->nw;
    const size_t total = nh * nw;

    for (size_t i = 0; i < total; ++i) {
        this->glo.vertices[i].position = this->cloth->getPosition(i);
    }
}

void RectClothRenderer::
updateNormals() {
    const int nh = this->cloth->nh;
    const int nw = this->cloth->nw;
    const size_t total = nh * nw;

    for (size_t i = 0; i < total; ++i) {
        this->glo.vertices[i].normal = glm::vec3(0);
    }

    for (int ih = 0; ih < nh - 1; ++ih) {
        for (int iw = 0; iw < nw - 1; ++iw) {
            size_t leftDownIdx = this->cloth->idxFromCoord(iw, ih);
            size_t rightDownIdx = this->cloth->idxFromCoord(iw + 1, ih);
            size_t leftUpIdx = this->cloth->idxFromCoord(iw, ih + 1);
            size_t rightUpIdx = this->cloth->idxFromCoord(iw + 1, ih + 1);

            size_t lower[3] = {leftDownIdx, rightDownIdx, rightUpIdx};
            size_t upper[3] = {leftDownIdx, rightUpIdx, leftUpIdx};
            glm::vec3 lowerNormal = calcNormal(
                this->glo.vertices[leftDownIdx].position,
                this->glo.vertices[rightDownIdx].position,
                this->glo.vertices[rightUpIdx].position
            );
            glm::vec3 upperNormal = calcNormal(
                this->glo.vertices[leftDownIdx].position,
                this->glo.vertices[rightUpIdx].position,
                this->glo.vertices[leftUpIdx].position
            );
            for (int i = 0; i < 3; ++i) {
                this->glo.vertices[lower[i]].normal += lowerNormal;
                this->glo.vertices[upper[i]].normal += upperNormal;
            }
        }
    }

    for (size_t i = 0; i < total; ++i) {
        this->glo.vertices[i].normal = glm::normalize(this->glo.vertices[i].normal);
    }
}

/*******************************my code*************************************/
// create the vertex data of the sphere.
void RectClothRenderer::
createSphere(float radius, float rx, float ry, float rz) {
    auto& v = this->sphere.vertices;
    auto& ind = this->sphere.indices;

    float pi = std::numbers::pi_v<float>;
    
    int theta_sep = 20;
    float theta_delta = 2 * pi / theta_sep;

    int phi_sep = 10;
    float phi_delta = pi / phi_sep;

    int stride = phi_sep + 1;

    v.resize((theta_sep + 1) * (phi_sep + 1));
    for (int t = 0; t < theta_sep + 1; ++t)
    {
        float theta = t * theta_delta;
        for (int p = 0; p < phi_sep + 1; p++)
        {
            float phi = p * phi_delta;

            // set the position of the sphere as (0,-2,0)
            float x = radius * sinf(phi) * cosf(theta)+rx;
            float y = radius * sinf(phi) * sinf(theta)+ry;
            float z = radius * cosf(phi)+rz;

            v[t * stride + p].position = glm::vec3(x, y, z);
            v[t * stride + p].normal = normalize(glm::vec3(x, y, z));
        }
    }

    ind.reserve(3 * 2 * theta_sep * phi_sep);
    for (int t = 0; t < theta_sep; t++)
    {
        for (int p = 0; p < phi_sep; p++)
        {
            ind.push_back(t * stride + p);
            ind.push_back(t * stride + p + 1);
            ind.push_back((t + 1) * stride + p);

            ind.push_back(t * stride + p + 1);
            ind.push_back((t + 1) * stride + p);
            ind.push_back((t + 1) * stride + p + 1);
        }
    }
    this->sphere.initData();
}
/*******************************my code end*************************************/