#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM OpenGL Mathematics
//------------------------------------------------
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

//other
#include "Soruce/Attenuation.h"
#include "Soruce/Shader.h"
#include "Soruce/stb_image.h"
#include "Soruce/Camera.h"
#include "Soruce/Unity/GameObject.h"
#include "Soruce/Model.h"
#include "Soruce/Unity/SimpleScaleAnimationComponent.h"
#include "Soruce/Unity/Time.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int GRID_WIDTH = 10;
const unsigned int GRID_DEPTH = 7;
vector<int> _gridPosition = vector<int>(2);

std::vector<GameObject> gameObjects = std::vector<GameObject>(0);

//movement / camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
float mouseLastX = SCR_WIDTH / 2.f;
float mouseLastY = SCR_HEIGHT / 2.f;
bool bFirstMouse = true;

//light related
glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    Time::Init();

#pragma region GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //flips all images loaded from now so they are the correcty way
    stbi_set_flip_vertically_on_load(false);

    //mouse settings
    //------------------------------------------------
    //hide and capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //setup mouse input callback
    glfwSetCursorPosCallback(window, mouseCallback);

    //scroll callback
    glfwSetScrollCallback(window, scrollCallback);


    //keycallback
    glfwSetKeyCallback(window, key_callback);

    //setting up depth test
    glEnable(GL_DEPTH_TEST);

#pragma endregion

    //grid

    //populatiing grid
    for (unsigned int i = 0; i < GRID_WIDTH; ++i) {
        for (unsigned int j = 0; j < GRID_DEPTH; ++j) {
            GameObject gameObject = GameObject();
            SimpleScaleAnimationComponent* scaleComponent = new SimpleScaleAnimationComponent();
            gameObject.AddComponent(scaleComponent);
            gameObject.transform.position = glm::vec3(j, 0.f, i);
            gameObjects.push_back(gameObject);

        }
    }

    // getting cube model
    Model gridModel("Models/BlenderBox/aBox.obj");
    Shader gridShader("Shaders/Unlit/UnlitV.glsl", "Shaders/Unlit/UnlitF.glsl");

#pragma region CustomModels
    // //custom models
    // Shader ourShader("Shaders/ShadedModelLoadV.glsl", "Shaders/ShadedModelLoadF.glsl");
    // Model ourModel("Models/backpack.obj");

    //light
    // Shader lightShader("Shaders/LightContainer.svs", "Shaders/LightContainer.sfs");
    // Model lightModel("Models/BlenderBox/aBox.obj");

#pragma endregion


    glm::mat4 projection = glm::perspective(glm::radians(camera.fov),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //START
    for (int i = 0; i < gameObjects.size(); ++i) {
        gameObjects[i].Start();
    }


    //render loop
    while (!glfwWindowShouldClose(window)) {
        //handlig time
        Time::UpdateTime();
        // std::cout << Time::GetTime() << std:: endl;


        //input
        processInput(window);


        //rendering commands
        //clears and sets color buffer
        glClearColor(0.2f, 0.1f, 0.4f, 1.f);

        //clears Z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // UPDATE GAMEOBJECTS
        //-----------------------------------------------
        for (int i = 0; i < gameObjects.size(); ++i) {
            gameObjects[i].Update();
        }


        // DRAWS
        //------------------------------------------------

        //matrices
        glm::mat4 view = camera.GetViewMatrix();

        gridShader.use();
        gridShader.setMat4("projection", projection);
        gridShader.setMat4("view", view);

        //draw cubes
        for (unsigned int i = 0; i < gameObjects.size(); ++i) {
            //matrices
            glm::mat4 model = glm::mat4(1.f); // translate scale rotate
            model = glm::translate(model, gameObjects[i].transform.position);
            model = glm::scale(model, gameObjects[i].transform.scale);

            // model = glm::rotate(model, gameObjects[i].transform.rotation);
            gridShader.setMat4("model", model);

            //color
            glm::vec3 Color(1.f);

            if (_gridPosition[0] + _gridPosition[1] * GRID_DEPTH == i) {
                Color = glm::vec3(1.f);
                gameObjects[i].GetComponent<SimpleScaleAnimationComponent>()->PlayAnimation();
            }
            else {
                Color = glm::vec3(0.5f);
            }

            gridShader.setVec3("Color", Color);

            //draws
            gridModel.Draw(gridShader);
        }

        // std::cout << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;

        gridModel.Draw(gridShader);

#pragma region customModels
        // // don't forget to enable shader before setting uniforms
        // ourShader.use();
        //
        // // set params to model
        // Attenuation::setDistance(EAttenuationDistance::E100);
        // ourShader.setVec3("pointLights[0].position",lightPostion);
        // ourShader.setFloat("pointLights[0].constant", Attenuation::current.constant);
        // ourShader.setFloat("pointLights[0].linear", Attenuation::current.linear);
        // ourShader.setFloat("pointLights[0].quadratic", Attenuation::current.quadratic);
        // ourShader.setVec3("pointLights[0].ambient", glm::vec3(0.1f));
        // ourShader.setVec3("pointLights[0].diffuse", glm::vec3(1.f));
        // ourShader.setVec3("pointLights[0].specular", glm::vec3(1.f));
        // ourShader.setFloat("shineiness", 32.f);
        // ourShader.setVec3("viewPos", camera.position);
        //
        // // view/projection transformations
        // glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix();
        // ourShader.setMat4("projection", projection);
        // ourShader.setMat4("view", view);
        //
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        // ourShader.setMat4("model", model);
        //
        //
        // // render the loaded model
        // ourModel.Draw(ourShader);
        //
        //light rendering
        //------------------------------------------------
        // lightShader.use();
        //
        // lightShader.setMat4("projection", projection);
        // lightShader.setMat4("view", view);
        // model = glm::mat4(1.f);
        // model = glm::translate(model, lightPostion);
        // model = glm::scale(model, glm::vec3(0.2f));
        // lightShader.setMat4("model", model);
        //
        // //color
        // lightShader.setVec3("lightColor", glm::vec3(1.f));
        //
        // lightModel.Draw(lightShader);


# pragma endregion

        //grid


        //swap buffers and check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::vector<bool> keyPresses = std::vector<bool>(4);

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //movement
    glm::vec3 keyboardAxis = glm::vec3(0.f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        keyboardAxis.z += 1.f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        keyboardAxis.z += -1.f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        keyboardAxis.x += -1.f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        keyboardAxis.x += 1.f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        keyboardAxis.y += -1.f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        keyboardAxis.y += 1.f;
    camera.ProcessKeyboard(keyboardAxis, Time::GetDeltaTime());


}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (bFirstMouse) {
        bFirstMouse = false;
        mouseLastX = xpos;
        mouseLastY = ypos;
    }
    float offsetX = xpos - mouseLastX;
    float offsetY = ypos - mouseLastY;
    mouseLastX = xpos;
    mouseLastY = ypos;

    camera.ProcessMouseMovement(offsetX, offsetY, true, true);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessMouseScroll(yoffset, true);
    }
    else {
        camera.ProcessMouseScroll(yoffset, false);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Temp Moving in grid
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        _gridPosition[1] += 1;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        _gridPosition[1] += -1;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        _gridPosition[0] += 1;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        _gridPosition[0] -= 1;
    }
}
