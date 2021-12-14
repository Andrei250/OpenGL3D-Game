#include "lab_m1/lab1/lab1.h"
// ON PRESSING C YOU CHANGE OBJECT.
#include <vector>
#include <iostream>
#define GRAVITATIONAL_ACC 9.8
#define INITIAL_SPEED 10

using namespace std;
using namespace m1;


struct element {
    string name;
    float x, y, z;
};

float rValue;
int pos = 0;
vector<element> options;
bool isBlocked;
float velocity;
float initialY;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!

}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    rValue = 0;
    isBlocked = false;

    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;

        Mesh* meshPlane = new Mesh("sphere");
        meshPlane->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[meshPlane->GetMeshID()] = meshPlane;

        Mesh* meshQuad = new Mesh("teapot");
        meshQuad->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[meshQuad->GetMeshID()] = meshQuad;
        options.push_back({ "box", 0, 0.5f, 0 });
        options.push_back({ "sphere", 0, 0.5f, 0 });
        options.push_back({ "teapot", 0, 0.5f, 0 });
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(rValue, 0, 0, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    //RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
   // RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    //RenderMesh(meshes["teapot"], glm::vec3(-1, 0, 1));
   // RenderMesh(meshes["sphere"], glm::vec3(-1, 0.5f, 2));

    if (pos < options.size()) {
        RenderMesh(meshes[options[pos].name], glm::vec3(options[pos].x, options[pos].y, options[pos].z));

        if (isBlocked) {
            velocity = velocity - GRAVITATIONAL_ACC * deltaTimeSeconds;
            options[pos].y = initialY + 1.0f * (INITIAL_SPEED * INITIAL_SPEED - velocity * velocity) / (2 * GRAVITATIONAL_ACC);

            if (options[pos].y < initialY) {
                options[pos].y = initialY;
            }

            if (options[pos].y == initialY) {
                isBlocked = false;
            }
        }
    }

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    if (!isBlocked && pos < options.size()) {
        if (window->KeyHold(GLFW_KEY_W)) {
            options[pos].z = options[pos].z - 2 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_A)) {
            options[pos].x = options[pos].x - 2 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_S)) {
            options[pos].z = options[pos].z + 2 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_D)) {
            options[pos].x = options[pos].x + 2 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_E)) {
            options[pos].y = options[pos].y + 2 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_Q)) {
            options[pos].y = options[pos].y - 2 * deltaTime;
        }
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    if (isBlocked) {
        return;
    }

    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        rValue = 0.2f;
    }

    if (key == GLFW_KEY_T) { // toggle image color
        rValue = abs(rValue - 0.2f);
    }

    if (key == GLFW_KEY_C) { // toggle element. when you press C you change object
        pos = this->MovePos(0, options.size() - 1, ++pos);
    }

    if (pos < options.size() && key == GLFW_KEY_SPACE) {
        isBlocked = true;
        velocity = INITIAL_SPEED;
        initialY = options[pos].y;
    }

    

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}

int Lab1::MovePos(int start, int stop, int value)
{
    if (value > stop) {
        return start;
    }

    if (value < start) {
        return stop;
    }

    return value;
}
