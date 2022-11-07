#include <iostream>
#include <glad/glad.h>
#include <GLFWW/glfw3.h>
// GLM OpenGL Mathematics
//------------------------------------------------
#include <glm/glm/glm.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

//other
#include "Soruce/Attenuation.h"
#include "Soruce/Shader.h"
#include "Soruce/stb_image.h"
#include "Soruce/Camera.h"
#include "Soruce/Model.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//movement / camera
Camera camera(glm::vec3(0.f,0.f,3.f));
float mouseLastX = SCR_WIDTH / 2.f;
float mouseLastY = SCR_HEIGHT / 2.f;
bool bFirstMouse = true;

//very ugly time global variables
float deltaTime = 0.0f; // the time between the current and last frame
float lastFrame = 0.0f; // the of last frame

//light related
glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadTexture(const char* path);

int main() {
    
    glm::mat4 mat(1);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            mat[i][j] = 4 * i + j;
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    //end testing
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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
    
    //setting up depth test
    glEnable(GL_DEPTH_TEST);

    
    
    //custom models
    Shader ourShader("Shaders/ShadedModelLoadV.glsl", "Shaders/ShadedModelLoadF.glsl");
    Model ourModel("Models/backpack.obj");

    //light
    Shader lightShader("Shaders/LightContainer.svs", "Shaders/LightContainer.sfs");
    Model lightModel("Models/BlenderBox/aBox.obj");

    // cubes
    // Shader BoxShader("Shaders/SimpleEmissionV.glsl", "Shaders/SimpleEmissionF.glsl");
    
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //handlig time
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;
        
        float sineOfTime = sin(time / 1.f)/2.f + 0.5f;
        //input
        processInput(window);
        
        
        //rendering commands
        //clears and sets color buffer
        glClearColor(0.2f, 0.1f, 0.4f, 1.f);
        
        //clears Z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float radius = 3.f;
        glm::vec3 lightPostion = glm::vec3(sin(time), 0.f, cos(time)) * radius;
        
        
        // model rendering
        //------------------------------------------------
        
        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // set params to model
        Attenuation::setDistance(EAttenuationDistance::E100);
        ourShader.setVec3("pointLights[0].position",lightPostion);
        ourShader.setFloat("pointLights[0].constant", Attenuation::current.constant);
        ourShader.setFloat("pointLights[0].linear", Attenuation::current.linear);
        ourShader.setFloat("pointLights[0].quadratic", Attenuation::current.quadratic);
        ourShader.setVec3("pointLights[0].ambient", glm::vec3(0.1f));
        ourShader.setVec3("pointLights[0].diffuse", glm::vec3(1.f));
        ourShader.setVec3("pointLights[0].specular", glm::vec3(1.f));
        ourShader.setFloat("shineiness", 32.f);
        ourShader.setVec3("viewPos", camera.position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);

        
        // render the loaded model
        ourModel.Draw(ourShader);

        //light rendering
        //------------------------------------------------
        lightShader.use();
        
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPostion);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("model", model);

        //color
        lightShader.setVec3("lightColor", glm::vec3(1.f));

        lightModel.Draw(lightShader);
        
        
        

        
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
    camera.ProcessKeyboard(keyboardAxis, deltaTime);
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
unsigned int loadTexture(const char* path) {
    unsigned int texture;
    glGenTextures(1, &texture);

    

    
    
    // load and generate texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
    }
    else {
        std::cout << "ERROR::TEXTURE::LOAD_FAILED\n" << std::endl;
    }

    stbi_image_free(data); // free image memory
    return texture;
}
