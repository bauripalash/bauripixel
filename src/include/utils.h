#ifndef UTILS_H
#define UTILS_H

#include "../external/raylib.h"
#include <stdbool.h>

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

#define SideAll()         (Side){0}
#define SideNone()        (Side){-1, -1, -1, -1}

#define IsAllSide(s)      (s.t != -1 && s.b != -1 && s.l != -1 && s.r != -1)

#define ArrCount(arr)     (sizeof(arr) / sizeof(arr[0]))
#define IsVecNeg(vec)     (vec.x < 0 && vec.y < 0)
#define Vector2Negetive() ((Vector2){-1, -1})

#define TraceVector(vec, msg)                                                  \
    TraceLog(LOG_WARNING, "%s [%f, %f]", msg, vec.x, vec.y)

#define TraceRect(r, msg)                                                      \
    TraceLog(                                                                  \
        LOG_WARNING, "%s [[%f, %f] [%f, %f]]", msg, r.x, r.y, r.width,         \
        r.height                                                               \
    )

#define DegToRad(d) ((float)(d * DEG2RAD))
#define RadToDeg(r) ((float)(r * RAD2DEG))

// If left/right shift key is being pressed down
#define IsShiftDown() (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))

// If left/right control key is being pressed down
#define IsCtrlDown()                                                           \
    (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))

// If left/right alt key is being pressed down
#define IsAltDown() (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))

// If left/right Meta/Super/Windows key is being pressed down
#define IsSuperDown() (IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER))

#define PrintEnum(d)  TraceLog(LOG_WARNING, "%s", #d)

// BeginScissorMode but with Rectangle
#define BeginScissorModeRec(r) BeginScissorMode(r.x, r.y, r.width, r.height)

// Functions

// Clamp integer between min and max
int ClampInt(int val, int min, int max);

// Get angle from a to b in Degrees;
// Right = 0 degree; Left = 180; Top = 90; Bottom = 270;
float GetDegAngleAtoB(Vector2 a, Vector2 b);

// Get angle from a to b in Radians;
float GetRadAngleAtoB(Vector2 a, Vector2 b);

// Make Value of a Vector2 zero
void MakeVecZero(Vector2 *vec);

// Create a malloc'd string
char *MakeString(const char *str);

// Return a value with specified prec
float FloatWithPrec(float value, float prec);

// Return minimum clamped value
float ClampMin(float value, float min);

// Return maximum clamped value
float ClampMax(float value, float max);

// Check if `any` of the values of the Rects are different
bool RectAnyDiff(Rectangle a, Rectangle b);
// Check if `all` of the values of the Rects are different
bool RectAllDiff(Rectangle a, Rectangle b);

// If Shift, Ctrl or Meta/Windows key is being pressed down
bool IsModifierDown();

bool IsOsWeb();
bool IsOsLinux();
bool IsOsMac();
bool IsOsWin();
#endif
