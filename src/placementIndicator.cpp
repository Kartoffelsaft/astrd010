#include <vector>

#include "raylib.h"
#include "raygui.h"

#include "mathUtils.h"
#include "placementIndicator.h"
#include "shaders.h"
#include "input.h"

std::vector<Mesh> placeableShapes{PLACEABLE_SHAPES_COUNT};
std::vector<std::vector<Vector3>> placeableSnapPoints{PLACEABLE_SHAPES_COUNT};
std::vector<Material> placeableTextures{PLACEABLE_TEXTURES_COUNT};

void PlacementIndicator::update() {
    Vector3 rotDelta = Vector3Zero();
    if (IsKeyDown(KEY_HOME)) {
        rotDelta = Vector3Add(rotDelta, LEFT);
    }
    if (IsKeyDown(KEY_END)) {
        rotDelta = Vector3Add(rotDelta, RIGHT);
    }
    if (IsKeyDown(KEY_DELETE)) {
        rotDelta = Vector3Add(rotDelta, DOWN);
    }
    if (IsKeyDown(KEY_PAGE_DOWN)) {
        rotDelta = Vector3Add(rotDelta, UP);
    }
    if (IsKeyDown(KEY_INSERT)) {
        rotDelta = Vector3Add(rotDelta, BACK);
    }
    if (IsKeyDown(KEY_PAGE_UP)) {
        rotDelta = Vector3Add(rotDelta, FORWARD);
    }
    
    if (IsKeyPressed(KEY_Z)) {
        this->selectionMenuIsOpen ^= true;
        mouseLookMode = !this->selectionMenuIsOpen;
    }

    this->rotation = MatrixMultiply(
        MatrixRotateZYX(Vector3Scale(rotDelta, 0.3 * GetFrameTime())),
        this->rotation
    );

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
