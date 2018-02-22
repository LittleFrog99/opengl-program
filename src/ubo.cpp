#include "utilities.h"

static const GLsizei width = 1024, height = 576;

// Cameras
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static Shader *ubo[4];
static Model *cube;
GLuint uboMatrices;

static void setup() {
    ubo[0] = new Shader("shaders/ubo/ubo.vs.glsl", "shaders/ubo/ubo_red.fs.glsl");
    ubo[1] = new Shader("shaders/ubo/ubo.vs.glsl", "shaders/ubo/ubo_blue.fs.glsl");
    ubo[2] = new Shader("shaders/ubo/ubo.vs.glsl", "shaders/ubo/ubo_green.fs.glsl");
    ubo[3] = new Shader("shaders/ubo/ubo.vs.glsl", "shaders/ubo/ubo_yellow.fs.glsl");
    cube = new Model("resources/cube/cube.obj");

    for (int i = 0; i < 4; i++) {
        GLuint index = glGetUniformBlockIndex(ubo[i]->ID, "Matrices");
        glUniformBlockBinding(ubo[i]->ID, index, 0); // bind block indices with binding points: Matrices -> 0
    }

    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); // bind ubo to binding points: uboMatrices -> 0

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static void draw(float time) {
    glm::mat4 view = camera.GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    const vector<glm::vec3> positions {
        glm::vec3(-0.75, 0.75, 0),
        glm::vec3(0.75, 0.75, 0),
        glm::vec3(-0.75, -0.75, 0),
        glm::vec3(0.75, -0.75, 0)
    };
    for (int i = 0; i < 4; i++) {
        ubo[i]->use();
        glm::mat4 model = glm::translate(glm::mat4(), positions[i]);
        ubo[i]->setMat4("model", model);
        cube->Draw(*ubo[i]);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = width / 2.0f, lastY = height / 2.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window, float time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static float deltaTime = 0.0f, lastFrame = 0.0f;
    deltaTime = time - lastFrame;
    lastFrame = time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main() {
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Program", NULL, NULL); // window hint first, then create
    if (!window) {
        std::cout << "Failed to create an GLFW window. " << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setup();

    // Draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window, glfwGetTime());
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    glfwTerminate();
    return 0;
}
