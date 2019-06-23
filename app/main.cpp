#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 red;\n"
"out vec4 incolor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0);\n"
"   incolor = vec4(red, 1.0f);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 incolor;\n"
"uniform vec4 ucolor;\n"
"void main()\n"
"{\n"
"   FragColor = incolor;\n"
"}\n\0";


unsigned int indices[] = { 0, 1, 2 };

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

unsigned int createProgram(unsigned int vs, unsigned int fs){
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

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window = initOpenGl();
    if (window == NULL) return -1;
    
    int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    int shaderProgram = createProgram(vs, fs);
    glUseProgram(shaderProgram);
    glDeleteShader(fs);
    glDeleteShader(vs);
    
    
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // CREATE A VBO (Vertex Buffer Object)
    // OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
    // From that point on any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, which is VBO.
    // 2. copy our vertices array in a buffer for OpenGL to use
    
    float rf = .0f;
    float vertices[] = {
        //positions  //colors
        -.5f, -.5f, rf, .4f, 0.2f,
        .5f, -.5f,  .0f, rf, 0.6f,
        .0f, .5f,   .4f, 0.1f, rf
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // CREATE A Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //tell opengl how to read data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //IBO (index buffer object)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    int location = glGetUniformLocation(shaderProgram, "ucolor");
    
    float r = 0.0f;
    float g = 1.0f;
    float b = 1.0f;
    float step = .02f;
    
    checkForErrors();
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //render loop
    while(!glfwWindowShouldClose(window)) {
        //inputs
        processInput(window);
        rf += step * 2;
        glUniform4f(location, r, g, b, 1.0f);
        if (r > 1.0f || r < 0.0f) {
            step *= -1;
        }
        r += step;
        
        float vertices[] = {
            //positions  //colors
            -.5f, -.5f, rf, .4f, 0.2f,
            .5f, -.5f,  .0f, rf, 0.6f,
            .0f, .5f,   .4f, 0.1f, rf
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //rendering
        //glClearColor(.2f, .3f, .4f, 1.0f); //sets the color
        //glClearColor(0.8f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // clears buffer bits (other buffer GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT)
        
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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
