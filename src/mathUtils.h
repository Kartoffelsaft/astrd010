#pragma once

#include "raymath.h"

auto const FORWARD = Vector3{ 0.0f,  0.0f, -1.0f};
auto const UP      = Vector3{ 0.0f,  1.0f,  0.0f};
auto const DOWN    = Vector3{ 0.0f, -1.0f,  0.0f};
auto const LEFT    = Vector3{-1.0f,  0.0f,  0.0f};
auto const RIGHT   = Vector3{ 1.0f,  0.0f,  0.0f};
auto const BACK    = Vector3{ 0.0f,  0.0f,  1.0f};

enum Dimension {
    X,
    Y,
    Z
};

float frobeniusDotProduct(Matrix const a, Matrix const b);

Vector3 orthogonalize(Vector3 const v);