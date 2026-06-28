/*
 * alloc.h - Wrapper for Allocation, Reallocation, Freeing
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

#ifndef BAURIPIXEL_ALLOC_H
#define BAURIPIXEL_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>

#define PMalloc(size)        (malloc((size_t)size))
#define PCalloc(count, size) (calloc((size_t)count, (size_t)size))
#define PRealloc(ptr, size)  (realloc(ptr, (size_t)(size)))
#define PCreate(type)        ((type *)(malloc(sizeof(type))))
#define PCreateArray(type, count)                                              \
    ((type *)(malloc(sizeof(type) * (size_t)(count))))
#define PFree(ptr) (free(ptr))

#ifdef __cplusplus
}
#endif

#endif
