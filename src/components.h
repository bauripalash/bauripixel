/*
 * components.h - Component Definitions and Small Utilities
 * Copyright (C) <year>  <name of author>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BAURIPIXEL_COMPONENTS_H
#define BAURIPIXEL_COMPONENTS_H

#ifdef __cplusplus
extern "C" {
#endif
#include "external/raylib/raylib.h"

typedef struct BpSide {
    int top;
    int bottom;
    int left;
    int right;
} BpSide;

#define MakeSide(t, b, l, r)                                                   \
    ((BpSide){.top = t, .bottom = b, .left = l, .right = r})

// Vector4(x = Top, y = Left, z = Right, w = Bottom)
#define SideToVector4(sd)                                                      \
    ((Vector4){.x = sd.top, .y = sd.left, .z = sd.right, .w = sd.bottom})

// Vector4(x = Top, y = Left, z = Right, w = Bottom)
#define Vector4ToSide(vec)                                                     \
    ((Side){.top = vec.x, .bottom = vec.w, .left = vec.y, .right = vec.z})

// Make an all sides enabled Side
#define MakeAllSide() ((BpSide){0})
// Make an all sides disabled Side
#define MakeNoSide() ((BpSide){-1, -1, -1, -1})
// Check if Side is all sides enabled
#define IsAllSide(s)                                                           \
    (s.top != -1 && s.bottom != -1 && s.left != -1 && s.right != -1)

void BpSimplePanel(Rectangle bounds, int thickness, BpSide sides);
void BpPanelBorder(Rectangle bounds, int thickness);
void BpPanelOnlyBorder(Rectangle bounds, int thickness);
void BpPanelNoBorder(Rectangle bounds);

#ifdef __cplusplus
}
#endif

#endif
