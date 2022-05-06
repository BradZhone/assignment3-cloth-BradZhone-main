#pragma once

#include <array>

#include <glm/glm.hpp>

#include "camera/fpc.hpp"
#include "shader/shader.hpp"
#include "cloth/cloth.hpp"

class RectClothRenderer {
private:
    struct VertexData {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct GLObject {
        GLuint VAO;
        GLuint VBO; std::vector<VertexData> vertices;
        GLuint EBO; std::vector<GLuint> indices;

        GLObject() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
        };
        ~GLObject() {
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        };
        void initData() {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), vertices.data(), GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                sizeof(VertexData),
                (void*)offsetof(VertexData, position)
            );

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                sizeof(VertexData),
                (void*)offsetof(VertexData, normal)
            );

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        };
    };

    Shader* shader;
    FirstPersonCamera* camera;
    RectCloth* cloth;

    GLObject glo;
    /*******************************my code*************************************/
    bool draw_sphere;
    GLObject sphere; // render a sphere
    /*******************************my code*************************************/

public:
    RectClothRenderer(
        Shader* shader,
        FirstPersonCamera* camera,
        RectCloth* cloth,
        bool draw_sphere //******my code*******
    );
    ~RectClothRenderer() = default;

    void draw();
    /*******************************my code*************************************/
    void createSphere(float radius, float rx, float ry, float rz);
    /*******************************my code end*************************************/

private:
    void initVertices();
    void updatePositions();
    void updateNormals();
    void initIndices();

    static glm::vec3 calcNormal(
        const glm::vec3& v1,
        const glm::vec3& v2,
        const glm::vec3& v3
    ) { return glm::normalize(glm::cross(v2 - v1, v3 - v1)); };
};