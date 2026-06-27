/*
 * colors.h - Application Colors Definitions
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

#ifndef BAURIPIXEL_COLORS_H
#define BAURIPIXEL_COLORS_H

// Convert RGB value to Color with Alpha of 0xff
#define BpSolidColor(r, g, b) (Color){r, g, b, 0xff}

// Convert RGB Color to RGBA Color with Alpha of 0xff
#define BpHexColor(x)           (int)0x##x##ff

#define BpRGB2SolidRGBA(rgb)    (((rgb) << 8) | 0xFFu)
#define BpRGB2AlphaRGBA(rgb, a) (((rgb) << 8) | a)
#define BpColorSetAlpha(clr, alpha)                                            \
    ((Color){.r = clr.r, .g = clr.g, .b = clr.b, .a = alpha})

// Velstad-4-by-mainzero Palette by Arkzero87
// https://lospec.com/palette-list/velstad-4-by-mainzero

#define BpHexColorVGray      (int)0x131726ff
#define BpHexColorVGrayLight (int)0x59608eff
#define BpHexColorVGreen     (int)0x61b8aeff
#define BpHexColorVWhite     (int)0xdde0bdff

#define BpColorVGray         (Color){0x13, 0x17, 0x26, 0xff}
#define BpColorVGrayLight    (Color){0x59, 0x60, 0x8e, 0xff}
#define BpColorVGreen        (Color){0x61, 0xb8, 0xae, 0xff}
#define BpColorVWhite        (Color){0xdd, 0xe0, 0xbd, 0xff}

/*
 * Neon Night Sky Palette by SoundsDotZip
 * https://lospec.com/palette-list/neon-night-sky
 */

#define BpHexColorNBlack        BpHexColor(131626)
#define BpHexColorNGray         BpHexColor(4d4d80)
#define BpHexColorNMagentaLight BpHexColor(e6a1cf)
#define BpHexColorNPinkLight    BpHexColor(ffe6ea)

#define BpColorNBlack           BpSolidColor(0x13, 0x16, 0x26)
#define BpColorNGray            BpSolidColor(0x4d, 0x4d, 0x80)
#define BpColorNMagentaLight    BpSolidColor(0xe6, 0xa1, 0xcf)
#define BpColorNPinkLight       BpSolidColor(0xff, 0xe6, 0xea)

/*
 * Cozy Camp Palette by Pin_Badger
 * https://lospec.com/palette-list/cozy-camp
 */

#define BpHexColorCBlack  BpHexColor(071721)
#define BpHexColorCGray   BpHexColor(30455c)
#define BpHexColorCOrange BpHexColor(c07e53)
#define BpHexColorCWhite  BpHexColor(f8eacf)

#define BpColorCBlack     BpSolidColor(0x07, 0x17, 0x21)
#define BpColorCGray      BpSolidColor(0x30, 0x45, 0x5c)
#define BpColorCOrange    BpSolidColor(0xc0, 0x7e, 0x53)
#define BpColorCWhite     BpSolidColor(0xf8, 0xea, 0xcf)

/*
 * Twilight 5 Palette by Star
 * https://lospec.com/palette-list/twilight-5
 */

#define BpHexColorTPinkLight    BpHexColor(fbbbad)
#define BpHexColorTPink         BpHexColor(ee8695)
#define BpHexColorTGrayLightest BpHexColor(4a7a96)
#define BpHexColorTGrayLighter  BpHexColor(333f58)
#define BpHexColorTGrayDarker   BpHexColor(292831)

#define BpColorTPinkLight       BpSolidColor(0xfb, 0xbb, 0xad)
#define BpColorTPink            BpSolidColor(0xee, 0x86, 0x95)
#define BpColorTGrayLightest    BpSolidColor(0x4a, 0x7a, 0x96)
#define BpColorTGrayLighter     BpSolidColor(0x33, 0x3f, 0x58)
#define BpColorTGrayDarker      BpSolidColor(0x29, 0x28, 0x31)

/*
 * Custom Color
 */

// Based on Twilight 5 Darkest Grey
#define BpHexColorAppBG BpHexColor(17171c)
#define BpColorAppBG    BpSolidColor(0x17, 0x17, 0x1c)

// Based on Twilight 5 Darkest Grey
#define BpHexColorStatus      BpHexColor(1d1c23)
#define BpColorStatus         BpSolidColor(0x1d, 0x1c, 0x23)

#define BpHexColorPanelBorder BpHexColorTGrayLighter

#define BpHexColorTransparent 0
#define BpColorTransparent    (Color){0, 0, 0, 0}

#define BpColorCheckerDark    BpSolidColor(0xA8, 0xA8, 0xA8)
#define BpColorCheckerLight   BpSolidColor(0xD8, 0xD8, 0xD8)

#define BpHexColorWhite       BpHexColor(ffffff)
#define BpColorWhite          BpSolidColor(0xff, 0xff, 0xff)

#define BpHexColorBlack       BpHexColor(000000)
#define BpColorBlack          BpSolidColor(0x00, 0x00, 0x00)

#endif
