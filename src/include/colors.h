#ifndef COLORS_H
#define COLORS_H

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
#define HexColorGrayDarkest   (int)0x333c57ff

#define ColorBlack            (Color){0x1A, 0x1C, 0x2C, 0xFF}
#define ColorPurple           (Color){0x5D, 0x27, 0x5D, 0xFF}
#define ColorRedDark          (Color){0xB1, 0x3E, 0x53, 0xFF}
#define ColorOrange           (Color){0xEF, 0x7D, 0x57, 0xFF}
#define ColorYellow           (Color){0xFF, 0xCD, 0x75, 0xFF}

#define ColorGreenLightest    (Color){0xA7, 0xF0, 0x70, 0xFF}
#define ColorGreenLighter     (Color){0x38, 0xB7, 0x64, 0xFF}
#define ColorGreenDarkest     (Color){0x25, 0x71, 0x79, 0xFF}

#define ColorBlueDarkest      (Color){0x29, 0x36, 0x6F, 0xFF}
#define ColorBlueLighter      (Color){0x3B, 0x5D, 0xC9, 0xFF}
#define ColorBlueLightest     (Color){0x41, 0xA6, 0xF6, 0xFF}
#define ColorCyan             (Color){0x73, 0xEF, 0xF7, 0xFF}

#define ColorWhite            (Color){0xF4, 0xF4, 0xF4, 0xFF}
#define ColorGrayLightest     (Color){0x94, 0xB0, 0xC2, 0xFF}
#define ColorGrayLighter      (Color){0x56, 0x6C, 0x86, 0xFF}
#define ColorGrayDarkest      (Color){0x33, 0x3C, 0x57, 0xFF}

// Custom Colors
#define HexColorTransparent 0
#define ColorTransparent    (Color){0, 0, 0, 0}

#define ColorFDGrayLighter  (Fade(ColorGrayLighter, 0.4))
#endif
