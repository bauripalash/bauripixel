#ifndef COLORS_H
#define COLORS_H

#define BpSolidColor(r, g, b) (Color){r, g, b, 0xff}

#define BpHexColor(x)         (int)0x##x##ff

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

// LTRO-2 Palette by JamesTDG
// https://lospec.com/palette-list/ltro-2

#define HexColorWhite        (int)0xeae1f0ff
#define HexColorGrayLightest (int)0x837185ff
#define HexColorGrayLighter  (int)0x36313bff
#define HexColorGrayDarker   (int)0x1c1c1fff
#define HexColorRedLight     (int)0xf4a3a6ff
#define HexColorRed          (int)0xf63f4cff
#define HexColorPurple       (int)0x8e3e77ff
// #define HexColorBlueLightest  (int)0x69d2ebff
// #define HexColorBlueLighter   (int)0x6883d8ff
// #define HexColorBlueDarker    (int)0x574fb0ff
#define HexColorGreenishLight (int)0xc6c283ff
#define HexColorGreenishDark  (int)0x8d902eff
#define HexColorBrownDark     (int)0x8b5b37ff
#define HexColorYellowLight   (int)0xf4db7eff
#define HexColorYellow        (int)0xfdbb27ff
#define HexColorBrownLight    (int)0xc36b33ff
// #define Color (int)0x

#define ColorWhite         (Color){0xea, 0xe1, 0xf0, 0xff}
#define ColorGrayLightest  (Color){0x83, 0x71, 0x85, 0xff}
#define ColorGrayLighter   (Color){0x36, 0x31, 0x3b, 0xff}
#define ColorGrayDarker    (Color){0x1c, 0x1c, 0x1f, 0xff}
#define ColorRedLight      (Color){0xf4, 0xa3, 0xa6, 0xff}
#define ColorRed           (Color){0xf6, 0x3f, 0x4c, 0xff}
#define ColorPurple        (Color){0x8e, 0x3e, 0x77, 0xff}
#define ColorBlueLightest  (Color){0x69, 0xd2, 0xeb, 0xff}
#define ColorBlueLighter   (Color){0x68, 0x83, 0xd8, 0xff}
#define ColorBlueDarker    (Color){0x57, 0x4f, 0xb0, 0xff}
#define ColorGreenishLight (Color){0xc6, 0xc2, 0x83, 0xff}
#define ColorGreenishDark  (Color){0x8d, 0x90, 0x2e, 0xff}
#define ColorBrownDark     (Color){0x8b, 0x5b, 0x37, 0xff}
#define ColorYellowLight   (Color){0xf4, 0xdb, 0x7e, 0xff}
#define ColorYellow        (Color){0xfd, 0xbb, 0x27, 0xff}
#define ColorBrownLight    (Color){0xc3, 0x6b, 0x33, 0xff}

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
