#include "utilities.h"

static const GLsizei width = 1024, height = 576;
static Camera camera(glm::vec3(0.0f, 0.0f,55.0f));

static Shader *program;
static Model *planet, *rock;
static TextRenderer *text;
static FrameCounter *counter;
static glm::mat4 *modelMatrices;
static GLuint ASTEROID_AMOUNT = 20000;

static void setup() {
    program = new Shader("shaders/instance/instance.vs.glsl", "shaders/instance/instance.fs.glsl");
    rock = new Model("resources/rock/rock.obj");
    planet = new Model("resources/planet/planet.obj");
    text = new TextRenderer("resources/IBMPlexMono-Regular.ttf", glm::ivec2(width, height));
    counter = new FrameCounter(text);

    modelMatrices = new glm::mat4[ASTEROID_AMOUNT];
    srand(glfwGetTime()); // initialize random seed
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < ASTEROID_AMOUNT; i++) {
        glm::mat4 model;
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)ASTEROID_AMOUNT * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. scale: Scale between 0.01 and 0.02f
        float scale = (rand() % 20) / 2000.0f + 0.01;
        model = glm::scale(model, glm::vec3(scale));
        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, ASTEROID_AMOUNT * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    for (int i = 0; i < rock->meshes.size(); i++) {
        GLuint VAO = rock->meshes[i].VAO;
        glBindVertexArray(VAO);

        GLuint vec4Size = sizeof(glm::vec4);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)vec4Size);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
        glEnableVertexAttribArray(5);

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glBindVertexArray(0);
    }
}

static void draw(float time) {
    program->use();
    glm::mat4 view, projection;
    projection = glm::perspective(glm::radians(45.0f), float(width)/height, 0.1f, 1000.0f);
    view = camera.GetViewMatrix();
    program->setMat4("view", view);
    program->setMat4("projection", projection);

    program->use();
    program->setInt("material.texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock->textures_loaded[0].id);
    for (int i = 0; i < rock->meshes.size(); i++) {
        glBindVertexArray(rock->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, ASTEROID_AMOUNT);
    }

    counter->count();
    counter->render();
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
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL program", NULL, NULL); // window hint first, then create
    if (!window) {
        std::cout << "Failed to create an GLFW window. " << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setup();

    // Draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window, glfwGetTime());
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    delete program;
    glfwTerminate();
    return 0;
}
