#ifndef COLORS_H
#define COLORS_H

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

#define HexColorXGray      (int)0x131726ff
#define HexColorXGrayLight (int)0x59608eff
#define HexColorXGreen     (int)0x61b8aeff
#define HexColorXWhite     (int)0xdde0bdff

#define ColorXGray         (Color){0x13, 0x17, 0x26, 0xff}
#define ColorXGrayLight    (Color){0x59, 0x60, 0x8e, 0xff}
#define ColorXGreen        (Color){0x61, 0xb8, 0xae, 0xff}
#define ColorXWhite        (Color){0xdd, 0xe0, 0xbd, 0xff}

#define ColorYGray         (Color){0x35, 0x33, 0x3f, 0xff}
#endif
