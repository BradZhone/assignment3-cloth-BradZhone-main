#pragma once

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

/*******************************my code*************************************/
struct Spring
{
    int idx; // the index of another particle
    float rest_length; // the rest length of two particles
};


struct clothParticle
{
    bool isPin = 0; // whether the particle is pined not to move
    std::vector<Spring> springs; // the spring relationship with other particles
};
/*******************************my code end*************************************/

struct RectCloth {
public:
    const float dx;
    const int nw;
    const int nh;

    const float width;
    const float height;
    std::vector<glm::vec3> positions;

    /*******************************my code*************************************/
    bool draw_sphere; // whether to draw a sphere
    bool wind; // whether to generate wind

    std::vector<glm::vec3> old_positions; // the position of each particle at t-1 timepoint (in order to make the simulation more stable)
    std::vector<glm::vec3> velocities; // velocity of each particle

    const float g = 9.8f; // gravaty coefficient
    const float K = 50.0f; // stiffness
    const float C = 0.005f; // damping
    const float totalMass = 1.0f; // sum mass of all particles
    float mass; // mass of single particle
    std::vector<clothParticle> particles; // particles compose the cloth
    /*******************************my code end*************************************/

public:
    RectCloth(unsigned int nw, unsigned int nh, float dx, glm::mat4 transform = glm::mat4(1.0), bool draw_sphere=0, bool wind=0);  //******my code*******
    ~RectCloth() = default;

    size_t idxFromCoord(int iw, int ih) { return ih * nw + iw; };

    glm::vec3& getPosition(size_t idx) { return this->positions[idx]; };
    glm::vec3& getPosition(int iw, int ih) { return this->positions[idxFromCoord(iw, ih)]; };

    /*******************************my code*************************************/
    void updateCloth(float time);
    /*******************************my code end*************************************/
};