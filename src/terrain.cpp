#include "utilities.h"

class Terrain {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        bool calculated = false;
    };

public:
    Terrain(glm::vec2 size, GLfloat dimension = DEFAULT_FRACTAL_DIMENSION, GLuint recursion = DEFAULT_NUM_RECURSION,
            GLfloat factor = DEFAULT_HEIGHT_FACTOR)
        :_size(size), _dimension(dimension), _recursion(recursion), _factor(factor) {
        initialize();
        generate();
        calculateNormal();
        triangulate();
        setupData();
    }

    void draw(Shader program) {
        program.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    ~Terrain() {
        delete []_vertices;
    }

private:
    constexpr static GLfloat DEFAULT_FRACTAL_DIMENSION = 2.45f;
    constexpr static GLfloat DEFAULT_HEIGHT_FACTOR = 0.3f;
    constexpr static GLuint DEFAULT_NUM_RECURSION = 9;
    Vertex *_vertices;
    vector<GLuint> _indices;
    glm::vec2 _size;
    GLfloat _dimension;
    GLuint _num;
    GLuint _recursion;
    GLfloat _factor;
    GLuint VAO, VBO, EBO;

    void initialize() {
        _num = int(pow(2, _recursion)) + 1; // allocate memory
        _vertices = new Vertex[_num * _num];

        glm::vec3 origin(-_size[0] / 2.0f, 0.0f, -_size[1] / 2.0f); // set plane
        float deltaX = _size[0] / (_num - 1);
        float deltaZ = _size[0] / (_num - 1);
        for (int i = 0; i < _num; i++)
            for (int j = 0; j < _num; j++) {
                glm::vec3 offset(i * deltaX, 0, j * deltaZ);
                getVertex(i, j)->position = origin + offset;
            }
    }

    void generate() {
        srand(time(NULL));
        generateRecursively(_recursion, glm::ivec2(0), glm::ivec2(_num - 1));
    }

    void generateRecursively(int recursion, glm::ivec2 up_left, glm::ivec2 down_right) { // use diamond-square algorithm
        const static glm::vec3 UP_UNIT(0, 1, 0);
        glm::ivec2 center = (up_left + down_right) / 2;
        glm::ivec2 up_right(down_right[0], up_left[1]), down_left(up_left[0], down_right[1]);
        glm::ivec2 up_middle(center[0], up_left[1]), middle_left(up_left[0], center[1]),
                down_middle(center[0], down_right[1]), middle_right(down_right[0], center[1]);
        float length = getLength(up_left, down_right); // nine points in total

        displaceMidpoint(center, planeAverage(up_left, up_right, down_left, down_right), length);
        displaceMidpoint(up_middle, lineAverage(up_left, up_right), length / 2);
        displaceMidpoint(middle_left, lineAverage(up_left, down_left), length / 2);
        displaceMidpoint(down_middle, lineAverage(down_left, down_right), length / 2);
        displaceMidpoint(middle_right, lineAverage(up_right, down_right), length / 2);

        if (recursion > 0) { // begin recursion
            generateRecursively(recursion-1, up_left, center);
            generateRecursively(recursion-1, up_middle, middle_right);
            generateRecursively(recursion-1, middle_left, down_middle);
            generateRecursively(recursion-1, center, down_right);
        }
    }

    void displaceMidpoint(glm::ivec2 coord, glm::vec3 average, float length) {
        const static glm::vec3 UP_UNIT(0, 1, 0);
        if (!getVertex(coord)->calculated) {
            getVertex(coord)->position = average + UP_UNIT * randomHeight(length);
            getVertex(coord)->calculated = true;
        }
    }

    inline glm::vec3 planeAverage(glm::ivec2 p1, glm::ivec2 p2,
                           glm::ivec2 p3, glm::ivec2 p4) {
        return (getVertex(p1)->position + getVertex(p2)->position
                + getVertex(p3)->position + getVertex(p4)->position) / 4.0f;
    }

    inline glm::vec3 lineAverage(glm::ivec2 first, glm::ivec2 second) {
        return (getVertex(first)->position + getVertex(second)->position) / 2.0f;
    }

    inline Vertex * getVertex(glm::ivec2 coord) { // use glm::vec2 as coordinate
        return getVertex(coord.x, coord.y);
    }

    inline Vertex * getVertex(int x, int y) {
        return &_vertices[getIndice(x, y)];
    }

    inline int getIndice(glm::ivec2 coord) {
        return getIndice(coord.x, coord.y);
    }

    inline int getIndice(int x, int y) {
        return x * _num + y;
    }

    inline float randomHeight(float length) {
        return nextGaussian(0, powf(length, 2 * (3 - _dimension)) * _factor);
        //return (float(rand()) / RAND_MAX - 0.5) * _factor * length;
    }

    float nextGaussian(float mean, float variance) {
        static float V1, V2, S;
        static int phase = 0;
        float X;

        if (phase == 0) {
            do {
                float U1 = (float)rand() / RAND_MAX;
                float U2 = (float)rand() / RAND_MAX;
                V1 = 2 * U1 - 1;
                V2 = 2 * U2 - 1;
                S = V1 * V1 + V2 * V2;
            } while (S >= 1 || S == 0);
            X = V1 * sqrt(-2 * log(S) / S);
        } else
            X = V2 * sqrt(-2 * log(S) / S);

        phase = 1 - phase;

        return X * variance + mean;
    }

    inline float getLength(glm::ivec2 up_left, glm::vec2 down_right) {
        return (getVertex(down_right)->position - getVertex(up_left)->position)[0];
    }

    void outputVertex(Vertex &vert) {
        cout << "Position: [" << vert.position[0] << ", " << vert.position[1] << ", "
             << vert.position[2] << "] " << endl;
        cout << "Normal: [" << vert.normal[0] << ", " << vert.normal[1] << ", "
             << vert.normal[2] << "]" << endl;
    }

    void calculateNormal() {
        glm::ivec2 LEFT(-1, 0), UP(0, -1), RIGHT(1, 0), DOWN(0, 1);
        pair<glm::ivec2, glm::ivec2> offsets[4];
        offsets[0] = pair<glm::ivec2, glm::ivec2>(LEFT, UP);
        offsets[1] = pair<glm::ivec2, glm::ivec2>(UP, RIGHT);
        offsets[2] = pair<glm::ivec2, glm::ivec2>(RIGHT, DOWN);
        offsets[3] = pair<glm::ivec2, glm::ivec2>(DOWN, LEFT);

        for (int i = 0; i < _num; i++) {
            for (int j = 0; j < _num; j++) {
                glm::ivec2 current(i, j);
                vector<glm::vec3> normals;
                for (int t = 0; t < 4; t++) {
                    glm::ivec2 coord1 = current + offsets[t].first;
                    if (!isValidIndex(coord1)) continue; // not valid index, check next pair
                    glm::ivec2 coord2 = current + offsets[t].second;
                    if (!isValidIndex(coord2)) continue;
                    glm::vec3 vector1 = getVertex(current)->position - getVertex(coord1)->position;
                    glm::vec3 vector2 = getVertex(coord2)->position - getVertex(current)->position;
                    glm::vec3 norm = glm::cross(vector1, vector2);
                    normals.push_back(norm);
                }
                glm::vec3 average = std::accumulate(normals.begin(), normals.end(), glm::vec3(0));
                average = glm::normalize(average);
                getVertex(current)->normal = average;
            }
        }
    }

    inline bool isValidIndex(glm::ivec2 coord) {
        return isValidIndex(coord.x, coord.y);
    }

    inline bool isValidIndex(int x, int y) {
        return (x >= 0) && (x < _num) && (y >= 0) && (y < _num);
    }

    void triangulate() {
        const static glm::ivec2 TRIANGLE_INDICES[6] = {
            glm::ivec2(1, 0), glm::ivec2(0, 0), glm::ivec2(1, 1),
            glm::ivec2(1, 1), glm::ivec2(0, 0), glm::ivec2(0, 1)
        };
        for (int i = 0; i < _num - 1; i++) {
            for (int j = 0; j < _num - 1; j++) {
                glm::ivec2 current(i, j);
                for (int t = 0; t < 6; t++) {
                    glm::ivec2 indiceCoords = current + TRIANGLE_INDICES[t];
                    _indices.push_back(getIndice(indiceCoords));
                }
            }
        }
        // for (GLuint i = 0; i < _indices.size(); i++) cout <<_indices[i] << ' ';
    }

    void setupData() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, _num * _num * sizeof(Vertex), _vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

};

static const GLsizei width = 1024, height = 576;
static Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

static Shader *program;
static Terrain *land;
static TextRenderer *text;
static FrameCounter *counter;

static void setup() {
    program = new Shader("shaders/terrain/terrain.vs.glsl", "shaders/terrain/terrain.fs.glsl");
    land = new Terrain(glm::vec2(3.0f));
    text = new TextRenderer("resources/IBMPlexMono-Regular.ttf", glm::ivec2(width, height));
    counter = new FrameCounter(text);
}

static void draw(float time) {
    program->use();
    glm::vec3 pointLightPos(3.0f, 4.0f, 3.0f);
    program->setFloat("material.shininess", 32.0f); // lighting
    program->setVec3("pointLight.position", pointLightPos);
    program->setVec3("pointLight.ambient", glm::vec3(0.1f));
    program->setVec3("pointLight.diffuse", glm::vec3(0.8f));
    program->setVec3("pointLight.specular", glm::vec3(0.7f));
    program->setFloat("pointLight.constant", 1.0f);
    program->setFloat("pointLight.linear", 0.09f);
    program->setFloat("pointLight.quadratic", 0.032f);
    program->setVec3("dirLight.direction", 2.0f, -1.0f, -1.0f);
    program->setVec3("dirLight.ambient", glm::vec3(0.05f));
    program->setVec3("dirLight.diffuse", glm::vec3(0.4f));
    program->setVec3("dirLight.specular", glm::vec3(0.2f));
    program->setBool("useBlinn", true);

    glm::mat4 model, view, projection; // view and projection matrices
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);
    glm::mat3 normalMat(glm::transpose(glm::inverse(view * model)));
    program->setMat4("model", model);
    program->setMat4("view", view);
    program->setMat4("projection", projection);
    program->setMat3("normalMat", normalMat);
    land->draw(*program);

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

int main() {
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
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
    delete program;
    delete counter;
    delete text;
    glfwTerminate();
    return 0;
}
