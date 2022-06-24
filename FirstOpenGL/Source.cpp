#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Soruce/Shader.h"

//vertex shader code in GLSL
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/*const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderOneSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";*/
/*const char* fragmentShaderTwoSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1f, 0.f, 1.0f);\n"
"}\n\0";*/

const char* vertexShaderSoruceShaderTut = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char* fragmentShaderSoruceShaderTut = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int main() {
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

    
    //building and compiling our shader program
    // -----------------------------------------
    Shader shader("Shaders/Shader.svs",
        "Shaders/Shader.sfs");
    


    //setup vectex data (and buffers(s)) and configure vertex attributes
    // ----------------------------------------------------------

    //creating a simple triangle vertecie positions
    float vertices[] = {
        //positions             //colors
         0.5f, -0.5f, 0.0f,     1.f, 0.f, 0.f, 
        -0.5f, -0.5f, 0.0f,     0.f, 1.f, 0.f,  
         0.0f,  0.5f, 0.0f,     0.f, 0.f, 1.f
    };

    float vertTri1[] = {
        -0.75f, 0.5f, 0.f,
        -1.f, 0.f, 0.f,
         0.f, 0.f, 0.f,
    };

    float vertTri2[] = {
         0.75f, 0.5f, 0.f,
         1.f, 0.f, 0.f,
         0.f, 0.f, 0.f,
    };

    unsigned int indices[] = {
        0,1,3, // first triangle
        1,2,3 // second --||--
    };

    //creating a VAO : VERTEX ARRAY OBJECT and VBO : VECTEX BUFFER OBJECT
    unsigned int VAO1, VBO1;

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    //bind Vertex Array Object
    glBindVertexArray(VAO1);

    //bind and set Vertex Buffer Objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //configure vertex attributes
    //------------------------------------------------
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();
        float sineOfTime = sin(time / 1.f)/2.f + 0.5f;
        //input
        processInput(window);

        //rendering commands
        glClearColor(sineOfTime, sineOfTime, sineOfTime, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //draw first triangle
        //------------------------------------------------
        //sets the uniform value
        shader.use();
        shader.setFloat("offset", 0.4f);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //swap buffers and check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //de allocate all resources one the outlived the purpose
    glDeleteVertexArrays(1, &VAO1);
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
    

}
