#include<iostream>
#include<memory>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<Shader.h>
#include<Cube.h>
#include<DotObject.h>
#include<FrameBuffer.h>
#include<Minimap.h>

unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 1200;
std::shared_ptr<DotObject> cameraObject = nullptr;
std::shared_ptr<DotObject> lightObject = nullptr;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void processInput(GLFWwindow* window, DotObject& camera, DotObject& light, const float& deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveDown(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        light.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        light.moveRight(deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (cameraObject != nullptr)
        cameraObject->zoom(-yoffset);
}

void renderScene(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const Cube& cubeObject, const bool hasTexture, const unsigned int texture, const bool cullFace = false)
{

    const glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, -1.3f),
        glm::vec3(-4.0f, 1.0f, -3.0f),
        glm::vec3(4.0f, 1.0f, 3.0f)
    };

    const float cubeRotations[] = { 0, 45, 20, 20 };

    const glm::vec3 platformPosition(0, 0, 0);

    shader.activate();

    shader.setMat4(view, "view");
    shader.setMat4(projection, "projection");
    shader.setMat4(lightObject->getView(), "lightView");
    shader.setMat4(lightObject->getOrtho(), "lightProjection");
    shader.setVec3(cameraObject->getPosition(), "viewPos");
    shader.setVec3(lightObject->getPosition(), "lightPos");

    if (hasTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    cubeObject.activate();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, platformPosition);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(15.0f, 15.0f, 0.1f));
    glm::mat3 modelNormal = glm::transpose(glm::inverse(model));
    shader.setMat4(model, "model");
    shader.setMat3(modelNormal, "modelNormal");
    shader.setBool(true, "isPlatform");
    if(cullFace)
        glCullFace(GL_FRONT);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    if(cullFace)
        glCullFace(GL_BACK);

    shader.setBool(false, "isPlatform");
    for (unsigned int i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians(cubeRotations[i]), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat3 modelNormal = glm::transpose(glm::inverse(model));

        shader.setMat4(model, "model");
        shader.setMat3(modelNormal, "modelNormal");

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}

void renderScene(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const Cube& cubeObject, const bool cullFace = false)
{
    renderScene(shader, view, projection, cubeObject, false, 0, cullFace);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

     
    std::shared_ptr<Shader> shaderProgram = std::make_shared<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
    shaderProgram->activate();
    shaderProgram->setInt(0, "shadowMap");
    std::shared_ptr<Shader> minimapGenShader = std::make_shared<Shader>("shaders/minimapGenVertex.glsl", "shaders/minimapGenFragment.glsl");
    std::shared_ptr<Shader> minimapShader = std::make_shared<Shader>("shaders/minimapVertex.glsl", "shaders/minimapFragment.glsl");
    minimapShader->activate();
    minimapShader->setInt(0, "screenTexture");
    std::shared_ptr<Shader> shadowMapGenShader = std::make_shared<Shader>("shaders/shadowMapVertex.glsl", "shaders/shadowMapFragment.glsl");
    std::shared_ptr<FrameBuffer> minimapFrameBuffer = std::make_shared<FrameBuffer>(SCR_WIDTH, SCR_HEIGHT);
    std::shared_ptr<FrameBuffer> shadowMapFrameBuffer = std::make_shared<FrameBuffer>(SCR_WIDTH, SCR_HEIGHT);
    cameraObject = std::make_shared<DotObject>(0, -45, 15);
    lightObject = std::make_shared<DotObject>(135, -45, 15);
    std::shared_ptr<Cube> cubeObject = std::make_shared<Cube>();
    std::shared_ptr<Minimap> minimapObject = std::make_shared<Minimap>();



    float deltaTime = 0;
    float lastFrame = 0;

    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, *cameraObject, *lightObject, deltaTime);

        //generate minimap texture
        minimapFrameBuffer->useBuffer();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = lightObject->getView();
        glm::mat4 projection = lightObject->getProjection(SCR_WIDTH, SCR_HEIGHT);
        renderScene(*minimapGenShader, view, projection, *cubeObject);

        //generate shadow map
        shadowMapFrameBuffer->useBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        view = lightObject->getView();
        projection = lightObject->getOrtho();
        renderScene(*shadowMapGenShader, view, projection, *cubeObject, true);

        //render main scene
        FrameBuffer::UseDefaultBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        view = cameraObject->getView();
        projection = cameraObject->getProjection(SCR_WIDTH, SCR_HEIGHT);
        unsigned int texture = shadowMapFrameBuffer->getDepthTexture();
        renderScene(*shaderProgram, view, projection, *cubeObject, true, texture);

        //render minimap
        glDisable(GL_DEPTH_TEST);
        minimapShader->activate();
        minimapObject->activate();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, minimapFrameBuffer->getRgbTexture());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}