#ifndef TOOLINFO_H
#define TOOLINFO_H

#include "components.h"
#include "drawtools.h"
typedef struct ToolInfo {
    DrawTool tool;
    GuiIconName iconTop;
    GuiIconName iconBottom;
} ToolInfo;

#endif // TOOLINFO_H
