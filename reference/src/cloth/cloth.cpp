#include "cloth/cloth.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RectCloth::RectCloth(
    unsigned int nw,
    unsigned int nh,
    float dx,
    glm::mat4 transform,
    bool draw_sphere,
    bool wind
) : nw(nw), nh(nh), dx(dx), width(nw * dx), height(nh * dx), draw_sphere(draw_sphere), wind(wind)
{
    /*********************my code*****************************/
    mass = totalMass/(nw*nh);
    /*********************my code end*****************************/
    for (int ih = 0; ih < nh; ++ih) {
        for (int iw = 0; iw < nw; ++iw) {
            glm::vec3 init {iw * dx - width / 2.0f, ih * dx - height / 2.0f, 0};
            positions.push_back(
                transform * glm::vec4(init, 1.0f)
            );
            /*********************my code*****************************/
            velocities.push_back(
                glm::vec3(0.0f,0.0f,0.0f)
            );
            // add each particles and implement the spring-mass system here.
            clothParticle particle;
            // structural springs
            if (iw>0)
                particle.springs.push_back(Spring{int(idxFromCoord(iw-1,ih)),dx}); // left
            if (ih>0)
                particle.springs.push_back(Spring{int(idxFromCoord(iw,ih-1)),dx}); // up
            if (iw<nw-1)
                particle.springs.push_back(Spring{int(idxFromCoord(iw+1,ih)),dx}); // right
            if (ih<nh-1)
                particle.springs.push_back(Spring{int(idxFromCoord(iw,ih+1)),dx}); // down
            // shear springs
            if (iw>0 && ih>0)
                particle.springs.push_back(Spring{int(idxFromCoord(iw-1,ih-1)),dx*1.4142f}); // leftup
            if (iw<nw-1 && ih<nh-1)
                particle.springs.push_back(Spring{int(idxFromCoord(iw+1,ih+1)),dx*1.4142f}); // rightdown
            if (iw>0 && ih<nh-1)
                particle.springs.push_back(Spring{int(idxFromCoord(iw-1,ih+1)),dx*1.4142f}); // leftdown
            if (iw<nw-1 && ih>0)
                particle.springs.push_back(Spring{int(idxFromCoord(iw+1,ih-1)),dx*1.4142f}); // rightup
            // bending springs
            if (iw>1)
                particle.springs.push_back(Spring{int(idxFromCoord(iw-2,ih)),dx*2.0f}); // leftleft
            if (ih>1)
                particle.springs.push_back(Spring{int(idxFromCoord(iw,ih-2)),dx*2.0f}); // upup
            if (iw<nw-2)
                particle.springs.push_back(Spring{int(idxFromCoord(iw+2,ih)),dx*2.0f}); // rightright
            if (ih<nh-2)
                particle.springs.push_back(Spring{int(idxFromCoord(iw,ih+2)),dx*2.0f}); // downdown

            particles.push_back(particle);
        }
    }
    // pin the top left and right two points of the cloth
    particles[0].isPin=1;
    particles[nw-1].isPin=1;

    this->old_positions.assign(this->positions.begin(), this->positions.end());
}
/*********************my code end*****************************/

/*********************my code*****************************/
void RectCloth::updateCloth(float time) {
    for(size_t i = 0; i < this->particles.size(); ++i) {
        if(!this->particles[i].isPin) // only the particles that not been pined can move
        {
            float mass = this->mass;
            glm::vec3 position = this->old_positions[i];
            glm::vec3 velocity = this->velocities[i];
            glm::vec3 Force = glm::vec3(0.0f,0.0f,0.0f);  // sum of all kinds of force

            // add every forces into Force
            std::vector<glm::vec3> f; //forces

            // Gravity
            f.push_back(this->mass*glm::vec3(0.0f, -1.0f, 0.0f)*this->g);// G = mg
            
            // Spring
            for(size_t t=0; t< this->particles[i].springs.size();t++)
            {
                int idx = this->particles[i].springs[t].idx;
                float rest_length = this->particles[i].springs[t].rest_length;
                glm::vec3 dest_position = this->old_positions[idx];
                float delta_length = glm::distance(position,dest_position);

                glm::vec3 spring_force = this->K*(rest_length-delta_length)*(position-dest_position)/(delta_length);
                f.push_back(spring_force);
            }

            // Damping
            f.push_back(-this->C*velocity);

            // Wind
            if(this->wind)
                f.push_back(float(0.05*rand() / double(RAND_MAX))*glm::vec3(-1.0f, 0.0f, 1.0f));

            for (size_t j=0;j<f.size();j++)
                Force += f[j];
            this->velocities[i] = velocity + time*Force/mass;
            this->positions[i] = position + time*this->velocities[i];
            if(glm::distance(this->positions[i],glm::vec3(0.0f,-1.0f,0.0f))<0.5 && draw_sphere)
            {
                glm::mat4 transform2sphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                this->positions[i] = transform2sphere * glm::vec4(0.5f*glm::normalize(this->old_positions[i]-glm::vec3(0.0f,-1.0f,0.0f)),1.0f);
                this->velocities[i] = glm::vec3(0.0f,0.0f,0.0f);
            }
        }
    }
    this->old_positions.assign(this->positions.begin(), this->positions.end());
}
/*********************my code*****************************/