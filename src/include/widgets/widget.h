#ifndef WIDGET_H
#define WIDGET_H

#include "../../external/raylib.h"
#include <stdbool.h>

typedef struct {
    bool active;
    Rectangle bounds;
} WidgetProp;

WidgetProp NewWidgetProp();
WidgetProp NewWidgetPropRec(Rectangle rect);
void ActivateWidget(WidgetProp *prop, bool active);
void SetWidgetBounds(WidgetProp *prop, Rectangle rect);

#endif
