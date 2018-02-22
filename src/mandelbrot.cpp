// mandelbrot.cpp

#include "utilities.h"
#include "data.h"

static const GLsizei width = 1024, height = 576;

static Shader *mandelbrot, *julia;
static TextRenderer *text;
static FrameCounter *counter;
static View2D view(glm::dvec2(-0.5, 0), 0.8, double(width)/height);
static GLuint mandVAO, mandVBO, juliaVAO, juliaVBO;

static void setup() {
    mandelbrot = new Shader("shaders/mandelbrot/mandelbrot.vs.glsl", "shaders/mandelbrot/mandelbrot.fs.glsl");
    julia = new Shader("shaders/mandelbrot/mandelbrot.vs.glsl", "shaders/mandelbrot/julia.fs.glsl");
    text = new TextRenderer("resources/IBMPlexMono-Regular.ttf", glm::ivec2(width, height));
    counter = new FrameCounter(text);

    glGenVertexArrays(1, &mandVAO); // main mandelbrot view
    glGenBuffers(1, &mandVBO);
    glBindVertexArray(mandVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mandVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTICES), SCREEN_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &juliaVAO);
    glGenBuffers(1, &juliaVBO);
    glBindVertexArray(juliaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, juliaVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UP_RIGHT_SCREEN_VERTICES), UP_RIGHT_SCREEN_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void draw(float time) {
    mandelbrot->use(); // draw mandelbrot set
    glm::dmat4 transform, inverse;
    transform = view.getViewMatrix();
    inverse = glm::inverse(transform);
    mandelbrot->setDMat4("invMat", inverse);
    glBindVertexArray(mandVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    julia->use();
    transform = glm::mat4();
    transform = glm::translate(transform, glm::dvec3(0.75, 0.75, 0.0));
    transform = glm::scale(transform, glm::dvec3(0.25 * double(height)/width, 0.25, 1));
    inverse = glm::inverse(transform);
    julia->setDMat4("invMat", inverse);
    julia->setDVec2("origin", view.getOrigin());
    glBindVertexArray(juliaVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    counter->count();
    counter->render();
    text->render(("Current Scale: " + to_string(view.getScale()) ).c_str(),
                 glm::vec2(20.0f, 40.0f), 14.0/48, glm::vec3(1.0));
}

static void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        view.move(View2D::UP);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        view.move(View2D::DOWN);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        view.move(View2D::LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        view.move(View2D::RIGHT);

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        view.zoom(View2D::IN);
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        view.zoom(View2D::OUT);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Mandelbrot Set", NULL, NULL); // window hint first
    if (!window) {
        std::cout << "Failed to create an GLFW window. " << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setup();

    // Draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        draw(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    delete mandelbrot;
    glfwTerminate();
    return 0;
}
