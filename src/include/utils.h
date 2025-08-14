#ifndef UTILS_H
#define UTILS_H

#include "../external/raylib.h"

typedef struct Side {
    int t;
    int b;
    int l;
    int r;
} Side;

// Create New Side(Top, Bottom, Left, Right)
#define NewSide(tp, bt, lf, rt)                                                \
    (Side) { .t = tp, .b = bt, .l = lf, .r = rt }

// Vector4(x = Top, y = Left, z = Right, w = Bottom)
#define SideToVector4(sd)                                                      \
    (Vector4) { .x = sd.t, .y = sd.l, .z = sd.r, .w = sd.b }

#define Vector4ToSide(vec)                                                     \
    (Side) { .t = vec.x, .b = vec.w, .l = vec.y, .r = vec.z }

#define SideAll()     (Side){0}
#define SideNone()    (Side){-1, -1, -1, -1}

#define IsAllSide(s)  (s.t != -1 && s.b != -1 && s.l != -1 && s.r != -1)

#define ArrCount(arr) (sizeof(arr) / sizeof(arr[0]))

#define TraceVector(vec, msg)                                                  \
    TraceLog(LOG_WARNING, "%s [%f, %f]", msg, vec.x, vec.y)

#define TraceRect(r, msg)                                                      \
    TraceLog(                                                                  \
        LOG_WARNING, "%s [[%f, %f] [%f, %f]]", msg, r.x, r.y, r.width,         \
        r.height                                                               \
    )

#define DegToRad(d) ((float)(d * DEG2RAD))
#define RadToDeg(r) ((float)(r * RAD2DEG))

int ClampInt(int val, int min, int max);

// Get angle from a to b in Degrees;
// Right = 0 degree; Left = 180; Top = 90; Bottom = 270;
float GetDegAngleAtoB(Vector2 a, Vector2 b);

// Get angle from a to b in Radians;
float GetRadAngleAtoB(Vector2 a, Vector2 b);

// Make Value of a Vector2 zero
void MakeVecZero(Vector2 *vec);
char *MakeString(const char *str);
#endif
