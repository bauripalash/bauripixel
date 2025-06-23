#include "../include/widgets/widget.h"
#include <stdbool.h>

WidgetProp NewWidgetProp() {
    WidgetProp prop = {0};

    prop.active = false;
    prop.bounds = (Rectangle){0, 0, 0, 0};

    return prop;
}
WidgetProp NewWidgetPropRec(Rectangle rect) {
    WidgetProp prop = NewWidgetProp();
    prop.bounds = rect;

    return prop;
}
void ActivateWidget(WidgetProp *prop, bool active) { prop->active = active; }
void SetWidgetBounds(WidgetProp *prop, Rectangle rect) { prop->bounds = rect; }
