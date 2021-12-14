#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/Tema2/Player.h"
#include "lab_m1/Tema2/components/projectile.h"
#include "lab_m1/Tema2/components/enemy.h"
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;
using namespace Tema2ns;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        std::vector<vector<int>> generateMap(int n, int m);
        void BKTRHelper(int n, int, std::vector<vector<int>>& maze, int i, int j);
        bool checkNeighbours(std::vector<vector<int>>& maze, int i, int j, int row, int col);
        void RenderObject(Mesh* mesh, Shader* shader, glm::vec3 deviation, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale, glm::vec3& color);
        void RenderMaze();
        void RenderPlayer();
        void MoveCameraToPlayer();
        void RenderProjectiles();
        void RenderEnemies(float deltaTimeSeconds);
        bool hitWall(float posX, float posZ, float radius);
        void killEnemies();
        bool circlesColide(float x1, float y1, float radius1, float x2, float y2, float radius2);
        bool validGrid(int r, int c);
        bool gameOver();

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float fovp;
        float fovo;
        bool proj_type;
        int rows, columns;
        std::vector<vector<int>> maze;
        short dx[4] = { -1, 0, 1, 0 };
        short checkDx[8] = { -1, 0, 1, 0, -1, -1, 1, 1 };
        short dy[4] = { 0, -1, 0, 1 };
        short checkDy[8] = { 0, -1, 0, 1, -1, 1, -1, 1 };
        glm::vec3 objects_color, floor_color;
        Tema2ns::Player player;
        glm::vec3 cameraPos;
        int dirToMove = 0;
        float edgeSize = 0;
        bool fpMode = false;
        std::vector<Projectile> projectiles;
        std::vector<Tema2ns::Enemy> enemies;
        float timeLeft;

    };
}   // namespace m1
