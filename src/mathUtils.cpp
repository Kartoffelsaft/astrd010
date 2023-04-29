#include "mathUtils.h"

float frobeniusDotProduct(Matrix const a, Matrix const b) {
    float sum = 0.f;
    auto ap = (float const *)&a;
    auto bp = (float const *)&b;

    auto start = ap;

    while (ap - start < 16) {
        sum += *ap * *bp;

        ap++;
        bp++;
    }

    return sum;
}

Vector3 orthogonalize(Vector3 const v) {
    if (fabs(v.x) > fabs(v.y) && fabs(v.x) > fabs(v.z)) return Vector3{copysignf(1.f, v.x), 0.f, 0.f};
    if (fabs(v.y) > fabs(v.x) && fabs(v.y) > fabs(v.z)) return Vector3{0.f, copysignf(1.f, v.y), 0.f};
    else return Vector3{0.f, 0.f, copysignf(1.f, v.z)};
}