#include <vector>
#include <array>
#include <algorithm>
#include <optional>

#include "raylib.h"
#include "raygui.h"
#include "raymath.h"

#include "mathUtils.h"
#include "placementIndicator.h"
#include "shaders.h"
#include "input.h"

std::vector<Mesh> placeableShapes{PLACEABLE_SHAPES_COUNT};
std::vector<std::vector<Vector3>> placeableSnapPoints{PLACEABLE_SHAPES_COUNT};
std::vector<Material> placeableTextures{PLACEABLE_TEXTURES_COUNT};

// std::vector<float> const rotationSnaps = {
//       0.f * DEG2RAD,
//      30.f * DEG2RAD,
//      45.f * DEG2RAD,
//      60.f * DEG2RAD,
//      90.f * DEG2RAD,
//     120.f * DEG2RAD,
//     135.f * DEG2RAD,
//     150.f * DEG2RAD,
//     180.f * DEG2RAD,
//     210.f * DEG2RAD,
//     225.f * DEG2RAD,
//     240.f * DEG2RAD,
//     270.f * DEG2RAD,
//     300.f * DEG2RAD,
//     315.f * DEG2RAD,
//     330.f * DEG2RAD,
// };
// 
// int rotSnapNext(int const index) { return (index + 1 + rotationSnaps.size()) % rotationSnaps.size(); }
// int rotSnapPrev(int const index) { return (index - 1 + rotationSnaps.size()) % rotationSnaps.size(); }

constexpr std::vector<Vector3> generateSnapDirections() {
    auto out = std::vector<Vector3>{};

    for (float x = -2.f; x <= 2.f; x += 1.f) {
        for (float y = -2.f; y <= 2.f; y += 1.f) {
            for (float z = -2.f; z <= 2.f; z += 1.f) {
                if (abs(x) > 1.9f
                ||  abs(y) > 1.9f
                ||  abs(z) > 1.9f) {
                    out.push_back(Vector3Normalize(Vector3{x, y, z}));
                }
            }
        }
    }

    return out;
}
std::vector<Vector3> const snapDirs = generateSnapDirections();

void PlacementIndicator::update(Matrix const lookdir) {
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        auto mouseDelta = GetMouseDelta();
        
        auto extraRotation = MatrixRotateZYX(Vector3Transform(Vector3{
            mouseDelta.y * 0.01f,
            mouseDelta.x * 0.01f, 
            0.0f
        }, lookdir));

        this->rotation = MatrixMultiply(this->rotation, extraRotation);
    }
    
    if (IsKeyPressed(KEY_Z)) {
        this->selectionMenuIsOpen ^= true;
        mouseLookMode = !this->selectionMenuIsOpen;
    }

    if (IsKeyPressed(KEY_LEFT_CONTROL)) mouseLookMode = false;
    if (IsKeyReleased(KEY_LEFT_CONTROL)) {
        this->applySnapRotation();
        mouseLookMode = true;
    }

    float selectionChange = GetMouseWheelMove();
    if (selectionChange > 0.f) {
        placeableSelection++;
    }
    if (selectionChange < 0.f) {
        placeableSelection--;
    }

    if (placeableSelection < 0) {
        placeableSelection = PLACEABLE_SHAPES_COUNT - 1;
    } else if (placeableSelection >= PLACEABLE_SHAPES_COUNT) {
        placeableSelection = 0;
    }
}

void PlacementIndicator::drawAt(Matrix const mtx) const {
    Material mat = placeableTextures[ERODED_METAL];
    mat.shader = paintInfo.shader;
    
    paint.applyPaintjob(this->paintInfo);

    DrawMesh(
        placeableShapes[this->placeableSelection],
        mat,
        mtx
    );
    
    // auto center = Vector3Transform(Vector3Zero(), mtx);
    
    // DrawLine3D(center, Vector3Transform(RIGHT, mtx), YELLOW);
    // DrawLine3D(center, Vector3Transform(Vector3Transform(UP, MatrixRotateX(rotationSnaps[this->rotSnapIndexX])), mtx), BLUE);
    // DrawLine3D(center, Vector3Transform(Vector3Transform(FORWARD, MatrixRotateZYX(Vector3{rotationSnaps[this->rotSnapIndexX], rotationSnaps[this->rotSnapIndexY], 0.f})), mtx), RED);
}

Matrix PlacementIndicator::getRotationMatrix() const {
    return this->rotation;
}

void PlacementIndicator::applySnapRotation() {
    auto currentDir = Vector3Transform(UP, this->rotation);

    int closestIdx = -1;
    float closestDist = INFINITY;
    for (int i = 0; i < snapDirs.size(); i++) {
        auto dist = Vector3DistanceSqr(currentDir, snapDirs[i]);

        if (dist < closestDist) {
            closestDist = dist;
            closestIdx = i;
        }
    }

    this->rotation = MatrixInvert(MatrixMultiply(
        MatrixLookAt(Vector3Zero(), snapDirs[closestIdx], Vector3{0.01f, 0.f, 0.99f}),
        Matrix{
            1, 0, 0, 0,
            0, 0, 1, 0,
            0,-1, 0, 0,
            0, 0, 0, 1,
        }
    ));
}

void setRGBColorPicker(Rectangle bounds, char const * title, Vector3& rgb) {
    auto c = Color{
        .r = (unsigned char)(rgb.x * 256),
        .g = (unsigned char)(rgb.y * 256),
        .b = (unsigned char)(rgb.z * 256),
        .a = 255
    };
    
    c = GuiColorPicker(bounds, title, c);
    
    rgb.x = c.r / 256.f;
    rgb.y = c.g / 256.f;
    rgb.z = c.b / 256.f;
}

void PlacementIndicator::drawSelectionMenu() {
    if (!this->selectionMenuIsOpen) return;
    
    float const COLOR_PICKER_WIDTH   = 110;
    float const COLOR_PICKER_HEIGHT  = 110;
    float const COLOR_PICKER_PADDING = 10;
    float const COLOR_PICKER_HUEBAR_PADDING = GuiGetStyle(COLORPICKER, HUEBAR_PADDING);
    float const COLOR_PICKER_HUEBAR_WIDTH = GuiGetStyle(COLORPICKER, HUEBAR_WIDTH);
    
    float const SELECTION_MENU_WIDTH = 
        3 * COLOR_PICKER_WIDTH +
        3 * COLOR_PICKER_HUEBAR_PADDING +
        3 * COLOR_PICKER_HUEBAR_WIDTH +
        5 * COLOR_PICKER_PADDING;
        
    float const SELECTION_MENU_HEIGHT =
        1 * COLOR_PICKER_HEIGHT +
        2 * COLOR_PICKER_PADDING;
        
    float const SELECTION_MENU_X = 300;
    float const SELECTION_MENU_Y = 100;
    
    float const SELECTION_MENU_MENUBAR_HEIGHT = 24; // Why doesn't raygui have an easy way to find what this is?
    
    GuiPanel(Rectangle{SELECTION_MENU_X, SELECTION_MENU_Y, SELECTION_MENU_WIDTH, SELECTION_MENU_HEIGHT + SELECTION_MENU_MENUBAR_HEIGHT}, "Selection Menu");
    
    setRGBColorPicker(Rectangle{SELECTION_MENU_X +     COLOR_PICKER_PADDING                                                                                     , SELECTION_MENU_Y + SELECTION_MENU_MENUBAR_HEIGHT + COLOR_PICKER_PADDING, COLOR_PICKER_WIDTH, COLOR_PICKER_HEIGHT}, "Primary Color"  , this->paint.primaryColor  );
    setRGBColorPicker(Rectangle{SELECTION_MENU_X + 2 * COLOR_PICKER_PADDING +     (COLOR_PICKER_WIDTH + COLOR_PICKER_HUEBAR_PADDING + COLOR_PICKER_HUEBAR_WIDTH), SELECTION_MENU_Y + SELECTION_MENU_MENUBAR_HEIGHT + COLOR_PICKER_PADDING, COLOR_PICKER_WIDTH, COLOR_PICKER_HEIGHT}, "Secondary Color", this->paint.secondaryColor);
    setRGBColorPicker(Rectangle{SELECTION_MENU_X + 3 * COLOR_PICKER_PADDING + 2 * (COLOR_PICKER_WIDTH + COLOR_PICKER_HUEBAR_PADDING + COLOR_PICKER_HUEBAR_WIDTH), SELECTION_MENU_Y + SELECTION_MENU_MENUBAR_HEIGHT + COLOR_PICKER_PADDING, COLOR_PICKER_WIDTH, COLOR_PICKER_HEIGHT}, "Tertiary Color" , this->paint.tertiaryColor );
}
