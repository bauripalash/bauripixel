#ifndef MENUINFO_H
#define MENUINFO_H

#include <stdbool.h>
typedef enum {
    MACTION_NEW_FILE,
    MACTION_OPEN_FILE,
    MACTION_SAVE_FILE,
    MACTION_IMPORT_FILE,
    MACTION_EXPORT_FILE,
    MACTION_QUIT,

    MACTION_EXPORTAS_IMAGE,
    MACTION_EXPORTAS_SHEET,

    MACTION_UNDO,
    MACTION_REDO,
    MACTION_CUT,
    MACTION_COPY,
    MACTION_PASTE,
    MACTION_ROTATE,
    MACTION_FLIP_H,
    MACTION_FLIP_V,

    MACTION_SHOW_ABOUT,

    //
    MACTION_COUNT
} MenuAction;

typedef enum TopMenuInfo {
    TMENU_FILE,
    TMENU_EDIT,
    TMENU_HELP,

    TMENU_COUNT
} TopMenuInfo;

typedef struct TopMenuEntry {
    const char *name;
    TopMenuInfo info;
    int itemCount;
    float width;
    bool active;
} TopMenuEntry;

#endif
