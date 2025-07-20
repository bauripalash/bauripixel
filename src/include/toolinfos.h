#ifndef TOOLINFOS_H
#define TOOLINFOS_H

#include "components.h"
#include "drawtools.h"

const ToolInfo PenToolInfo[] = {
    {ICON_PENCIL, DT_PENCIL},
};

const ToolInfo EraserToolInfo[] = {{ICON_RUBBER, DT_ERASER}};

const ToolInfo LineToolInfo[] = {
    {ICON_WAVE_TRIANGULAR, DT_LINE},
};

const ToolInfo CircleToolInfo[] = {
    {ICON_PLAYER_RECORD, DT_CIRCLE},
    {ICON_BREAKPOINT_ON, DT_CIRCLE_FILL},
};

const ToolInfo RectToolInfo[] = {
    {ICON_PLAYER_STOP, DT_RECT},
    {ICON_GRID_FILL, DT_RECT_FILL},
};

const ToolInfo PanToolInfo[] = {
    {ICON_CURSOR_HAND, DT_PAN},
};

#endif
