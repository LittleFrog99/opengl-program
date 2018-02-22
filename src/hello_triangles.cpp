#include "utilities.h"

static GLsizei width = 400, height = 400;

static const float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
     0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};

static const GLuint indices[] = {
    0, 1, 2,
};

static const char *vertexShaderSource =
        "#version 330 core \n"
        "layout (location = 1) in vec3 aPos; \n"
        "layout (location = 2) in vec3 aColor; \n"
        "out vec3 ourColor; \n"
        "void main() { \n"
        "   gl_Position = vec4(aPos, 1.0); \n"
        "   ourColor = aColor; \n"
        "} \0";

static const char *fragmentShaderSource =
        "#version 330 core \n"
        "in vec3 ourColor; \n"
        "out vec4 FragColor; \n"
        "void main() { \n"
        "   FragColor = vec4(ourColor, 1.0f); \n"
        "} \0";

static GLuint program, VAO, VBO, EBO;

static void checkCompileErrors(GLuint shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

static void checkLinkErrors(GLuint program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

static void compileShaders() {
    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkLinkErrors(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static void setup() {
    compileShaders();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)) );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void draw(float time) {
    glUseProgram(program);
    glBindVertexArray(VAO);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int helloTriangles() {
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Program", NULL, NULL); // window hint first, then create
    if (!window) {
        std::cout << "Failed to create an GLFW window. " << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    setup();

    // Draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    glfwTerminate();
    return 0;
}

