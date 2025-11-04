#ifndef MENUINFO_H
#define MENUINFO_H

#include <stdbool.h>
// Top Menu Actions
typedef enum {
	// New File/Sprite
    MACTION_NEW_FILE,
	// Open File
    MACTION_OPEN_FILE,
	// Save File
    MACTION_SAVE_FILE,
	// Import File
    MACTION_IMPORT_FILE,
	// Export File
    MACTION_EXPORT_FILE,
	// Quit Application
    MACTION_QUIT,

	// Export as plain single image
    MACTION_EXPORTAS_IMAGE,
	// Export as spritesheet
    MACTION_EXPORTAS_SHEET,

	// Undo Action
    MACTION_UNDO,
	// Redo Action
    MACTION_REDO,
	// Cut
    MACTION_CUT,
	// Copy
    MACTION_COPY,
	// Pase
    MACTION_PASTE,
	// Rotate Sprite/Canvas
    MACTION_ROTATE,
	// Rotate Sprite/Canvas Horizontally
    MACTION_FLIP_H,
	// Rotate Sprite/Canvas Vertically
    MACTION_FLIP_V,

	// Show About Window
    MACTION_SHOW_ABOUT,
	// Show Help Window
    MACTION_SHOW_HELP,

    // Count of Menu Action
    MACTION_COUNT
} MenuAction;

// Top Level Menu Items
typedef enum TopMenuInfo {
	// File Menu (New, Open, Save, Export etc)
    TMENU_FILE,
	// Edit Menu (Undo, Redo, Cut, Copy, Paste etc)
    TMENU_EDIT,
	// Help Menu (About, Help etc)
    TMENU_HELP,

	// Count of Top Level Menu
    TMENU_COUNT
} TopMenuInfo;

// Top Menu Item
typedef struct TopMenuEntry {
	// Menu Item Button name
    const char *name;
	// Which Menu ?
    TopMenuInfo info;
	// How many sub menu options
    int itemCount;
	// Width for popup panel
    float width;
	// Is active
    bool active;
} TopMenuEntry;

#endif
