#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM OpenGL Mathematics
//------------------------------------------------
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

//other
#include "Soruce/Shader.h"
#include "Soruce/stb_image.h"
#include "Soruce/Camera.h"


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
// testMain();
//     return 0;
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

    
    //building and compiling our shader program
    // -----------------------------------------
    Shader containerShader("Shaders/Shader.svs",
        "Shaders/Shader.sfs");

    Shader lightShader("Shaders/LightContainer.svs",
        "Shaders/LightContainer.sfs");


    //setup vectex data (and buffers(s)) and configure vertex attributes
    // ----------------------------------------------------------

    //creating a simple triangle vertecie positions
    float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float texCoords[] = {
        0.0f, 0.0f, // bottom left
        1.0f, 0.0f, // bottom right
        0.5f, 1.0f  // top middle
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    //creating a VAO : VERTEX ARRAY OBJECT and VBO : VERTEX BUFFER OBJECT
    unsigned int VAO1, VBO1, EBO;

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO);


    //bind and set Vertex Buffer Objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind and set Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //bind Vertex Array Object
    glBindVertexArray(VAO1);
    //configure vertex attributes
    //------------------------------------------------
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //UV space
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //setting up light container data
    //------------------------------------------------
    unsigned int LightVAO;
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    //vertex attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    
    // funcy stb_image stuff
    //------------------------------------------------
    
    //generating and setup image
    unsigned int texture = loadTexture("Textures/container2.png");
    //generates and loads second texture
    unsigned int texture2 = loadTexture("Textures/container2_specular.png");

    unsigned int texture3 = loadTexture("Textures/matrix.jpg");
    
    
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

        //exercise updating light position
        const float radius = 2.f;
        glm::vec3 origin = glm::vec3(0.f, 1.f, 0.f);
        float xVal = sin(time) * radius;
        float zVal = cos(time) * radius;
        glm::vec3 newPos = glm::vec3(xVal, 0.f, zVal) + origin;
        lightPosition = newPos;
        
        
        //rendering
        //------------------------------------------------
        
        //projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
        //view matrix
        glm::mat4 view = camera.GetViewMatrix();
        
        
        
            
        
        //activate shader
        containerShader.use();
        //activate texture
        containerShader.setInt("material.diffuse", 0);
        containerShader.setInt("material.specular", 1);
        containerShader.setInt("material.emission", 2);
        
        
        //updates shaders matricies
        containerShader.setMat4("view", view);
        containerShader.setMat4("projection", projection);

        //Matierial properties
        containerShader.setVec3("material.ambient", glm::vec3(1.f));
        containerShader.setVec3("material.diffuse", glm::vec3(1.f));
        containerShader.setVec3("material.specular", glm::vec3(1.f));
        containerShader.setFloat("material.shininess", .25 * 128);
        
        //Light color
        
        glm::vec3 lightDiff =  glm::vec3(1.0f); 
        glm::vec3 lightAmbient =  glm::vec3(0.1f);
        glm::vec3 lightSpecular = glm::vec3(1.f);
        
        //light properties
        containerShader.setVec3("light.ambient",  lightAmbient);
        containerShader.setVec3("light.diffuse",  lightDiff); // darken diffuse light a bit
        containerShader.setVec3("light.specular", lightSpecular);
        containerShader.setFloat("light.constant", 1.f);
        containerShader.setFloat("light.linear", 0.09f);
        containerShader.setFloat("light.quadratic", 0.032f);	
        //update cointainer colors
        containerShader.setVec3("light.position", camera.position);
        containerShader.setVec3("light.direction", camera.forward);
        containerShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        containerShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        
        containerShader.setVec3("viewPos", camera.position);
        
        //bind vertex array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        glBindVertexArray(VAO1);
        
        for (int i = 0; i < 10; ++i) {
            //model matris
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, glm::radians(angle) * 7.f, glm::vec3(1.f,0.3f,0.5f));
            containerShader.setMat4("model", model);
                
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        //drawing cube
        

        //rendering lightContainer
        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("lightColor", lightDiff);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f));
        
        lightShader.setMat4("model", model);
        
        glBindVertexArray(LightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //swap buffers and check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }

    //de allocate all resources one the outlived the purpose
    glDeleteVertexArrays(1, &VAO1);
    glDeleteVertexArrays(1, &LightVAO);
    glDeleteBuffers(1, &VBO1);
    // glDeleteProgram(shaderProgram2);


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
        camera.ProcessMouseScroll(yoffset);
    }
    else {
        camera.speed += yoffset;
    }
}
unsigned int loadTexture(const char* path) {
    unsigned int texture;
    glGenTextures(1, &texture);

    

    //flips all images loaded from now so they are the correcty way
    stbi_set_flip_vertically_on_load(true);
    
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
