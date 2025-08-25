#include "include/utils.h"
#include "external/raygui.h"
#include "external/raylib.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

void MakeVecZero(Vector2 *vec) {
    vec->x = 0;
    vec->y = 0;
}

char *MakeString(const char *str) { return strdup(str); }

bool IsOsWeb() {
#ifdef BPM_OS_WEB
    return true;
#else
    return false;
#endif
}
bool IsOsLinux() {
#ifdef BPM_OS_LINUX
    return true;
#else
    return false;
#endif
}
bool IsOsMac() {
#ifdef BPM_OS_MAC
    return true;
#else
    return false;
#endif
}
bool IsOsWin() {
#ifdef BPM_OS_WIN
    return true;
#else
    return false;
#endif
}
