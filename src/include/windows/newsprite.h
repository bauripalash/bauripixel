#ifndef WIN_NEWSPRITE_H
#define WIN_NEWSPRITE_H

#include "../defaults.h"
#include "window.h"
#include <stdbool.h>

// New Sprite Window State
typedef struct WNewSpriteState {
	// Base window prop
    WinProp p;
	// Sprite width
    int width;
	// Edit sprite width
    bool editWidth;
	// Hover on the sprite width slider
    bool hoverWidth;
	// Sprit height
    int height;
	// Edit sprite height
    bool editHeight;
	// Hover on the sprite height slider
    bool hoverHeight;
	// Sprite name
    char name[DEF_NAME_SIZE];
	// Is the name being edited
    bool editName;
	// Lock the aspect ratio of size
    bool sizeLinked;

} WNewSpriteState;

// Create new sprite window state
WNewSpriteState NewWNewSprite();
// Draw new sprite window
WinStatus WNewSprite(WNewSpriteState *state, double dt);
// Free new sprite window state
void FreeWNewSprite(WNewSpriteState *state);

#endif
