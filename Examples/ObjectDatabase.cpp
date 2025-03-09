#include <cassert>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

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

struct ObjectData {
    ObjectType m_type;

    vector<int> north_views;
    vector<int> east_views;
    vector<int> south_views;
    vector<int> west_views;

    ObjectData() : m_type(Prop) {}
};

struct ParseContext {
    map<string, ObjectData> m_database;
    string m_name = "";
    ObjectData m_data;

    vector<int>* getFace(Face face) {
        switch (face) {
        case North:
            return &m_data.north_views;
        case East:
            return &m_data.east_views;
        case South:
            return &m_data.south_views;
        case West:
            return &m_data.west_views;
        default:
            assert(false);
        }
    }

    void add(Face face, int shape) {
        if (m_data.m_type == Character) {
            // Characters always have 16 north views and 16 south views.
            assert(face == South);
            for (int i = 0; i < 16; i++) {
                m_data.north_views.push_back(shape * 32 + i);
            }
            for (int i = 16; i < 32; i++) {
                m_data.south_views.push_back(shape * 32 + i);
            }
        } else {
            auto vec = getFace(face);
            for (int i = 0; i < 32; i++) {
                vec->push_back(shape * 32 + i);
            }
        }
    }

    void add(Face face, int shape, int frame) {
        auto vec = getFace(face);
        vec->push_back(shape * 32 + frame);
    }

    void add(Face face, int shape, int frame, int endFrame) {
        auto vec = getFace(face);
        for (int i = frame; i <= endFrame; i++) {
            vec->push_back(shape * 32 + i);
        }
    }
};

struct ObjectStarter {
    ParseContext& m_context;
    ObjectType m_type;

    ObjectStarter(ParseContext& context, ObjectType type)
        : m_context(context), m_type(type) {}

    void cycle(const char* name) {
        if (m_context.m_name != "") {
            m_context.m_database[m_context.m_name] = m_context.m_data;
            m_context.m_data = ObjectData();
            m_context.m_data.m_type = m_type;
        }

        if (m_context.m_database.find(name) != m_context.m_database.end()) {
            printf("Error: Duplicate object name: %s\n", name);
            exit(1);
        }

        m_context.m_name = name;
    }

    void operator()(const char* name) {
        // Objects with no south-facing view.
        cycle(name);
    }

    void operator()(const char* name, int shape) {
        // Character or south view of all shape frames.
        cycle(name);
        m_context.add(South, shape);
    }

    void operator()(const char* name, int shape, int frame) {
        // Single frame south view.
        cycle(name);
        m_context.add(South, shape, frame);
    }

    void operator()(const char* name, int shape, int frame, int endFrame) {
        // Frame range south view.
        cycle(name);
        m_context.add(South, shape, frame, endFrame);
    }
};

struct Functor {
    void operator()() {
        // TODO
    }

    void operator()(int shape) {
        // TODO
    }

    void operator()(int shape, int frame) {
        // TODO
    }

    void operator()(int shape, int frame, int endFrame) {
        // TODO
    }

    void operator()(int x, int y, int z, int shape) {
        // TODO
    }

    void operator()(int x, int y, int z, int shape, int frame) {
        // TODO
    }

    void operator()(int x, int y, int z, int shape, int frame, int endFrame) {
        // TODO
    }
};

map<string, ObjectData> buildDatabase() {
    ParseContext context;

    // TODO: Fill in more enum types as needed.
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

    // TODO: Functors that do things for the complex stuff.
    auto alt = Functor();
    auto east = Functor();
    auto north = Functor();
    auto west = Functor();
    auto south = Functor();
    auto extend = Functor();

// No-op types for
#include "u7objects.inc"

    return context.m_database;
}

int main() {
    auto database = buildDatabase();

    int count = 0;
    for (auto& pair : database) {
        count++;
        printf("%s\n", pair.first.c_str());
        if (pair.second.north_views.size() > 0) {
            printf("  north");
            for (int frame : pair.second.north_views)
                printf(" %d:%d", frame / 32, frame % 32);
            printf("\n");
        }
        if (pair.second.east_views.size() > 0) {
            printf("  east");
            for (int frame : pair.second.east_views)
                printf(" %d:%d", frame / 32, frame % 32);
            printf("\n");
        }
        if (pair.second.south_views.size() > 0) {
            printf("  south");
            for (int frame : pair.second.south_views)
                printf(" %d:%d", frame / 32, frame % 32);
            printf("\n");
        }

        if (pair.second.west_views.size() > 0) {
            printf("  west");
            for (int frame : pair.second.west_views)
                printf(" %d:%d", frame / 32, frame % 32);
            printf("\n");
        }
    }

    fprintf(stderr, "\nTotal objects: %d\n", count);
}
