#ifndef COLORS_H
#define COLORS_H

// Convert RGB value to Color with Alpha of 0xff
#define BpSolidColor(r, g, b) (Color){r, g, b, 0xff}

// Convert RGB Color to RGBA Color with Alpha of 0xff
#define BpHexColor(x)           (int)0x##x##ff

#define BpRGB2SolidRGBA(rgb)    (((rgb) << 8) | 0xFFu)
#define BpRGB2AlphaRGBA(rgb, a) (((rgb) << 8) | a)

// Velstad-4-by-mainzero Palette by Arkzero87
// https://lospec.com/palette-list/velstad-4-by-mainzero

#define HexColorVGray      (int)0x131726ff
#define HexColorVGrayLight (int)0x59608eff
#define HexColorVGreen     (int)0x61b8aeff
#define HexColorVWhite     (int)0xdde0bdff

#define ColorVGray         (Color){0x13, 0x17, 0x26, 0xff}
#define ColorVGrayLight    (Color){0x59, 0x60, 0x8e, 0xff}
#define ColorVGreen        (Color){0x61, 0xb8, 0xae, 0xff}
#define ColorVWhite        (Color){0xdd, 0xe0, 0xbd, 0xff}

#define ColorYGray         (Color){0x35, 0x33, 0x3f, 0xff}

/*
 * Twilight 5 Palette by Star
 * https://lospec.com/palette-list/twilight-5
 */

#define HexColorTPinkLight    BpHexColor(fbbbad)
#define HexColorTPink         BpHexColor(ee8695)
#define HexColorTGrayLightest BpHexColor(4a7a96)
#define HexColorTGrayLighter  BpHexColor(333f58)
#define HexColorTGrayDarker   BpHexColor(292831)

#define ColorTPinkLight       BpSolidColor(0xfb, 0xbb, 0xad)
#define ColorTPink            BpSolidColor(0xee, 0x86, 0x95)
#define ColorTGrayLightest    BpSolidColor(0x4a, 0x7a, 0x96)
#define ColorTGrayLighter     BpSolidColor(0x33, 0x3f, 0x58)
#define ColorTGrayDarker      BpSolidColor(0x29, 0x28, 0x31)

/*
 * Neon Night Sky Palette by SoundsDotZip
 * https://lospec.com/palette-list/neon-night-sky
 */

#define HexColorNBlack        BpHexColor(131626)
#define HexColorNGray         BpHexColor(4d4d80)
#define HexColorNMagentaLight BpHexColor(e6a1cf)
#define HexColorNPinkLight    BpHexColor(ffe6ea)

#define ColorNBlack           BpSolidColor(0x13, 0x16, 0x26)
#define ColorNGray            BpSolidColor(0x4d, 0x4d, 0x80)
#define ColorNMagentaLight    BpSolidColor(0xe6, 0xa1, 0xcf)
#define ColorNPinkLight       BpSolidColor(0xff, 0xe6, 0xea)

/*
 * Custom Color
 *
 */

// Based on Twilight 5 Darkest Grey
#define HexColorAppBG BpHexColor(17171c)
#define ColorAppBG    BpSolidColor(0x17, 0x17, 0x1c)

// Based on Twilight 5 Darkest Grey
#define HexColorStatus      BpHexColor(1d1c23)
#define ColorStatus         BpSolidColor(0x1d, 0x1c, 0x23)

#define HexColorPanelBorder HexColorTGrayLighter

#define HexColorTransparent 0
#define ColorTransparent    (Color){0, 0, 0, 0}

#define ColorCheckerDark    BpSolidColor(0xA8, 0xA8, 0xA8)
#define ColorCheckerLight   BpSolidColor(0xD8, 0xD8, 0xD8)

#define HexColorWhite       BpHexColor(ffffff)
#define ColorWhite          BpSolidColor(0xff, 0xff, 0xff)

#define HexColorBlack       BpHexColor(000000)
#define ColorBlack          BpSolidColor(0x00, 0x00, 0x00)

#endif
