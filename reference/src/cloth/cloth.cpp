#include "cloth/cloth.hpp"

RectCloth::RectCloth(
    unsigned int nw,
    unsigned int nh,
    float dx,
    glm::mat4 transform
) : nw(nw), nh(nh), dx(dx), width(nw * dx), height(nh * dx)
{
    for (int ih = 0; ih < nh; ++ih) {
        for (int iw = 0; iw < nw; ++iw) {
            glm::vec3 init {iw * dx - width / 2.0f, ih * dx - height / 2.0f, 0};
            positions.push_back(
                transform * glm::vec4(init, 1.0f)
            );
        }
    }
}