#include "include/utils.h"
#include "external/raylib.h"
#include <math.h>

int ClampInt(int val, int min, int max) {
    if (val < min) {
        return min;
    }
    if (val > max) {
        return max;
    }

    return val;
}

float GetRadAngleAtoB(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = a.y - b.y;

    return atan2f(dy, dx);
}

float GetDegAngleAtoB(Vector2 a, Vector2 b) {
    float rad = GetRadAngleAtoB(a, b);
    float deg = rad * RAD2DEG;
    if (deg < 0) {
        deg += 360.0f;
    }

    return deg;
}
