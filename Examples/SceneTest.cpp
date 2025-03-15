#include <cassert>
#include <cstdint>
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <tuple>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

// Scene display demo.
//
// Use the U7 object database to determine how meshes loaded from the disk
// should be used to render a scene from the game data.

// {{{1 Prelude

using namespace std;

/// Object facing.
enum Face {
    South,
    East,
    North,
    West,
};

void die(const char* message, ...) {
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    exit(1);
}

/// Map game tile (8x8x4 pixels / 40cm cubed voxel) coordintates to world space.
Vector3 tileToWorld(int x, int y, int z) {
    const float c = 0.4f;
    return Vector3{(float)x * c, (float)z * c, (float)y * c};
}

/// Map game pixel (5cm) coordinates to world space.
Vector3 pixelToWorld(int x, int y) {
    const float c = 0.05f;
    return Vector3{(float)x * c, 0.0f, (float)y * c};
}

// {{{1 Scene data

// Functions here are brief excerpts of actual game data. They should report
// stuff as a game data parser does and not do any significant processing on
// it or use our u7 object database content yet.

Vector3 sizeData(int shape) {
    // Object sizes can be found in game data. Adding the subset used in the
    // scene since I don't want to have any game data dependency here.
    switch (shape) {
    case 158:
        return Vector3{1, 1, 1};
    case 711:
        return Vector3{4, 5, 2};
    case 830:
        return Vector3{3, 3, 1};
    case 862:
        return Vector3{1, 1, 1};
    case 880:
        return Vector3{1, 1, 1};
    case 917:
        return Vector3{1, 1, 4};
    case 941:
        return Vector3{2, 5, 7};
    case 942:
        return Vector3{2, 5, 7};
    }

    return Vector3{0, 0, 0};
}

// (x, y, z, shape-and-frame), coordinates in tiles.
vector<tuple<int, int, int, int>> buildScene() {
    // hax for human-readable shape-frame values: multiply shape by 100, add
    // frame.
    const int holderN = 15801;
    const int millStone = 71100;
    const int gear = 83000;
    const int shaftN = 86200;
    const int shaftNTeeth = 86201;
    const int shaftE = 88000;
    const int shaftETeeth = 88001;
    const int supportPole = 91700;
    const int supportHolderE = 91701;
    const int waterWheelS = 94100;
    const int waterWheelN = 94200;

    vector<tuple<int, int, int, int>> ret;

    auto add = [&ret](int x, int y, int z, int sf) {
        ret.push_back(make_tuple(x, y, z, sf));
    };

    // This should correspond to the actual game data for the machinery in the
    // Paws water wheel mill.

    add(889, 1719, 0, waterWheelN);
    add(889, 1724, 0, waterWheelS);

    // This seems to be a bug int the original game data. The shaft bit
    // connecting to the wheel is displaced from the shaft line, but in such a
    // way that it's not seen in the game's oblique view. It's very visible
    // in 3D though.
    // add(889, 1719, 1, shaftE);
    add(890, 1720, 3, shaftE);  // Changed to this.

    add(891, 1720, 3, shaftE);
    add(892, 1720, 3, shaftE);
    add(893, 1720, 3, shaftE);
    add(894, 1720, 3, shaftE);
    add(894, 1720, 0, supportHolderE);
    add(895, 1720, 3, shaftE);
    add(896, 1720, 3, shaftETeeth);
    add(897, 1720, 3, shaftE);

    add(898, 1720, 3, shaftE);
    add(898, 1720, 0, supportHolderE);
    add(899, 1720, 3, shaftE);

    add(901, 1721, 4, gear);
    add(900, 1720, 1, supportPole);
    add(901, 1721, 0, gear);

    add(900, 1719, 1, shaftNTeeth);
    add(900, 1718, 1, shaftN);
    add(900, 1717, 1, shaftN);
    add(900, 1717, 0, holderN);
    add(900, 1716, 1, shaftN);
    add(900, 1715, 1, shaftNTeeth);
    add(900, 1714, 1, shaftN);
    add(900, 1713, 1, shaftN);
    add(900, 1713, 0, holderN);
    add(900, 1712, 1, shaftN);

    add(901, 1711, 0, millStone);
    return ret;
}

// {{{1 Object database

// Create transformations based on model directives

/// World space rotation matrix for a given facing.
Matrix faceMtx(Face f) {
    // NB. It might be important to make sure that 90 degree turns are made
    // with matrices like this with exact ones in them and not with
    // trigonometry, since we want to avoid any numerical imprecision with the
    // vertices of the rotated objects fitting together.
    const Matrix ccwTurn{0,  0, 1, 0,  //
                         0,  1, 0, 0,  //
                         -1, 0, 0, 0,  //
                         0,  0, 0, 1};

    Matrix ret = MatrixIdentity();
    // Hack based on the order of the enum values being the counterclockwise
    // progression.
    for (auto i = 0; i < f; i++) {
        ret = MatrixMultiply(ret, ccwTurn);
    }
    return ret;
}

/// A view to an object that's associated with a game shape.
struct ObjectView {
    /// Name of the model of this object.
    string m_name;
    /// Which way is this view facing.
    Face m_facing = South;

    /// Bounding box for the view in world space.
    Vector3 m_boundsMin = Vector3{0, 0, 0};
    Vector3 m_boundsMax = Vector3{0, 0, 0};

    /// Custom offset from object definitions.
    Vector3 m_offset = Vector3{0, 0, 0};

    ObjectView() {}

    ObjectView(string name, Face facing) : m_name(name), m_facing(facing) {}

    void addBounds(int xOffset, int yOffset, int zOffset, int shape) {
        // Get the game data for size.
        auto dim = sizeData(shape);

        // The bounding box is to the northwest and up from the origin.

        // Convert to world coordinates, swap y and z and scale from tiles to
        // metric.
        auto p1 = tileToWorld(xOffset - dim.x, yOffset - dim.y, zOffset);
        auto p2 = tileToWorld(xOffset, yOffset, zOffset + dim.z);

        // Update bounding box to contain new extremities.
        m_boundsMin = Vector3Min(p1, m_boundsMin);
        m_boundsMax = Vector3Max(p2, m_boundsMax);
    }

    void adjustOffset(int x, int y) {
        // Transform offset to world space and rotate it to match facing.
        auto offset = pixelToWorld(x, y);
        m_offset = Vector3Transform(offset, faceMtx(m_facing));
    }

    /// Build the model transform for this view.
    Matrix modelTransform() {
        // XXX: It's ineffective to compute this every time, this could be
        // cached in the object instead.

        // Initial offset from bounding box bottom center.
        auto offset = Vector3Scale(Vector3Add(m_boundsMin, m_boundsMax), 0.5f);
        offset.y = 0;

        // Add custom offset to it.
        offset = Vector3Add(offset, m_offset);

        // Start from the rotation matrix.
        auto ret = faceMtx(m_facing);

        // Apply offset
        ret.m12 = offset.x;
        ret.m13 = offset.y;
        ret.m14 = offset.z;

        return ret;
    }
};

/// Database of known named objects and their views.
struct ObjectDatabase {
    /// List the names of known objects, used to load models.
    set<string> m_objects;
    /// Map shape+frame values into object name, facing and bounding box
    /// (min-corner, max-corner)
    map<uint32_t, ObjectView> m_views;
};

/// Context object for parsing the object description dataset.
struct ParseContext {
    ObjectDatabase m_db;

    string m_currentName = "";
    /// The frames that were added in the last operation. Used by offset
    /// adjustment.
    vector<int> m_previousKeys;

    /// Set to true after adding an extension bit, offset adjustments for
    /// extensions are not registered.
    bool m_inExtension = false;

    void begin(const char* name) {
        if (m_db.m_objects.find(name) != m_db.m_objects.end()) {
            die("Error: Duplicate object name: %s\n", name);
        }

        m_currentName = name;
        m_previousKeys = {};
        m_inExtension = false;

        m_db.m_objects.insert(name);
    }

    void add(Face face, int shape) {
        m_inExtension = false;
        m_previousKeys = {};

        for (auto key = shape * 100; key < shape * 100 + 32; key++) {
            m_previousKeys.push_back(key);
            m_db.m_views[key] = ObjectView(m_currentName, face);
        }
        addBounds(0, 0, 0, shape);
    }

    void add(Face face, int shape, int frame) {
        m_inExtension = false;
        m_previousKeys = {shape * 100 + frame};

        m_db.m_views[shape * 100 + frame] = ObjectView(m_currentName, face);
        addBounds(0, 0, 0, shape);
    }

    void add(Face face, int shape, int frame, int endFrame) {
        m_inExtension = false;
        m_previousKeys = {};
        for (auto key = shape * 100 + frame; key <= shape * 100 + endFrame;
             key++) {
            m_previousKeys.push_back(key);
            m_db.m_views[key] = ObjectView(m_currentName, face);
        }
        addBounds(0, 0, 0, shape);
    }

    void adjustOffset(int x, int y) {
        if (m_inExtension) {
            return;
        }

        for (auto key : m_previousKeys) {
            m_db.m_views[key].adjustOffset(x, y);
        }
    }

    void addBounds(int xOffset, int yOffset, int zOffset, int shape) {
        for (auto key : m_previousKeys) {
            m_db.m_views[key].addBounds(xOffset, yOffset, zOffset, shape);
        }
    }

    void addExtension(int xOffset, int yOffset, int zOffset, int shape) {
        m_inExtension = true;
        addBounds(xOffset, yOffset, zOffset, shape);
    }
};

/// Interpreter functor for registering objects and adding views to them.
struct ObjectFunctor {
    ParseContext& m_context;
    Face m_facing = South;

    ObjectFunctor(ParseContext& context) : m_context(context) {}

    ObjectFunctor(ParseContext& context, Face facing)
        : m_context(context), m_facing(facing) {}

    void operator()(const char* name) {
        // Objects with no south-facing view.
        m_context.begin(name);
    }

    void operator()(const char* name, int shape) {
        // Character or south view of all shape frames.
        m_context.begin(name);
        m_context.add(m_facing, shape);
    }

    void operator()(const char* name, int shape, int frame) {
        // Single frame south view.
        m_context.begin(name);
        m_context.add(m_facing, shape, frame);
    }

    void operator()(const char* name, int shape, int frame, int endFrame) {
        // Frame range south view.
        m_context.begin(name);
        m_context.add(m_facing, shape, frame, endFrame);
    }

    void operator()(int shape) { m_context.add(m_facing, shape); }

    void operator()(int shape, int frame) {
        m_context.add(m_facing, shape, frame);
    }

    void operator()(int shape, int frame, int endFrame) {
        m_context.add(m_facing, shape, frame, endFrame);
    }
};

/// Interpreter functor for adding extension shapes.
struct ExtendFunctor {
    ParseContext& m_context;

    ExtendFunctor(ParseContext& context) : m_context(context) {}

    // We don't care about the frames of the extension objects. The shape is
    // important though since it determines how much the bounding box must be
    // changed.

    void operator()(int x, int y, int z, int shape) {
        m_context.addExtension(x, y, z, shape);
    }
    void operator()(int x, int y, int z, int shape, int frame) {
        m_context.addExtension(x, y, z, shape);
    }
    void operator()(int x, int y, int z, int shape, int frame, int endFrame) {
        m_context.addExtension(x, y, z, shape);
    }
};

// For parts of the database we're ignoring.
struct StubFunctor {
    void operator()(int shape) {}
    void operator()(int shape, int frame) {}
    void operator()(int shape, int frame, int endFrame) {}
    void operator()(int x, int y, int z, int shape) {}
    void operator()(int x, int y, int z, int shape, int frame) {}
    void operator()(int x, int y, int z, int shape, int frame, int endFrame) {}
};

/// Build the object database from the u7objects.inc dataset script.
ObjectDatabase buildDatabase() {
    ParseContext context;

    // Fill in more enum types as needed.
    auto ammo = ObjectFunctor(context);
    auto creature = ObjectFunctor(context);
    auto human = ObjectFunctor(context);
    auto item = ObjectFunctor(context);
    auto pile = ObjectFunctor(context);
    auto plant = ObjectFunctor(context);
    auto prop = ObjectFunctor(context);
    auto rock = ObjectFunctor(context);
    auto shield = ObjectFunctor(context);
    auto tree = ObjectFunctor(context);
    auto weapon = ObjectFunctor(context);

    auto east = ObjectFunctor(context, East);
    auto north = ObjectFunctor(context, North);
    auto west = ObjectFunctor(context, West);
    auto south = ObjectFunctor(context, South);

    auto extend = ExtendFunctor(context);

    auto offset = [&context](int x, int y) { context.adjustOffset(x, y); };

    auto alt = StubFunctor();

// Process the actual data.
#include "u7objects.inc"

    return context.m_db;
}

// {{{1 Program code

/// Wrapper object for animated raylib models.
struct RayModel {
    Model m_model;
    ModelAnimation* m_animations = nullptr;
    int m_animCount = 0;

    RayModel(const char* path) {
        m_model = LoadModel(path);
        assert(m_model.meshCount);
        m_animations = LoadModelAnimations(path, &m_animCount);
    }

    ~RayModel() {
        if (m_animations) {
            UnloadModelAnimations(m_animations, m_animCount);
        }

        UnloadModel(m_model);
    }

    void setAnimFrame(int anim, int frame) {
        UpdateModelAnimation(m_model, m_animations[anim], frame);
    }

 private:
    // non-copyable
    RayModel();
    RayModel(const RayModel&);
    RayModel& operator=(const RayModel&);
};

/// Main game engine with loaded models and the object database.
struct Engine {
    map<string, RayModel> m_models;
    ObjectDatabase m_db;

    Engine() {
        m_db = buildDatabase();

        /// Load all glTF models that have names of known objects from disk
        /// into runtime model storage.
        auto files = LoadDirectoryFilesEx("Examples", ".gltf", false);
        for (unsigned int i = 0; i < files.count; i++) {
            const char* name = GetFileNameWithoutExt(files.paths[i]);
            if (m_db.m_objects.find(name) != m_db.m_objects.end()) {
                fprintf(stderr, "Found model %s\n",
                        GetFileNameWithoutExt(files.paths[i]));

                // RayModels are non-copyable, so we need funny stuff to put
                // them on the map.
                m_models.try_emplace(name, files.paths[i]);
            }
        }
        UnloadDirectoryFiles(files);
    }

 private:
    // non-copyable
    Engine(const Engine&);
    Engine& operator=(const Engine&);
};

// {{{1 Main function

int main(int argc, char* argv[]) {
    InitWindow(1920, 1080, "Scene Test");
    SetTargetFPS(60);
    rlEnableBackfaceCulling();

    auto scene = buildScene();

    Engine engine;

    Camera camera;
    camera.position = Vector3{6.0f, 6.0f, 6.0f};
    camera.target = Vector3{0.0f, 0.0f, 0.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Set up shaders for lighting.
    Shader shader = LoadShader("Examples/model.vs", "Examples/model.fs");

    shader.locs[SHADER_LOC_MATRIX_MODEL] =
        GetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    // Set up directional light
    int lightDirLoc = GetShaderLocation(shader, "lightDir");
    Vector3 lightDir = {2.0f, -4.0f, -2.0f};
    SetShaderValue(shader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

    Vector3 ambientLight = {0.2f, 0.2f, 0.2f};
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, &ambientLight, SHADER_UNIFORM_VEC3);

    for (auto& [name, model] : engine.m_models) {
        model.m_model.materials[1].shader = shader;
    }

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_SPACE)) {
            UpdateCamera(&camera, CAMERA_ORBITAL);
            UpdateCamera(&camera, CAMERA_ORBITAL);
            UpdateCamera(&camera, CAMERA_ORBITAL);
            UpdateCamera(&camera, CAMERA_ORBITAL);
        }

        // Render a main perspective view of the model.
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                for (auto [x, y, z, sf] : scene) {
                    // Bring near origin.
                    x -= 900;
                    y -= 1715;
                    auto worldPos = tileToWorld(x, y, z);

                    if (engine.m_db.m_views.find(sf) ==
                        engine.m_db.m_views.end()) {
                        // Complete unknown, ignore this.
                        continue;
                    }

                    auto view = engine.m_db.m_views[sf];

                    if (engine.m_models.find(view.m_name) !=
                        engine.m_models.end()) {
                        auto& model = engine.m_models.at(view.m_name);

                        model.m_model.transform = view.modelTransform();
                        DrawModel(model.m_model, worldPos, 1.0f, WHITE);
                    } else {
                        // Draw a box for missing models.
                        Vector3 center = Vector3Scale(
                            Vector3Add(view.m_boundsMin, view.m_boundsMax),
                            0.5f);
                        Vector3 dim =
                            Vector3Subtract(view.m_boundsMax, view.m_boundsMin);
                        DrawCubeWires(Vector3Add(center, worldPos), dim.x,
                                      dim.y, dim.z, MAROON);
                    }
                }

                DrawGrid(10, 1.0f);
            }
            EndMode3D();
        }
        EndDrawing();
    }

    return 0;
}

// vim:foldmethod=marker
