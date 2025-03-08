#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// Normalized frame address: shape_num * 32 + frame_num

// TODO: Weapons and ammo have frames that shouldn't go into catalog animation
// TODO: Add "up" facing to the cardinal facings, needed with shields,
// different encoding in items

enum ObjectType {
    Prop,
    Character,
};

enum Face {
    North,
    East,
    South,
    West,
};

const char* faceName(Face face) {
    switch (face) {
    case North:
        return "north";
    case East:
        return "east";
    case South:
        return "south";
    case West:
        return "west";
    default:
        assert(false);
    }
    return "";
}

struct View {
    vector<int> frames;
    int m_offsetX = 0;
    int m_offsetY = 0;
};

struct ObjectData {
    ObjectType m_type = Prop;

    // Key is facing direction + x,y,z extents for large object parts.
    map<tuple<Face, int, int, int>, View> m_views;
};

void die(const char* message, ...) {
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    exit(1);
}

/// State machine being operated by the object database instructions.
struct ParseContext {
    map<string, ObjectData> m_database;
    string m_currentName = "";
    ObjectData m_currentData;
    Face m_currentFacing;

    int m_extX = 0;
    int m_extY = 0;
    int m_extZ = 0;

    void begin(const char* name, ObjectType type) {
        if (m_currentName != "") {
            m_database[m_currentName] = m_currentData;
            m_currentData = ObjectData();
        }

        m_currentData.m_type = type;

        if (m_database.find(name) != m_database.end()) {
            die("Error: Duplicate object name: %s\n", name);
        }

        m_extX = 0;
        m_extY = 0;
        m_extZ = 0;

        m_currentName = name;
        m_currentFacing = South;
    }

    void adjustOffset(int x, int y) {
        auto key = make_tuple(m_currentFacing, m_extX, m_extY, m_extZ);
        m_currentData.m_views[key].m_offsetX = x;
        m_currentData.m_views[key].m_offsetY = y;
    }

    void newView(Face facing) {
        m_currentFacing = facing;
        m_extX = 0;
        m_extY = 0;
        m_extZ = 0;
    }

    void add(Face face, int shape) {
        if (m_currentData.m_type == Character) {
            // Characters always have 16 north views and 16 south views.
            assert(face == South);
            // Characters are single shapes.
            assert(m_extX == 0 && m_extY == 0 && m_extZ == 0);

            auto key_n = make_tuple(North, 0, 0, 0);
            auto key_s = make_tuple(South, 0, 0, 0);
            for (int i = 0; i < 16; i++) {
                m_currentData.m_views[key_n].frames.push_back(shape * 32 + i);
            }
            for (int i = 16; i < 32; i++) {
                m_currentData.m_views[key_s].frames.push_back(shape * 32 + i);
            }
        } else {
            auto key = make_tuple(face, m_extX, m_extY, m_extZ);
            for (int i = 0; i < 32; i++) {
                m_currentData.m_views[key].frames.push_back(shape * 32 + i);
            }
        }
    }

    void add(Face face, int shape, int frame) {
        auto key = make_tuple(face, m_extX, m_extY, m_extZ);
        m_currentData.m_views[key].frames.push_back(shape * 32 + frame);
    }

    void add(Face face, int shape, int frame, int endFrame) {
        auto key = make_tuple(face, m_extX, m_extY, m_extZ);
        for (int i = frame; i <= endFrame; i++) {
            m_currentData.m_views[key].frames.push_back(shape * 32 + i);
        }
    }
};

struct ObjectStarter {
    ParseContext& m_context;
    ObjectType m_type;

    ObjectStarter(ParseContext& context, ObjectType type)
        : m_context(context), m_type(type) {}

    void operator()(const char* name) {
        // Objects with no south-facing view.
        m_context.begin(name, m_type);
    }

    void operator()(const char* name, int shape) {
        // Character or south view of all shape frames.
        m_context.begin(name, m_type);
        m_context.add(South, shape);
    }

    void operator()(const char* name, int shape, int frame) {
        // Single frame south view.
        m_context.begin(name, m_type);
        m_context.add(South, shape, frame);
    }

    void operator()(const char* name, int shape, int frame, int endFrame) {
        // Frame range south view.
        m_context.begin(name, m_type);
        m_context.add(South, shape, frame, endFrame);
    }
};

struct ViewAdder {
    ParseContext& m_context;
    Face m_facing;

    ViewAdder(ParseContext& context, Face facing)
        : m_context(context), m_facing(facing) {}

    void operator()(int shape) {
        m_context.newView(m_facing);
        m_context.add(m_facing, shape);
    }

    void operator()(int shape, int frame) {
        m_context.newView(m_facing);
        m_context.add(m_facing, shape, frame);
    }

    void operator()(int shape, int frame, int endFrame) {
        m_context.newView(m_facing);
        m_context.add(m_facing, shape, frame, endFrame);
    }
};

struct Extender {
    ParseContext& m_context;

    Extender(ParseContext& context) : m_context(context) {}

    void operator()(int x, int y, int z, int shape) {
        m_context.m_extX = x;
        m_context.m_extY = y;
        m_context.m_extZ = z;
        m_context.add(m_context.m_currentFacing, shape);
    }

    void operator()(int x, int y, int z, int shape, int frame) {
        m_context.m_extX = x;
        m_context.m_extY = y;
        m_context.m_extZ = z;
        m_context.add(m_context.m_currentFacing, shape, frame);
    }

    void operator()(int x, int y, int z, int shape, int frame, int endFrame) {
        m_context.m_extX = x;
        m_context.m_extY = y;
        m_context.m_extZ = z;
        m_context.add(m_context.m_currentFacing, shape, frame, endFrame);
    }
};

struct Functor {
    void operator()(int shape) {
        // blank
    }

    void operator()(int shape, int frame) {
        // blank
    }

    void operator()(int shape, int frame, int endFrame) {
        // blank
    }
};

map<string, ObjectData> buildDatabase() {
    ParseContext context;

    // Fill in more enum types as needed.
    auto ammo = ObjectStarter(context, Prop);
    auto creature = ObjectStarter(context, Character);
    auto human = ObjectStarter(context, Character);
    auto item = ObjectStarter(context, Prop);
    auto pile = ObjectStarter(context, Prop);
    auto plant = ObjectStarter(context, Prop);
    auto prop = ObjectStarter(context, Prop);
    auto rock = ObjectStarter(context, Prop);
    auto shield = ObjectStarter(context, Prop);
    auto tree = ObjectStarter(context, Prop);
    auto weapon = ObjectStarter(context, Prop);

    // TODO: Handle alt stuff somehow, currently just no-opping it.
    auto alt = Functor();
    auto east = ViewAdder(context, East);
    auto north = ViewAdder(context, North);
    auto west = ViewAdder(context, West);
    auto south = ViewAdder(context, South);
    auto extend = Extender(context);

    auto offset = [&context](int x, int y) { context.adjustOffset(x, y); };

// Process the actual data.
#include "u7objects.inc"

    return context.m_database;
}

int main() {
    auto database = buildDatabase();

    int count = 0;
    for (auto& pair : database) {
        count++;
        printf("%s\n", pair.first.c_str());

        for (auto& view : pair.second.m_views) {
            Face dir;
            int x, y, z;
            tie(dir, x, y, z) = view.first;

            if (z != 0) {
                printf("  %s %d %d %d\n", faceName(dir), x, y, z);
            } else if (x != 0 || y != 0) {
                printf("  %s %d %d\n", faceName(dir), x, y);
            } else {
                printf("  %s\n", faceName(dir));
            }

            printf("    frames");
            for (int frame : view.second.frames) {
                printf(" %d:%d", frame / 32, frame % 32);
            }
            printf("\n");

            if (view.second.m_offsetX != 0 || view.second.m_offsetY != 0) {
                printf("    offset %d %d\n", view.second.m_offsetX,
                       view.second.m_offsetY);
            }
        }
    }

    fprintf(stderr, "\nTotal objects: %d\n", count);
}
