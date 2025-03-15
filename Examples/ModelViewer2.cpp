#include <cstdio>
#include <cstring>
#include "raylib.h"
#include "rlgl.h"

// Postprocessing shader based model viewer

const int SCREEN_W = 1920;
const int SCREEN_H = 1080;

using namespace std;

int main(int argc, char* argv[]) {
    // Get model path from command line.
    if (argc < 2) {
        printf("Usage: %s <model_path>\n", argv[0]);
        return 1;
    }

    InitWindow(SCREEN_W, SCREEN_H, "Model viewer");
    SetTargetFPS(60);
    rlEnableBackfaceCulling();

    // Load glTF model from file.
    Model model = LoadModel(argv[1]);

    int animsCount = 0;
    ModelAnimation* anims = LoadModelAnimations(argv[1], &animsCount);
    printf("Loaded %d animations.\n", animsCount);

    // Set up shaders.

    // Regular visual view, does shading and lighting.
    Shader color = LoadShader("Examples/model.vs", "Examples/model.fs");

    color.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(color, "matModel");
    color.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(color, "viewPos");

    RenderTexture2D target = LoadRenderTexture(SCREEN_W, SCREEN_H);

    // Set up directional light
    int lightDirLoc = GetShaderLocation(color, "lightDir");
    Vector3 lightDir = {2.0f, -4.0f, -2.0f};
    SetShaderValue(color, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

    Vector3 ambientLight = {0.2f, 0.2f, 0.2f};
    int ambientLoc = GetShaderLocation(color, "ambient");
    SetShaderValue(color, ambientLoc, &ambientLight, SHADER_UNIFORM_VEC3);

    // Depth shader, bakes scene depth map into a render texture.
    Shader depth = LoadShader("Examples/depth.vs", "Examples/depth.fs");

    depth.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(depth, "matModel");
    depth.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(depth, "viewPos");

    RenderTexture2D depthTarget = LoadRenderTexture(SCREEN_W, SCREEN_H);

    // Set up prostprocessing shader.
    Shader postProc = LoadShader(0, "Examples/postproc.fs");

    // Camera setup.
    Camera camera;
    camera.position = Vector3{6.0f, 6.0f, 6.0f};
    camera.target = Vector3{0.0f, 2.0f, 0.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int currentAnim = 0;
    int currentFrame = 0;

    if (currentAnim < animsCount) {
        UpdateModelAnimation(model, anims[currentAnim], currentFrame);
        currentFrame++;
        if (currentFrame >= anims[0].frameCount)
            currentFrame = 0;
    }

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_SPACE)) {
            /*
            UpdateCamera(&camera, CAMERA_ORBITAL);
            UpdateCamera(&camera, CAMERA_ORBITAL);
            UpdateCamera(&camera, CAMERA_ORBITAL);
            UpdateCamera(&camera, CAMERA_ORBITAL);
            */

            if (currentAnim < animsCount) {
                UpdateModelAnimation(model, anims[currentAnim], currentFrame);
                currentFrame++;
                if (currentFrame >= anims[0].frameCount)
                    currentFrame = 0;
            }
        }

        SetShaderValue(color, color.locs[SHADER_LOC_VECTOR_VIEW],
                       &camera.position.x, SHADER_UNIFORM_VEC3);

        // Draw regular
        BeginTextureMode(target);
        {
            for (int i = 0; i < model.materialCount; i++) {
                model.materials[i].shader = color;
            }
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                DrawModel(model, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
                DrawGrid(10, 1.0f);
            }
            EndMode3D();
        }
        EndTextureMode();

        // Draw depth
        BeginTextureMode(depthTarget);
        {
            for (int i = 0; i < model.materialCount; i++) {
                model.materials[i].shader = depth;
            }
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                DrawModel(model, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
                // Don't draw things we don't want to outlinerate.
                // DrawGrid(10, 1.0f);
            }
            EndMode3D();
        }
        EndTextureMode();

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // Draw the deferred buffers.
            BeginShaderMode(postProc);
            SetShaderValueTexture(postProc,
                                  GetShaderLocation(postProc, "depthTexture"),
                                  depthTarget.texture);
            DrawTextureRec(
                target.texture,
                Rectangle{0, 0, static_cast<float>(target.texture.width),
                          static_cast<float>(-target.texture.height)},
                Vector2{0, 0}, WHITE);
            EndShaderMode();
        }
        EndDrawing();
    }

    return 0;
}
