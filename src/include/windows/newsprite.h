#ifndef WIN_NEWSPRITE_H
#define WIN_NEWSPRITE_H

#include "../defaults.h"
#include "window.h"
#include <stdbool.h>

typedef struct WNewSpriteState {
    WinProp p;
    int width;
    bool editWidth;
    bool hoverWidth;
    int height;
    bool editHeight;
    bool hoverHeight;
    char name[DEF_NAME_SIZE];
    bool editName;
    bool sizeLinked;

} WNewSpriteState;

WNewSpriteState NewWNewSprite();
WinStatus WNewSprite(WNewSpriteState *state, double dt);
void FreeWNewSprite(WNewSpriteState *state);

#endif
