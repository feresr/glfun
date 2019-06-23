#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 red;\n"
"out vec3 incolor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0);\n"
"   incolor = red;\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 incolor;\n"
"uniform vec4 ucolor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(incolor.x * ucolor.x, incolor.y * ucolor.y, incolor.z * ucolor.z, 1.0f);\n"
"}\n\0";

float vertices[] = {
    //positions     //colors
    -.5f, -.5f,     .0f, .4f, 0.2f,
    .5f, -.5f,      .0f, .0f, 0.6f,
    .5f, .5f,       .4f, 0.1f, .0f,
    -.5f, .5f,      1.0, 0.9f, 0.1f
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
    
    //tell opengl how to read data
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //IBO (index buffer object)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    int colorUniform = glGetUniformLocation(shaderProgram, "ucolor");
    
    float time = 0.0f;
    float step = .02f;
    
    checkForErrors();
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //render loop
    while(!glfwWindowShouldClose(window)) {
        //inputs
        processInput(window);
        glUniform4f(
                    colorUniform,
                    (sin(time) + 1.0f) / 2.0f,
                    (sin(.6f * time) + 1.0f) / 2.0f,
                    (sin(.2f * time) + 1.0f) / 2.0f,
                    1.0f);
        time += step;
        
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
