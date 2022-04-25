#pragma once

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct RectCloth {
public:
    const float dx;
    const int nw;
    const int nh;

    const float width;
    const float height;
    std::vector<glm::vec3> positions;

public:
    RectCloth(unsigned int nw, unsigned int nh, float dx, glm::mat4 transform = glm::mat4(1.0));
    ~RectCloth() = default;

    size_t idxFromCoord(int iw, int ih) { return ih * nw + iw; };

    glm::vec3& getPosition(size_t idx) { return this->positions[idx]; };
    glm::vec3& getPosition(int iw, int ih) { return this->positions[idxFromCoord(iw, ih)]; };
};