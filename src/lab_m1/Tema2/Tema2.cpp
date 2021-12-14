#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;
using namespace Tema2ns;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Variables initialization.
    {
        rows = 15 + rand() % 10;
        columns = 15 + rand() % 10;
        maze = generateMap(rows, columns);
        objects_color = glm::vec3(0.6f, 0.6f, 0.6f);
        floor_color = glm::vec3(0.5, 0.5, 0.5);
        cameraPos = glm::vec3(0, 5, 0);
        edgeSize = 6;
        timeLeft = rows * columns / edgeSize;

        int playerRow = 5 + rand() % 5, playerColumn = 5 + rand() % 5;

        while (!maze[playerRow][playerColumn]) {
            playerRow = 5 + rand() % 5, playerColumn = 5 + rand() % 5;
        }

        player = Tema2ns::Player(edgeSize * (playerColumn + 1) + edgeSize / 2, 0, edgeSize * (playerRow + 1) + edgeSize / 2, 0, 0, 0);
        MoveCameraToPlayer();

        for (int i = 0; i < 10; ++i) {
            int enemyRow = 1 + rand() % (rows - 2), enemyColumn = 1 + rand() % (columns - 2);

            while (!maze[enemyRow][enemyColumn] || (enemyRow == playerRow && enemyColumn == playerColumn)) {
                enemyRow = 1 + rand() % (rows - 2), enemyColumn = 1 + rand() % (columns - 2);
            }

            enemies.push_back(Tema2ns::Enemy(edgeSize * (enemyColumn + 1), 0, edgeSize * (enemyRow + 1), 0, 0, 0, enemyRow, enemyColumn));
        }
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    fovp = 91;
    fovo = 5;

    projectionMatrix = glm::perspective(RADIANS(fovp), window->props.aspectRatio, 0.01f, 500.0f);
    proj_type = false;

    {
        Shader* shader = new Shader("ObstacleShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

// 0.5f este latura unui cub.
void Tema2::Update(float deltaTimeSeconds)
{
    projectionMatrix = glm::perspective(RADIANS(fovp), window->props.aspectRatio, 0.01f, 200.0f);

    {
        RenderMaze();

        if (gameOver()) {
            return;
        }

        timeLeft -= deltaTimeSeconds;

        killEnemies();
        RenderPlayer();
        MoveCameraToPlayer();
        RenderProjectiles();
        RenderEnemies(deltaTimeSeconds);
    }

    {
        if (!player.canShoot()) {
            player.timer -= deltaTimeSeconds;
        }
    }

    if (gameOver()) {
        cout << "Game Over\n";
    }

    if (!validGrid(((int)player.translateZ ) / edgeSize - 1, ((int)player.translateX) / edgeSize - 1)) {
        cout << "Ai castigat\n";
        player.health = 0;
    }

    if (gameOver()) {
        cout << "Game Over\n";
    }

    projectionMatrix = glm::ortho(-fovo, fovo, -fovo, fovo, -fovo, fovo);

    glm::mat4 modelMatrix = glm::mat4(1);
    glm::mat4 timeMat = glm::mat4(1);
    glm::mat4 lifeMat = glm::mat4(1);
    glm::mat4 modelMatrix2 = glm::mat4(1);
    
    if (fpMode) {
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition() - glm::vec3(0, -4.5, 0));
        timeMat = glm::translate(timeMat, camera->GetTargetPosition() - glm::vec3(0, -4.5, 0));
        modelMatrix *= transform3D::RotateOY(-player.rotateY);
        timeMat *= transform3D::RotateOY(-player.rotateY);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.5, 0.01));
        timeMat = glm::scale(timeMat, glm::vec3(2 * (timeLeft / (rows * columns / edgeSize)), 0.5, 0.01));

        modelMatrix2 = glm::translate(modelMatrix2, camera->GetTargetPosition() - glm::vec3(0, -3.8, 0));
        lifeMat = glm::translate(lifeMat, camera->GetTargetPosition() - glm::vec3(0, -3.8, 0));
        modelMatrix2 *= transform3D::RotateOY(-player.rotateY);
        lifeMat *= transform3D::RotateOY(-player.rotateY);
        modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(2, 0.5, 0.01));
        lifeMat = glm::scale(lifeMat, glm::vec3(2 * (1.0 * player.health / 4), 0.5, 0.01));
    }
    else {
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition() - glm::vec3(3.5, 0, 4));
        timeMat = glm::translate(timeMat, camera->GetTargetPosition() - glm::vec3(3.5, 0, 4));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1.5, 0.01));
        timeMat = glm::scale(timeMat, glm::vec3(2 * (timeLeft / (rows * columns / edgeSize)), 1.5, 0.01));

        modelMatrix2 = glm::translate(modelMatrix2, camera->GetTargetPosition() - glm::vec3(-3.5, 0, 4));
        lifeMat = glm::translate(lifeMat, camera->GetTargetPosition() - glm::vec3(-3.5, 0, 4));
        modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(2, 1.5, 0.01));
        lifeMat = glm::scale(lifeMat, glm::vec3(2 * (1.0 * player.health / 4), 1.5, 0.01));
    }

    RenderMesh(meshes["box"], shaders["ObstacleShader"], timeMat, glm::vec3(1, 0, 1));
    RenderMesh(meshes["box"], shaders["ObstacleShader"], modelMatrix, glm::vec3(1, 1, 1));
    RenderMesh(meshes["box"], shaders["ObstacleShader"], lifeMat, glm::vec3(1, 0, 1));
    RenderMesh(meshes["box"], shaders["ObstacleShader"], modelMatrix2, glm::vec3(1, 1, 1));
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(color));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (fpMode || gameOver()) {
        return;
    }


    if (window->KeyHold(GLFW_KEY_W)) {
        if (dirToMove) {
            player.rotateY = 0;
            dirToMove = 0;
        }

        if (!hitWall(player.translateX, player.translateZ - player.velocity * deltaTime, 0.35)) {
            player.translateZ -= player.velocity * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        if (dirToMove != 3) {
            player.rotateY = M_PI * 3 / 2;
            dirToMove = 3;
        }

        if (!hitWall(player.translateX - player.velocity * deltaTime, player.translateZ, 0.35)) {
            player.translateX -= player.velocity * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (dirToMove != 2) {
            player.rotateY = M_PI;
            dirToMove = 2;
        }

        if (!hitWall(player.translateX, player.translateZ + player.velocity * deltaTime, 0.35)) {
            player.translateZ += player.velocity * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        if (dirToMove != 1) {
            player.rotateY = M_PI / 2;
            dirToMove = 1;
        }

        if (!hitWall(player.translateX + player.velocity * deltaTime, player.translateZ, 0.35)) {
            player.translateX += player.velocity * deltaTime;
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (gameOver()) {
        return;
    }

    // Add key press event
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
    {
        fpMode = !fpMode;
        fovp = 151 - fovp;
        projectionMatrix = glm::perspective(RADIANS(fovp), window->props.aspectRatio, 0.01f, 200.0f);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (gameOver()) {
        return;
    }


    if (fpMode)
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        //camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        player.rotateY += sensivityOY * deltaX;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (gameOver()) {
        return;
    }

    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && player.canShoot() && fpMode) {
        player.shoot();

        // add new projectile if the click is pressed
        Projectile proj = Projectile(player.rotateY, 0, 0, 0, player.translateX, 1.4f, player.translateZ);
        projectiles.push_back(proj);
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

bool Tema2::checkNeighbours(vector<vector<int>>& maze, int i, int j, int row, int col) {
    for (int k = 0; k < 8; ++k) {
        int nextI = row + checkDx[k];
        int nextJ = col + checkDy[k];

        if (nextI >= 0 && nextJ >= 0 && nextI < rows && nextJ < columns &&
            maze[nextI][nextJ] && nextI != i && nextJ != j) {
            return false;
        }
    }

    return true;
}

void Tema2::BKTRHelper(int n, int m, vector<vector<int>>& maze, int i, int j) {
    if (i >= n || j >= m || i < 0 || j < 0) {
        return;
    }

    maze[i][j] = 1;

    if (i == 0 || j == 0 || i == n - 1 || j == m - 1) {
        return;
    }

    int start = rand() % 4;

    for (int k = 0; k < 4; ++k) {
        int nextI = i + dx[(start + k) % 4];
        int nextJ = j + dy[(start + k) % 4];

        if (maze[nextI][nextJ] == 0 && checkNeighbours(maze, i, j, nextI, nextJ)) {
            BKTRHelper(n, m, maze, nextI, nextJ);
        }
    }
}

vector<vector<int>> Tema2::generateMap(int n, int m) {
    vector<vector<int>> maze (n, vector<int>(m, 0));

    BKTRHelper(rows, columns, maze, 1 + rand() % (rows - 2), 1 + rand() % (columns - 2));

    return maze;
}

void Tema2::RenderObject(Mesh* mesh, Shader* shader, glm::vec3 deviation, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale, glm::vec3& color) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translate.x , translate.y, translate.z);
    modelMatrix *= transform3D::RotateOX(rotate.x);
    modelMatrix *= transform3D::RotateOY(rotate.y);
    modelMatrix *= transform3D::RotateOZ(rotate.z);
    modelMatrix *= transform3D::Scale(scale.x, scale.y, scale.z);

    RenderMesh(mesh, shader, modelMatrix, color);
}

void Tema2::RenderMaze() {
    // render floor.
    RenderObject(meshes["box"], shaders["ObstacleShader"], glm::vec3(0), glm::vec3(columns * edgeSize / 2, -0.5f, rows * edgeSize / 2), glm::vec3(0, 0, 0), glm::vec3((columns + 2) * edgeSize + 1, 1, (rows + 2) * edgeSize + 1), floor_color);

    // render walls;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (!maze[i][j]) {
                RenderObject(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0, 0), glm::vec3(edgeSize * (j + 1) + edgeSize / 2, 1, edgeSize * (i + 1) + edgeSize / 2), glm::vec3(0, 0, 0), glm::vec3(edgeSize, 2, edgeSize), objects_color);
            }
        }
    }
}

void Tema2::RenderPlayer() {
    for (auto it : player.bodyParts) {
        RenderObject(meshes["box"], shaders["ObstacleShader"], glm::vec3(it.centerX, it.centerY, it.centerZ), glm::vec3(player.translateX + it.centerX * cos(it.isNeg * player.rotateY), player.translateY + it.centerY, player.translateZ - it.centerZ * sin(it.isNeg * player.rotateY)), glm::vec3(player.rotateX, player.rotateY, player.rotateZ), glm::vec3(it.scaleX, it.scaleY, it.scaleZ), it.color);
    }
}

void Tema2::MoveCameraToPlayer() {
    if (!fpMode) {
        camera->Set(glm::vec3(player.translateX, cameraPos.y, player.translateZ + 1), glm::vec3(player.translateX, 1.7, player.translateZ), glm::vec3(0, 1, 0));
        return;
    }

    camera->Set(glm::vec3(player.translateX + 0.5 * sin(player.rotateY), 1.7, player.translateZ - 0.5 * cos(player.rotateY)), glm::vec3(player.translateX + 3 * sin(player.rotateY), 1.7, player.translateZ - 3 * cos(player.rotateY)), glm::vec3(0, 1, 0));
}

void Tema2::RenderProjectiles() {
    for (int i = 0; i < projectiles.size(); ++i) {
        auto it = projectiles[i];

        if (it.endLife() || hitWall(projectiles[i].centerX + projectiles[i].translateX, projectiles[i].centerZ + projectiles[i].translateZ, 0.08)) { // remove bullet if it's life is done.
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }

        projectiles[i].translateX += sin(it.angle) * it.velocity;
        projectiles[i].translateZ -= cos(it.angle) * it.velocity;
        projectiles[i].updateDistance(sqrt(sin(it.angle) * it.velocity * sin(it.angle) * it.velocity + cos(it.angle) * it.velocity * cos(it.angle) * it.velocity));

        RenderObject(meshes["box"], shaders["VertexNormal"], glm::vec3(0), glm::vec3(projectiles[i].centerX + projectiles[i].translateX, projectiles[i].centerY + projectiles[i].translateY, projectiles[i].centerZ + projectiles[i].translateZ), glm::vec3(0, M_PI - it.angle, 0), glm::vec3(0.1, 0.1, 0.15), glm::vec3(0));
    }
}

void Tema2::RenderEnemies(float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); ++i) {
        if (enemies[i].overLapPoint()) {
            enemies[i].toX = edgeSize * (enemies[i].column + 1) + rand() % ((int)edgeSize);
            enemies[i].toZ = edgeSize * (enemies[i].row + 1) + rand() % ((int)edgeSize);
        }

        if (circlesColide(player.translateX, player.translateZ, 0.35, enemies[i].translateX, enemies[i].translateZ, 0.35)) {
            player.health--;
            enemies.erase(enemies.begin() + i);
            i--;
            continue;
        }

        float y = enemies[i].toX - enemies[i].translateX;
        float x = enemies[i].toZ - enemies[i].translateZ;

        enemies[i].rotateY = glm::atan2(y, x);
        enemies[i].translateX += deltaTimeSeconds * enemies[i].velocity * sin(enemies[i].rotateY);
        enemies[i].translateZ += deltaTimeSeconds * enemies[i].velocity * cos(enemies[i].rotateY);

        for (auto it : enemies[i].bodyParts) {
            RenderObject(meshes["box"], shaders["ObstacleShader"], glm::vec3(it.centerX, it.centerY, it.centerZ), glm::vec3(enemies[i].translateX + it.centerX * cos(it.isNeg * enemies[i].rotateY), enemies[i].translateY + it.centerY, enemies[i].translateZ - it.centerZ * sin(it.isNeg * enemies[i].rotateY)), glm::vec3(enemies[i].rotateX, enemies[i].rotateY, enemies[i].rotateZ), glm::vec3(it.scaleX, it.scaleY, it.scaleZ), it.color);
        }
    }
}

bool Tema2::validGrid(int r, int c) {
    if (r >= rows || r < 0 || c >= columns || c < 0) {
        return false;
    }

    return true;
}


bool Tema2::hitWall(float posX, float posZ, float radius) {
    if (validGrid(((int)(posZ - radius)) / edgeSize - 1, ((int)posX) / edgeSize - 1) && !maze[((int)(posZ - radius)) / edgeSize - 1][((int)posX) / edgeSize - 1]) {
        return true;
    }

    if  (validGrid(((int)posZ) / edgeSize - 1, ((int)(posX - radius)) / edgeSize - 1) && !maze[((int)posZ) / edgeSize - 1][((int)(posX - radius)) / edgeSize - 1]) {
        return true;
    }

    if (validGrid(((int)(posZ + radius)) / edgeSize - 1, ((int)posX) / edgeSize - 1) && !maze[((int)(posZ + radius)) / edgeSize - 1][((int)posX) / edgeSize - 1]) {
        return true;
    }

    if (validGrid(((int)posZ) / edgeSize - 1, ((int)(posX + radius)) / edgeSize - 1) && !maze[((int)posZ) / edgeSize - 1][((int)(posX + radius)) / edgeSize - 1]) {
        return true;
    }

    return false;
}

bool Tema2::circlesColide(float x1, float y1, float radius1, float x2, float y2, float radius2) {
    float deltaX = x1 - x2, deltaY = y1 - y2;
    float distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

    if (distance < radius1 + radius2) {
        return true;
    }

    return false;
}


void Tema2::killEnemies() {
    for (int i = 0; i < projectiles.size(); ++i) {
        for (int j = 0; j < enemies.size(); ++j) {
            if (circlesColide(projectiles[i].centerX + projectiles[i].translateX, projectiles[i].centerZ + projectiles[i].translateZ, 0.08, enemies[j].translateX, enemies[j].translateZ, 0.35)) {
                projectiles.erase(projectiles.begin() + i);
                i--;
                enemies.erase(enemies.begin() + j);
                break;
            }
        }
    }
}

bool Tema2::gameOver() {
    return (timeLeft <= 0) || (player.health <= 0);
}
