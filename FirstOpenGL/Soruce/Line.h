#pragma once
#include <glm/glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFWW/glfw3.h>


using namespace std;
using namespace glm;
class Line {
    int shaderProgram;
    unsigned int VBO, VAO;
    vector<float> vertices;
    vec3 startPoint;
    vec3 endPoint;
    mat4 MVP;
    vec3 lineColor;
public:
    Line(vec3 start, vec3 end);

    int setMVP(mat4 mvp);

    int setColor(vec3 color);

    int draw();

    ~Line();
};