#include "raylib.h"

Camera camera = { 0 };
Vector3 cubePosition = { 0 };

// Update and draw one frame
static void UpdateDrawFrame(void);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Splander");

    camera.position = (Vector3){ 3.0f, 3.0f, 2.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while(!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    UpdateCamera(&camera, CAMERA_ORBITAL);
    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

            DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("Welcome to raylib basic sample", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

    EndDrawing();
}
