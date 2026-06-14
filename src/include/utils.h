#ifndef BAURIPIXEL_UTILS_H
#define BAURIPIXEL_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

// Raylib Utils

#define BeginScissorModeRec(rect)                                              \
    (BeginScissorMode(rect.x, rect.y, rect.width, rect.height))

#define ArrCount(arr) (sizeof(arr) / sizeof(arr[0]))

#ifdef __cplusplus
}
#endif

#endif
