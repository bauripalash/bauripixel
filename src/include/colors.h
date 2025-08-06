#ifndef COLORS_H
#define COLORS_H

#define BpSolidColor(r, g, b) (Color){r, g, b, 0xff}

#define BpHexColor(x)         (int)0x##x##ff

// Sweetie 16 by GrafxKid
// from https://lospec.com/palette-list/sweetie-16

#define HexColorBlack         (int)0x1a1c2cff
#define HexColorPurple        (int)0x5d275dff
#define HexColorRedDark       (int)0xb13e53ff
#define HexColorOrange        (int)0xef7d57ff
#define HexColorYellow        (int)0xffcd75ff

#define HexColorGreenLightest (int)0xa7f070ff
#define HexColorGreenLighter  (int)0x38b764ff
#define HexColorGreenDarkest  (int)0x257179ff

#define HexColorBlueDarkest   (int)0x29366fff
#define HexColorBlueLighter   (int)0x3b5dc9ff
#define HexColorBlueLightest  (int)0x41a6f6ff
#define HexColorCyan          (int)0x73eff7ff

#define HexColorWhite         (int)0xf4f4f4ff
#define HexColorGrayLightest  (int)0x94b0c2ff
#define HexColorGrayLighter   (int)0x566c86ff
#define HexColorGrayDarker    (int)0x333c57ff

#define ColorBlack            (Color){0x1A, 0x1C, 0x2C, 0xFF}
#define ColorPurple           (Color){0x5D, 0x27, 0x5D, 0xFF}
#define ColorRedDark          (Color){0xB1, 0x3E, 0x53, 0xFF}
#define ColorOrange           (Color){0xEF, 0x7D, 0x57, 0xFF}
#define ColorYellow           (Color){0xFF, 0xCD, 0x75, 0xFF}

#define ColorGreenLightest    (Color){0xA7, 0xF0, 0x70, 0xFF}
#define ColorGreenLighter     (Color){0x38, 0xB7, 0x64, 0xFF}
#define ColorGreenDarker      (Color){0x25, 0x71, 0x79, 0xFF}

#define ColorBlueDarkest      (Color){0x29, 0x36, 0x6F, 0xFF}
#define ColorBlueLighter      (Color){0x3B, 0x5D, 0xC9, 0xFF}
#define ColorBlueLightest     (Color){0x41, 0xA6, 0xF6, 0xFF}
#define ColorCyan             (Color){0x73, 0xEF, 0xF7, 0xFF}

#define ColorWhite            (Color){0xF4, 0xF4, 0xF4, 0xFF}
#define ColorGrayLightest     (Color){0x94, 0xB0, 0xC2, 0xFF}
#define ColorGrayLighter      (Color){0x56, 0x6C, 0x86, 0xFF}
#define ColorGrayDarker       (Color){0x33, 0x3C, 0x57, 0xFF}

// Custom Colors
#define HexColorTransparent 0
#define ColorTransparent    (Color){0, 0, 0, 0}

#define ColorFDGrayLighter  (Fade(ColorGrayLighter, 0.4))

#define ColorCheckerDark    (Color){0xA8, 0xA8, 0xA8, 0xFF}
#define ColorCheckerLight   (Color){0xD8, 0xD8, 0xD8, 0xFF}

// #define ColorXDarkestBlue   (Color){0x0e, 0x0a, 0x2e, 0xff}
// #define ColorXDarkGray1     (Color){0x13, 0x17, 0x26, 0xFF}

#define HexColorBlack (int)0x1a1c2cff
#define ColorBlack    (Color){0x1A, 0x1C, 0x2C, 0xFF}

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
 * Winter Wonderland Palette by Jimison3
 * https://lospec.com/palette-list/winter-wonderland
 */

#define HexColorWBlueDarkest  BpHexColor(20284e)
#define HexColorWBlueDarker   BpHexColor(2c4a78)
#define HexColorWBlueLighter  BpHexColor(3875a1)
#define HexColorWBlueLightest BpHexColor(8bcadd)
#define HexColorWWhite        BpHexColor(ffffff)
#define HexColorWGrayLightest BpHexColor(d6e1e9)
#define HexColorWGrayLighter  BpHexColor(a7bcc9)
#define HexColorWGrayDarker   BpHexColor(738d9d)

#define ColorWBlueDarkest     BpSolidColor(0x20, 0x28, 0x4e)
#define ColorWBlueDarker      BpSolidColor(0x2c, 0x4a, 0x78)
#define ColorWBlueLighter     BpSolidColor(0x38, 0x75, 0xa1)
#define ColorWBlueLightest    BpSolidColor(0x8b, 0xca, 0xdd)
#define ColorWWhite           BpSolidColor(0xff, 0xff, 0xff)
#define ColorWGrayLightest    BpSolidColor(0xd6, 0xe1, 0xe9)
#define ColorWGrayLighter     BpSolidColor(0xa7, 0xbc, 0xc9)
#define ColorWGrayDarker      BpSolidColor(0x73, 0x8d, 0x9d)

/*
 * Seafoam Palette by Jimison3
 * https://lospec.com/palette-list/seafoam
 */

#define HexColorSGray          BpHexColor(37364e)
#define HexColorSGreenDarker   BpHexColor(355d69)
#define HexColorSGreenLighter  BpHexColor(6aae9d)
#define HexColorSGreenLightest BpHexColor(b9d4b4)
#define HexColorSWhite         BpHexColor(f4e9d4)
#define HexColorSWhiteDarker   BpHexColor(d0baa9)
#define HexColorSPurpleLight   BpHexColor(9e8e91)
#define HexColorSpurpleDark    BpHexColor(5b4a68)

#define ColorSGray             BpSolidColor(0x37, 0x36, 0x4e)
#define ColorSGreenDarker      BpSolidColor(0x35, 0x5d, 0x69)
#define ColorSGreenLighter     BpSolidColor(0x6a, 0xae, 0x9d)
#define ColorSGreenLightest    BpSolidColor(0xb9, 0xd4, 0xb4)
#define ColorSWhite            BpSolidColor(0xf4, 0xe9, 0xd4)
#define ColorSWhiteDarker      BpSolidColor(0xd0, 0xba, 0xa9)
#define ColorSPurpleLight      BpSolidColor(0x9e, 0x8e, 0x91)
#define ColorSpurpleDark       BpSolidColor(0x5b, 0x4a, 0x68)

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

#endif
