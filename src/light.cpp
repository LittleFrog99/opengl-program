#include "utilities.h"

static const GLsizei width = 1024, height = 576;
static Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
static glm::vec3 lightPos = glm::vec3(1.0f, 0.0f, 0.0f);

static float vertices[] = {
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

static const glm::vec3 cubePositions[] = {
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

static glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

static GLuint objectVAO, lightVAO, VBO, diffuseMap, specularMap;
static Shader *lighting, *lamp;

static void setup() {
    lighting = new Shader("shaders/light/lighting_vertex.glsl", "shaders/light/lighting_fragment.glsl");
    lamp = new Shader("shaders/light/lamp_vertex.glsl", "shaders/light/lamp_fragment.glsl");

    glGenVertexArrays(1, &objectVAO); // vertex attribute object
    glGenBuffers(1, &VBO); // vetex buffer object
    glBindVertexArray(objectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*) (3 * sizeof (float)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*) (6 * sizeof (float)) );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    loadTexture("resources/container2.jpg", diffuseMap, false);
    loadTexture("resources/container2_specular.jpg", specularMap, false);
    lighting->use();
    lighting->setInt("material.diffuse", 0);
    lighting->setInt("material.specular", 1);

    glEnable(GL_DEPTH_TEST);
}

static void draw(float time) {
    lighting->use(); // draw lighting effect
    glm::mat4 model, view, projection;
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), float(width)/height, 0.1f, 100.0f);
    lighting->setMat4("view", view);
    lighting->setMat4("projection", projection);
    lighting->setFloat("material.shininess", 32.0f);

    lighting->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lighting->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    lighting->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lighting->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lighting->setVec3("pointLights[0].position", pointLightPositions[0]);
    lighting->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lighting->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lighting->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lighting->setFloat("pointLights[0].constant", 1.0f);
    lighting->setFloat("pointLights[0].linear", 0.09f);
    lighting->setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    lighting->setVec3("pointLights[1].position", pointLightPositions[1]);
    lighting->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lighting->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lighting->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lighting->setFloat("pointLights[1].constant", 1.0f);
    lighting->setFloat("pointLights[1].linear", 0.09f);
    lighting->setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    lighting->setVec3("pointLights[2].position", pointLightPositions[2]);
    lighting->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    lighting->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    lighting->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    lighting->setFloat("pointLights[2].constant", 1.0f);
    lighting->setFloat("pointLights[2].linear", 0.09f);
    lighting->setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    lighting->setVec3("pointLights[3].position", pointLightPositions[3]);
    lighting->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    lighting->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    lighting->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    lighting->setFloat("pointLights[3].constant", 1.0f);
    lighting->setFloat("pointLights[3].linear", 0.09f);
    lighting->setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    lighting->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lighting->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    lighting->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    lighting->setFloat("spotLight.constant", 1.0f);
    lighting->setFloat("spotLight.linear", 0.09f);
    lighting->setFloat("spotLight.quadratic", 0.032f);
    lighting->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lighting->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    glBindVertexArray(objectVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    for (int i = 0; i < 10; i++) {
        float angle = 20.0f * i;
        model = glm::mat4();
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lighting->setMat4("model", model);
        glm::mat3 normalMat(glm::transpose(glm::inverse(view * model)));
        lighting->setMat3("normalMat", normalMat);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    lamp->use();
    lamp->setMat4("view", view);
    lamp->setMat4("projection", projection);
    glBindVertexArray(lightVAO);
    for (int i = 0; i < 4; i++)
    {
        model = glm::mat4();
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        lamp->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
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

int light() {
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    delete lighting;
    delete lamp;
    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
