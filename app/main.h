//
//  main.h
//  app
//
//  Created by Fernando Raviola on 03/07/2019.
//  Copyright © 2019 Fernando Raviola. All rights reserved.
//

#ifndef main_h
#define main_h

#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "STBIMAGE/stb_image.h"
#include <glm/gtc/matrix_transform.hpp>

// projection
glm::mat4 projection = glm::perspective(45.0f, float(800.0f/600.0f), .1f, 100.0f);

void checkForErrors() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL error] (" << error <<")" << std::endl;
    }
    std::cout << std::endl;
}

unsigned int createTexture(const char* path, GLenum format) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    
    stbi_image_free(data);
    return texture;
}

float vertices[] = {
    -0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    
    -0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    
    -0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,.0f, .4f, 0.2f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,.0f, .4f, 0.2f,  1.0f, 0.0f,
    
    0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,.0f, .4f, 0.2f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    
    -0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, .0f, .4f, 0.2f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, .0f, .4f, 0.2f, 0.0f, 1.0f
};

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};


/**
 uniform: are per-primitive parameters (constant during an entire draw call) ;
 attribute: are per-vertex parameters (typically : positions, normals, colors, UVs, ...) ;
 varying: are per-fragment (or per-pixel) parameters : they vary from pixels to pixels.
 **/

const char *vertexShaderSource = "#version 330 core\n"

"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"out vec3 incolor;\n"
"out vec2 texCoord;\n"

"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   texCoord = aTexCoord;"
"   incolor = aColor;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"

"uniform vec4 ucolor;\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"

"in vec3 incolor;\n"
"in vec2 texCoord;\n"

"out vec4 fragmentColor;\n"

"void main()\n"
"{\n"
"   fragmentColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), ucolor.x);\n"
"}\n\0";

bool firstMouse = true;
float lastX = 400.0f;
float lastY = 300.0f;
float yaw = -90.0f;
float pitch = 0.0f;

//move the camera back so it's not too close to the near plane
glm::vec3 cameraPos = glm::vec3(0, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void resizeCallback(GLFWwindow* window, int width, int height) {
    //set the opengl viewport size
    glViewport(0, 0, width, height);
}

GLFWwindow* initOpenGl() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //ios
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //Crete a window, fail fast if this cannot be done
    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to crete GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)  glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    
    //register window resize callback
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSwapInterval(1);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture mouse events
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glEnable(GL_DEPTH_TEST);
    return window;
}

unsigned int compileShader(GLenum type, const char* source) {
    unsigned int shader;
    shader = glCreateShader(type);
    //shader, how many strings, actual source code
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return shader;
}

unsigned int createProgram(unsigned int vs, unsigned int fs) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    
    glLinkProgram(shaderProgram);
    
    //TODO: check if linking was ok?
    return shaderProgram;
}

float deltaTime = 0.0f;
float lastTime = 0.0f;

void processKeyboardInputs(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    float cameraSpeed = 3.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

#endif /* main_h */
