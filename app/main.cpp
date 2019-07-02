#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "STBIMAGE/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 uniform: are per-primitive parameters (constant during an entire draw call) ;
 attribute: are per-vertex parameters (typically : positions, normals, colors, UVs, ...) ;
 varying: are per-fragment (or per-pixel) parameters : they vary from pixels to pixels.
**/

const char *vertexShaderSource = "#version 330 core\n"

"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"out vec3 incolor;\n"
"out vec2 texCoord;\n"

"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, 0.0f, 1.0);\n"
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
 
float vertices[] = {
    //positions     //colors            //tex coors
    -.5f, -.5f,     .0f, .4f, 0.2f,     0.0f, 0.0f,
    .5f, -.5f,      .0f, .0f, 0.6f,     1.0f, 0.0f,
    .5f, .5f,       .4f, 0.1f, .0f,     1.0f, 1.0f,
    -.5f, .5f,      1.0, 0.9f, 0.1f,    0.0f, 1.0f
};

unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};

void resizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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
    
    //TODO: check fi linking ws ok?
    return shaderProgram;
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
    return window;
}

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

int main() {
    
    GLFWwindow* window = initOpenGl();
    if (window == NULL) return -1;
    
    int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    int shaderProgram = createProgram(vs, fs);
    glUseProgram(shaderProgram);
    glDeleteShader(fs);
    glDeleteShader(vs);
    
    // CREATE A VBO (Vertex Buffer Object)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // CREATE A Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //how much to move to get to the next vertex
    float stride = 7 * sizeof(float);
    
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // tex coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //IBO (index buffer object)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    checkForErrors();
    
    //Textures
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    unsigned int texture1 = createTexture("/Users/feresr/Workspace/learnOpenGL/app/container.jpg", GL_RGB);
    // OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side,
    // images usually have 0.0 at the top of the y-axis
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture2 = createTexture("/Users/feresr/Workspace/learnOpenGL/app/awesomeface.png", GL_RGBA);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // 0 refers to texture unit (GL_TEXTURE0)
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1); // 1 refers to texture unit (GL_TEXTURE1)
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    float cameraDistance = 2.0f;
    
    //render loop
    while(!glfwWindowShouldClose(window)) {
        //inputs
        processInput(window);
        
        //model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        //model = glm::translate(model, glm::vec3(.5f, -0.5f, .0f));
        
        //view
        //move the camera back so it's not too close to the near plane
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1 * cameraDistance));
        cameraDistance+=0.001f;

        //perspective
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(800.0f/600.0f), .1f, 100.0f);
        
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        unsigned int proyectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        int colorUniform = glGetUniformLocation(shaderProgram, "ucolor");
        glUniform4f(
                    colorUniform,
                    (sin((float) glfwGetTime()) + 1.0f) / 2.0f,
                    (sin(.6f * (float) glfwGetTime()) + 1.0f) / 2.0f,
                    (sin(.2f * (float) glfwGetTime()) + 1.0f) / 2.0f,
                    1.0f);
    
        //rendering
        //glClearColor(.2f, .3f, .4f, 1.0f); //sets the color
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //openGL primitives  GL_POINTS, GL_TRIANGLES and GL_LINE_STRIP.
        //swap the color buffer (a large buffer that contains color values for each pixel in GLFW's window)
        glfwSwapBuffers(window);
        //any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }
    
    checkForErrors();
    glfwTerminate();
    return 0;
}

void resizeCallback(GLFWwindow* window, int width, int height) {
    //set the opengl viewport size
    glViewport(0, 0, width, height);
}
