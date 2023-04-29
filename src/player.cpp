#include "raylib.h"
#include "raymath.h"

#include "mathUtils.h"
#include "placementIndicator.h"
#include "player.h"
#include "input.h"
#include "constructpart.h"

Camera3D Player::GetCamera() const {
    auto to = Vector3Transform(
        FORWARD,
        this->rotation
    );
    auto up = Vector3Transform(
        UP,
        this->rotation
    );

    to = Vector3Add(to, this->position);

    return Camera3D{
        .position = this->position,
        .target = to,
        .up = up,
        .fovy = 90.0f,
        .projection = CAMERA_PERSPECTIVE,
    };
}

Vector3 Player::getRelPos(Vector3 pos) const {
    return Vector3Add(this->position, Vector3Transform(pos, this->rotation));
}

Matrix Player::getPlacementIndicatorOrientation() const {
    auto pos = this->getRelPos(Vector3Scale(FORWARD, this->placementIndicator.distance));
    auto indicatorOrientation = MatrixMultiply(
        this->placementIndicator.getRotationMatrix(),
        MatrixTranslate(pos.x, pos.y, pos.z)
    );
    
    for (auto indicatorSnapPoint: placeableSnapPoints[this->placementIndicator.placeableSelection]) {
        auto worldPosIndicatorSnapPoint = Vector3Transform(indicatorSnapPoint, indicatorOrientation);
        
        for (auto part: shipParts) for (auto partSnapPoint: placeableSnapPoints[part.shape]) {
            auto worldPosPartSnapPoint = Vector3Transform(partSnapPoint, part.where);
            
            if (Vector3DistanceSqr(worldPosIndicatorSnapPoint, worldPosPartSnapPoint) < 0.2) {
                auto posDelta = Vector3Subtract(worldPosPartSnapPoint, worldPosIndicatorSnapPoint);
                return MatrixMultiply(indicatorOrientation, MatrixTranslate(posDelta.x, posDelta.y, posDelta.z));
            }
        }
    }
    
    return indicatorOrientation;
}

void Player::update() {
    auto mouseDelta = GetMouseDelta();

    if (mouseLookMode) {
        auto extraRotation = MatrixRotateZYX(Vector3{
            mouseDelta.y * -0.01f,
            mouseDelta.x * -0.01f, 
            0.0f
        });

        this->rotation = MatrixMultiply(extraRotation, this->rotation);
    }

    auto posDelta = Vector3{0.0, 0.0, 0.0};

    if (IsKeyDown(KEY_W)) {
        posDelta = Vector3Add(posDelta, FORWARD);
    } 
    if (IsKeyDown(KEY_S)) {
        posDelta = Vector3Add(posDelta, BACK);
    } 
    if (IsKeyDown(KEY_A)) {
        posDelta = Vector3Add(posDelta, LEFT);
    } 
    if (IsKeyDown(KEY_D)) {
        posDelta = Vector3Add(posDelta, RIGHT);
    } 
    if (IsKeyDown(KEY_R)) {
        posDelta = Vector3Add(posDelta, UP);
    } 
    if (IsKeyDown(KEY_F)) {
        posDelta = Vector3Add(posDelta, DOWN);
    } 

    posDelta = Vector3Transform(
        posDelta,
        this->rotation
    );

    this->position = Vector3Add(this->position, Vector3Scale(posDelta, 2.0f * GetFrameTime()));

    this->placementIndicator.update(this->rotation);
}

void Player::drawPlacementIndicator() const {
    this->placementIndicator.drawAt(this->getPlacementIndicatorOrientation());
}
