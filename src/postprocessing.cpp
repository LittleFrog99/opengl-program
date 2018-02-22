#include "utilities.h"

static const GLsizei width = 1024, height = 576;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

static Shader *program, *postprocessing;
static Model *mitsuba, *cube;
static GLuint framebuffer, texColorBuffer, rbo, quadVAO, quadVBO;

static void setup() {
    program = new Shader("shaders/model/model_vertex.glsl", "shaders/model/model_fragment.glsl");
    postprocessing = new Shader("shaders/postprocessing/postprocessing_vertex.glsl",
                                "shaders/postprocessing/postprocessing_fragment.glsl");
    mitsuba = new Model("resources/mitsuba/mitsuba.obj");
    cube = new Model("resources/cube/cube.obj");

    const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)) );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    postprocessing->use();
    postprocessing->setInt("screenTex", 0);
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void draw(float time) {
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->use();

    glm::vec3 pointLightPos(1.0f, 2.0f, 1.0f);
    program->setFloat("material.shininess", 32.0f); // lighting
    program->setVec3("pointLight.position", pointLightPos);
    program->setVec3("pointLight.ambient", glm::vec3(0.1f));
    program->setVec3("pointLight.diffuse", glm::vec3(0.8f, 0.2f, 0.2f));
    program->setVec3("pointLight.specular", glm::vec3(1.0f));
    program->setFloat("pointLight.constant", 1.0f);
    program->setFloat("pointLight.linear", 0.09f);
    program->setFloat("pointLight.quadratic", 0.032f);
    program->setVec3("dirLight.direction", 2.0f, -1.0f, -1.0f);
    program->setVec3("dirLight.ambient", glm::vec3(0.05f));
    program->setVec3("dirLight.diffuse", glm::vec3(0.6f, 0.3f, 0.2f));
    program->setVec3("dirLight.specular", glm::vec3(0.2f));

    glm::mat4 model, view, projection; // view and projection matrices
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);
    program->setMat4("view", view);
    program->setMat4("projection", projection);

    glm::mat3 normalMat(glm::transpose(glm::inverse(view * model))); // model 1: mitsuba
    program->setMat4("model", model);
    program->setMat3("normalMat", normalMat);
    mitsuba->Draw(*program);

    model = glm::translate(model, glm::vec3(-3.0f, 0.45f, 0.0f)); // model 2: cube
    normalMat = glm::mat3(glm::transpose(glm::inverse(view * model)));
    program->setMat4("model", model);
    program->setMat3("normalMat", normalMat);
    cube->Draw(*program);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    postprocessing->use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
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

int drawModel() {
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL program", NULL, NULL); // window hint first, then create
    if (!window) {
        std::cout << "Failed to create an GLFW window. " << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setup();

    // Draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window, glfwGetTime());
        glEnable(GL_DEPTH_TEST);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    delete program;
    delete mitsuba;
    delete cube;
    glfwTerminate();
    return 0;
}
