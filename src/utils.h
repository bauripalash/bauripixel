/*
 * utils.h - Utilities Definitions
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
