#include <iostream>
#include "main.h"
#include <glm/gtc/type_ptr.hpp>

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
    float stride = 8 * sizeof(float);
    
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // tex coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //    IBO (index buffer object) not needed (using glDrawElements, not glDrawArrays)
    
    //    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    //    unsigned int ibo;
    //    glGenBuffers(1, &ibo);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
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

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
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
    
    //render loop
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        
        processKeyboardInputs(window);
        
        glm::mat4 view = glm::lookAt(cameraPos,
                                     cameraPos + cameraFront, // (target)
                                     cameraUp);
        
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
        //glClearColor(.2f, .3f, .4f, 1.0f); //sets a color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (unsigned int i = 0; i < 10; i++) {
            //model
            glm::mat4 model = glm::mat4(1.0f);
            
            //model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float) glfwGetTime() * (i + 1), glm::vec3(cos(1.0f), sin(1.0f), 0.0f));
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // needs an indexBuffer
            glDrawArrays(GL_TRIANGLES, 0, 36); //glDrawArrays:: does not need an index buffer to be uploaded to the GPU
        }
        
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
