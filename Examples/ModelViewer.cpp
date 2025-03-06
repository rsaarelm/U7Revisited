#include <cstdio>
#include <map>
#include <vector>
#include <array>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

const int SCREEN_W = 1920;
const int SCREEN_H = 1080;

using namespace std;

Mesh makeShellMesh(float weight, Mesh* source) {
    Mesh mesh = {0};
    mesh.vertexCount = source->vertexCount;
    mesh.triangleCount = source->triangleCount;

    vector<Vector3> vertices;
    for (int i = 0; i < mesh.vertexCount * 3; i += 3) {
        vertices.push_back({source->vertices[i], source->vertices[i + 1],
                            source->vertices[i + 2]});
    }

    vector<array<unsigned short, 3>> triangles;
    for (int i = 0; i < mesh.triangleCount * 3; i += 3) {
        triangles.push_back({source->indices[i], source->indices[i + 1],
                             source->indices[i + 2]});
    }

    // Synthesize normals for the outline from geometry, don't trust what the
    // model gives us.
    map<array<float, 3>, Vector3> normals;

    for (auto i = 0; i < vertices.size(); i++) {
        auto vertex = vertices[i];
        // std::array form that can be used as map key.
        array<float, 3> key = {vertex.x, vertex.y, vertex.z};
        if (normals.find(key) != normals.end()) {
            // We've already done this vertex, continue.
            continue;
        }

        // Go through all triangles and see if they contain an identical
        // vertex.
        Vector3 normal = {0};
        for (auto triangle : triangles) {
            for (auto index : triangle) {
                auto v = vertices[index];
                if (Vector3Equals(v, vertex)) {
                    // This triangle contains our vertex, add its normal.
                    auto v1 = vertices[triangle[0]];
                    auto v2 = vertices[triangle[1]];
                    auto v3 = vertices[triangle[2]];

                    normal = Vector3Add(
                        normal, Vector3CrossProduct(Vector3Subtract(v2, v1),
                                                    Vector3Subtract(v3, v1)));
                    // And move to next triangle.
                    break;
                }
            }
        }

        // Add the combined normal.
        normals[key] = Vector3Normalize(normal);
    }

    // I hate C++.
    mesh.vertices = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)RL_MALLOC(mesh.vertexCount * 2 * sizeof(float));
    mesh.normals = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
    mesh.indices = (unsigned short*)RL_MALLOC(mesh.triangleCount * 3 *
                                              sizeof(unsigned short));

    for (int i = 0; i < mesh.vertexCount * 2; i++)
        mesh.texcoords[i] = source->texcoords[i];
    for (int i = 0; i < mesh.vertexCount * 3; i++)
        mesh.normals[i] = source->normals[i];

    // Offset vertices along normals.
    for (int i = 0; i < mesh.vertexCount * 3; i += 3) {
        Vector3 here = {source->vertices[i], source->vertices[i + 1],
                        source->vertices[i + 2]};
        auto normal = Vector3Scale(normals[{here.x, here.y, here.z}], weight);
        Vector3 vertex = Vector3Add(here, normal);

        mesh.vertices[i] = vertex.x;
        mesh.vertices[i + 1] = vertex.y;
        mesh.vertices[i + 2] = vertex.z;
    }

    for (int i = 0; i < mesh.triangleCount * 3; i += 3) {
        // Flip all triangles to pull off the outline trick. Now the backside
        // of the shell is drawn and the front is invisible.
        mesh.indices[i] = source->indices[i];
        mesh.indices[i + 1] = source->indices[i + 2];
        mesh.indices[i + 2] = source->indices[i + 1];
    }

    // TODO: Need to clone animations too when we're making shells for
    // animated meshes...

    UploadMesh(&mesh, false);
    return mesh;
}

Model makeShell(float weight, Mesh* source) {
    Mesh mesh = makeShellMesh(weight, source);
    Model model = LoadModelFromMesh(mesh);
    // Turn it black.
    for (auto i = 0; i < model.materialCount; i++) {
        model.materials[i].maps[MATERIAL_MAP_DIFFUSE].color = BLACK;
    }
    return model;
}

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

    // Set up shaders for lighting.
    Shader shader = LoadShader("Examples/model.vs", "Examples/model.fs");

    shader.locs[SHADER_LOC_MATRIX_MODEL] =
        GetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    // Set up directional light
    int lightDirLoc = GetShaderLocation(shader, "lightDir");
    Vector3 lightDir = {2.0f, -4.0f, 2.0f};
    SetShaderValue(shader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

    Vector3 ambientLight = {0.2f, 0.2f, 0.2f};
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, &ambientLight, SHADER_UNIFORM_VEC3);

    model.materials[1].shader = shader;

    // Make the inverted shell model for drawing a black outline.

    // XXX: This only does the first mesh, so it won't work on multi-mesh
    // models.
    Model edgeShell = makeShell(0.05, &model.meshes[0]);

    // Camera setup.
    Camera camera = {{0}};
    camera.position = (Vector3){6.0f, 6.0f, 6.0f};
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Camera tileCamera = {{0}};
    tileCamera.position = (Vector3){0.0f, 20.0f, 0.0f};
    tileCamera.target = (Vector3){0.0f, 0.0f, 0.0f};
    tileCamera.up = (Vector3){0.0f, 0.0f, 1.0f};
    tileCamera.fovy = 4.0f;
    tileCamera.projection = CAMERA_ORTHOGRAPHIC;

    RenderTexture2D obliqueView = LoadRenderTexture(400, 400);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW],
                       &camera.position.x, SHADER_UNIFORM_VEC3);

        // Render an inset oblique view of the model.
        BeginTextureMode(obliqueView);
        {
            ClearBackground(SKYBLUE);
            BeginMode3D(tileCamera);
            {
                rlPushMatrix();
                Matrix oblique = MatrixIdentity();
                oblique.m4 = 0.5f;
                oblique.m6 = 0.5f;
                rlMultMatrixf(MatrixToFloat(oblique));

                DrawModel(model, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
                DrawModel(edgeShell, {0.0f, 0.0f, 0.0f}, 1.00f, WHITE);
                rlPopMatrix();
            }
            EndMode3D();
        }
        EndTextureMode();

        // Render a main perspective view of the model.
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                DrawModel(model, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
                DrawModel(edgeShell, {0.0f, 0.0f, 0.0f}, 1.00f, WHITE);
                DrawGrid(10, 1.0f);
            }
            EndMode3D();

            DrawTextureRec(
                obliqueView.texture,
                (Rectangle){0, 0, static_cast<float>(obliqueView.texture.width),
                            static_cast<float>(-obliqueView.texture.height)},
                (Vector2){8, 8}, WHITE);
        }
        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();

    return 0;
}
