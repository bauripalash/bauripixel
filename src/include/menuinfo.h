#ifndef MENUINFO_H
#define MENUINFO_H

#include <stdbool.h>
typedef enum {
    MACTION_NEW_FILE,
    MACTION_OPEN_FILE,
    MACTION_SAVE_FILE,

    MACTION_SHOW_ABOUT,

    //
    MACTION_COUNT
} MenuAction;

typedef enum TopMenuInfo {
    TMENU_FILE,
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
