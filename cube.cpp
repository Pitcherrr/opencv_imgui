
#include "raylib.h"
#include "raymath.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    float size = 2.0f;

    Mesh cubeMesh = GenMeshCube(size,size,size);

    Model cubeModel = LoadModelFromMesh(cubeMesh);

    SetTargetFPS(60);

    float rotationSpeed = 0.2f;
    Vector2 mousePositionPrev = GetMousePosition();

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();
        float deltaX = (mousePosition.x - mousePositionPrev.x) * rotationSpeed;
        float deltaY = (mousePosition.y - mousePositionPrev.y) * rotationSpeed;

        mousePositionPrev = mousePosition;

        BeginDrawing();

        ClearBackground(DARKGRAY);

        BeginMode3D(camera);

        // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        // DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, BLACK);

        float rot_scale = 10.0f;

        // DrawGrid(10, 1.0f);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            cubeModel.transform = MatrixMultiply(MatrixRotateY(rot_scale * deltaX * DEG2RAD), cubeModel.transform);
            cubeModel.transform = MatrixMultiply(MatrixRotateX(rot_scale * deltaY * DEG2RAD), cubeModel.transform);

            DrawModel(cubeModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, RED);
            DrawModelWires(cubeModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);   
        }

        else {DrawModel(cubeModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, RED); DrawModelWires(cubeModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);  }

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}