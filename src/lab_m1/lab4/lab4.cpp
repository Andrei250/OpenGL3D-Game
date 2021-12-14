#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    translateSunY = 0;
    rotateSunY = 0;
    direction = 1;

    radiusEarth = 2;
    rotationYEarth = 0;
    scaleEarth = 0.4f;

    radiusMoon = 0.8f;
    rotationYMoon = 0;
    scaleMoon = 0.2f;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f + translateX, 0.5f + translateY, -1.5f + translateZ);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, translateSunY, 0);
    modelMatrix *= transform3D::RotateOY(rotateSunY);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    float transX = radiusEarth * cos(rotateSunY);
    float transZ = radiusEarth * sin(rotateSunY);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(transX, translateSunY, transZ);
    modelMatrix *= transform3D::Scale(scaleEarth, scaleEarth, scaleEarth);
    modelMatrix *= transform3D::RotateOY(rotationYEarth);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    float transXMoon = transX + radiusMoon * cos(rotationYEarth);
    float transZMoon = transZ + radiusMoon * sin(rotationYEarth);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(transXMoon, translateSunY, transZMoon);
    modelMatrix *= transform3D::Scale(scaleMoon, scaleMoon, scaleMoon);
    modelMatrix *= transform3D::RotateOY(rotationYMoon);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    translateSunY += direction * deltaTimeSeconds / 2;
    rotateSunY += deltaTimeSeconds / 4;
    rotationYEarth += deltaTimeSeconds;
    rotationYMoon += 3 * deltaTimeSeconds;

    if (rotateSunY > 2 * 3.14) {
        rotateSunY -= 2 * 3.14;
    }

    if (rotationYEarth > 2 * 3.14) {
        rotationYEarth -= 2 * 3.14;
    }

    if (rotationYMoon > 2 * 3.14) {
        rotationYMoon -= 2 * 3.14;
    }

    if (translateSunY <= 0) {
        direction = 1;
    }
    else if (translateSunY >= 3) {
        direction = -1;
    }

}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W)) {
        translateZ -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translateZ += 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_R)) {
        translateY += 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_F)) {
        translateY -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime;
        scaleY += deltaTime;
        scaleZ += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime;
        scaleY -= deltaTime;
        scaleZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOY -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOY += 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOX -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOX += 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ += 2 * deltaTime;
    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
