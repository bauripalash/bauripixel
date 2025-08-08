#ifndef UTILS_H
#define UTILS_H

#include "../external/raylib.h"

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
