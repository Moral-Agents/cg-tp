#include "glutil.h"
#include "figures.h"
#include "camera.h"
#include <vector>

const unsigned int FSIZE = sizeof(float);
const unsigned int ISIZE = sizeof(int);
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float ASPECT = (float)SCR_WIDTH / (float)SCR_HEIGHT;

using namespace std;
using namespace glm;

bool firstMouse = true;

float lastx = SCR_WIDTH / 2.0f;
float lasty = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int n = 128;

Cam camara(0.0, 8.5);

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camara.processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camara.processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camara.processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camara.processKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastx = xpos;
        lasty = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;
    
    lastx = xpos;
    lasty = ypos;
    
    camara.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xpos, double ypos) {
    camara.processScroll(ypos);
}

vector<float> perlinNoise1D(int nOctaves, float fScalingBias) {
    vector<float> pNoise1D(n);

    vector<float> noise1D(n);
    for (int i = 0; i < n; i++) noise1D[i] = (float)rand() / (float)RAND_MAX;

    

    for (int i = 0; i < n; i++)
    {
        float iNoise = 0.0f;
        float fScale = 1.0f;
        float fScaleAcc = 0.0f;
        for (int o = 0; o < nOctaves; o++)
        {
            int nPitch = n >> o;
            int nSample1 = (i / nPitch) * nPitch;
            int nSample2 = (nSample1 + nPitch) % n;

            float fBlend = (float)(i - nSample1) / (float)nPitch;
            float fInter = (1.0f - fBlend) * noise1D[nSample1] + fBlend * noise1D[nSample2];

            iNoise += fInter * fScale;
            fScaleAcc += fScale;
            fScale /= fScalingBias;
        }
        pNoise1D[i] = iNoise / fScaleAcc;
    }
    return pNoise1D;
}

int main() {
    GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Trabajo Parcial");
    Shader* shader = new Shader("/Users/wilmartarazona/Documents/OpenGL", "/Users/wilmartarazona/Pictures/Texturas");
    Cube* cube = new Cube();
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    vector<float> pNoise1D = perlinNoise1D(7, 3.20);
    vector<glm::vec3> positions(n*n);
    for (u32 i = 0; i < n; ++i) {
        for (u32 j = 0; j < n; ++j) {
            f32 x = i - n / 2.0f + 0.01;
            f32 z = j - n / 2.0f + 0.01;
            f32 y = int(pNoise1D[i] * 120) - 30;
            positions[i*n + j] = glm::vec3(x, y, z);
        }
    }
    
    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    
    glBufferData(GL_ARRAY_BUFFER, cube->getVSize() * FSIZE, cube->getVertices(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->getISize() * ISIZE, cube->getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(3*FSIZE));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(6 * FSIZE));
    glEnableVertexAttribArray(2);
    
    unsigned int textura = shader->loadTexture("iron.jpg");
    unsigned int textura1 = shader->loadTexture("pasto.jpg");
    unsigned int textura2 = shader->loadTexture("grietas.jpg");
    
    glEnable(GL_DEPTH_TEST);
    
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        glClearColor(.1f, .2f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader->useProgram();
        
        mat4 projection = perspective(camara.getZoom(), ASPECT, 0.1f, 100.0f);
        shader->setMat4("proj", projection);
        
        mat4 view = mat4(1.0f);
        view = camara.getViewM4();
        shader->setMat4("view", view);
        
        glBindVertexArray(vao);
        
        for (unsigned int i = 0; i < positions.size(); ++i) {
            if (positions[i].y > -5) {
                glBindTexture(GL_TEXTURE_2D, textura);
            }
            else if (positions[i].y > -10) {
                glBindTexture(GL_TEXTURE_2D, textura1);
            }
            else {
                glBindTexture(GL_TEXTURE_2D, textura2);
            }
            mat4 model = mat4(1.0f);
            model = translate(model, positions[i]);
            shader->setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    delete shader;
    delete cube;
    
    glfwTerminate();
    return 0;
}
