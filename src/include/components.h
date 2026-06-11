#ifndef BAURIPIXEL_COMPONENTS_H
#define BAURIPIXEL_COMPONENTS_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../external/raylib/raylib.h"

void BpPanelBorder(Rectangle bounds, int thickness);
void BpPanelOnlyBorder(Rectangle bounds, int thickness);
void BpPanelNoBorder(Rectangle bounds);

#ifdef __cplusplus
}
#endif

#endif
