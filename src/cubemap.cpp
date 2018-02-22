#include "utilities.h"
#include "data.h"

static const GLsizei width = 1024, height = 576;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

static Shader *program, *skybox;
static Model *mitsuba, *cube, *nanosuit;

static GLuint cubemap, skyboxVAO, skyboxVBO;

static void setup() {
    program = new Shader("shaders/reflection/reflection_map.vs.glsl", "shaders/reflection/reflection_map.fs.glsl");
    skybox = new Shader("shaders/skybox/skybox.vs.glsl", "shaders/skybox/skybox.fs.glsl");
    // mitsuba = new Model("resources/mitsuba/mitsuba.obj");
    // cube = new Model("resources/cube/cube.obj");
    nanosuit = new Model("resources/nanosuit/nanosuit.obj");

    std::string skybox_path = "resources/skybox/"; // load skybox faces
    vector<std::string> faces = {
        skybox_path + "right.jpg",
        skybox_path + "left.jpg",
        skybox_path + "top.jpg",
        skybox_path + "bottom.jpg",
        skybox_path + "back.jpg",
        skybox_path + "front.jpg"
    };
    cubemap = loadCubemap(faces);

    glGenVertexArrays(1, &skyboxVAO); // skybox vertex array
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), SKYBOX_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void draw(float time) {
    program->use();
    glm::mat4 model, view, projection; // view and projection matrices
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);
    program->setMat4("view", view);
    program->setMat4("projection", projection);

    glm::mat3 normalMat(glm::transpose(glm::inverse(view * model))); // model: mitsuba
    program->setMat4("model", model);
    program->setMat3("normalMat", normalMat);
    // mitsuba->Draw(*program);
    nanosuit->Draw(*program);

    model = glm::translate(model, glm::vec3(-3.0f, 0.45f, 0.0f)); // model: cube
    normalMat = glm::mat3(glm::transpose(glm::inverse(view * model)));
    program->setMat4("model", model);
    program->setMat3("normalMat", normalMat);
    // cube->Draw(*program);

    glDepthFunc(GL_LEQUAL);
    skybox->use(); // skybox
    skybox->setMat4("view", glm::mat4(glm::mat3(view)));
    skybox->setMat4("projection", projection);
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    delete program;
    delete skybox;
    delete mitsuba;
    delete cube;
    delete nanosuit;
    glfwTerminate();
    return 0;
}
