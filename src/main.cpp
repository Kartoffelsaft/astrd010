#include <cstdio>
#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <optional>

#include "player.h"
#include "shaders.h"
#include "paintjob.h"
#include "input.h"
#include "constructpart.h"

auto player = Player{
    .position = Vector3{0.0f, 0.0f, 5.0f},
    .rotation = MatrixIdentity(),
};

void draw() {
    BeginDrawing(); {
        ClearBackground(BLACK);

        DrawText("astrd010", 10, 10, 16, RAYWHITE);
        DrawFPS(10, 30);

        BeginMode3D(player.GetCamera()); {
            for (auto m: shipParts) {
                m.draw();
            }

            player.drawPlacementIndicator();
        } EndMode3D();
        
        player.placementIndicator.drawSelectionMenu();
    } EndDrawing();
}

void placePartAt(Matrix orientation, PlacementIndicator what) {
    auto newPart = ConstructPart{
        .shape = (PlaceableShape)what.placeableSelection,
        .texture = ERODED_METAL,
        .paint = what.paint,

        .where = orientation,
    };

    shipParts.push_back(newPart);
}

// why does raylib not have this already? this way of doing it feels jank
Mesh LoadMesh(char const * filename) {
    auto model = LoadModel(filename);
    if (model.meshCount != 1) printf("WARNING: unexpected number of meshes in file %s (%d meshes)\n", filename, model.meshCount);
    
    auto ret = model.meshes[0];
    for (auto i = 0; i < model.materialCount; i++) {
        UnloadMaterial(model.materials[i]);
    }

    return ret;
}

Material loadPlaceableMaterial(char const * textureFilename, char const * normalMapFilename) {
    auto ret = LoadMaterialDefault();
    ret.shader = ConstructPart::paintInfo.shader;
    ret.maps[0].texture = LoadTexture(textureFilename);
    
    if (normalMapFilename) ret.maps[1].texture = LoadTexture(normalMapFilename);
    else ret.maps[1].texture = LoadTextureFromImage(GenImageColor(1, 1, Color{127, 127, 255}));

    return ret;
}

int main() {
    InitWindow(800, 600, "astrd010");
    SetTargetFPS(60);

    ConstructPart::paintInfo.load(loadShaderPreprocess("resources/shaders/lighting.vs", "resources/shaders/lighting.fs"));
    player.placementIndicator.paintInfo.load(loadShaderPreprocess(nullptr, "resources/shaders/indicator.fs"));

    placeableShapes[TUBE] = LoadMesh("./resources/models/tube.obj");
    placeableShapes[CUBE] = LoadMesh("./resources/models/cube.obj");
    
    // TODO, snap point files
    placeableSnapPoints[TUBE] = {
        {0,  1, 0},
        {0, -1, 0},
    };
    
    placeableSnapPoints[CUBE] = {
        {-1,  0,  0},
        { 1,  0,  0},
        { 0, -1,  0},
        { 0,  1,  0},
        { 0,  0, -1},
        { 0,  0,  1},
    };

    placeableTextures[ERODED_METAL] = loadPlaceableMaterial("./resources/textures/metaltile.png", "./resources/textures/metaltile_normal.png");

    while (!WindowShouldClose()) {
        player.update();

        if (mouseLookMode && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            placePartAt(
                player.getPlacementIndicatorOrientation(), 
                player.placementIndicator
            );
        }

        draw();
    }

    CloseWindow();

    return 0;
}
